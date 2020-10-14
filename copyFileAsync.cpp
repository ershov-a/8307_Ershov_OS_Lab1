#include "copyFileAsync.h"

// Размер блока
int blockSize = 4096;
int blockSizeFactor;
CHAR **buffersArray;
OVERLAPPED *overlappedInput, *overlappedOutput;
HANDLE originalFileHandle, destinationFileHandle;
LARGE_INTEGER fileSize, endOfFile;
LONGLONG doneCount, blocksCount;

// Количество перекрывающихся операций
int numberOfOverlappingOperations = 0;

void copyFileAsync() {

    // Множитель размера блока
    blockSizeFactor = 1;

    std::string sourceFileName, destinationFileName;
    std::cout << "Enter source file name " << std::endl;
    std::cin >> sourceFileName;
    std::cout << "Enter destination file name"  << std::endl;
    std::cin >> destinationFileName;
    std::cout << "Enter number of overlapping operations" << std::endl;
    std::cin >> numberOfOverlappingOperations;
    std::cout << "Enter block factor" << std::endl;
    std::cin >> blockSizeFactor;
    blockSize *= blockSizeFactor;
    std::cout << "Block size is " << blockSize << " bytes" << std::endl;

    // Выделение памяти для чтения
    buffersArray = new CHAR*[numberOfOverlappingOperations];
    for (int i = 0; i < numberOfOverlappingOperations; i++)
        buffersArray[i] = new CHAR[blockSize];

    // Выделение памяти для перекрвающихся операций чтения и записи
    overlappedInput = new OVERLAPPED[numberOfOverlappingOperations];
    overlappedOutput = new OVERLAPPED[numberOfOverlappingOperations];

    LARGE_INTEGER currentPositionInput;

    // Получение HANDLE'ов оригинального файла и его копии
    originalFileHandle = CreateFile(sourceFileName.c_str(),
                                    GENERIC_READ,
                                    0,
                                    nullptr,
                                    OPEN_EXISTING,
                                    FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING,
                                    nullptr);
    destinationFileHandle = CreateFile(destinationFileName.c_str(),
                                       GENERIC_WRITE,
                                       0,
                                       nullptr,
                                       CREATE_ALWAYS,
                                       FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING,
                                       nullptr);

    // Получение размера оригинального файла
    GetFileSizeEx(originalFileHandle, &fileSize);

    std::cout << "Original file size is " << fileSize.QuadPart << " bytes." << std::endl;

    // Подсчет количества целых блоков в файле
    blocksCount = fileSize.QuadPart / blockSize;
    // Добавляем один блок, если файл не умещается в целое количество блоков
    if ((fileSize.QuadPart % blockSize) != 0)
        blocksCount++;
    std::cout << "Original file fits in " << blocksCount << " blocks" << std::endl;

    DWORD startCopyTime, endCopyTime;
    // Начинаем считать время, затраченное на копирование
    startCopyTime = GetTickCount();

    currentPositionInput.QuadPart = 0;

    // Запускаем несколько процессов чтения оригинального файла
    for (int i = 0; i < numberOfOverlappingOperations; ++i) {
        // Несмотря на то, что надобность в hEvent отсутствует,
        // передаем индекс буфера (порядковый номер операции) для дальнейшего доступа по индексу
        overlappedInput[i].hEvent = (HANDLE)i;
        overlappedOutput[i].hEvent = (HANDLE)i;
        // Подготавливаем очередную OVERLAPPED структуру,
        // устанавливая позицию (смещение) как в оригинальном файле
        overlappedInput[i].Offset = currentPositionInput.LowPart;
        overlappedInput[i].OffsetHigh = currentPositionInput.HighPart;
        // Если файл еще не закончился, то продолжаем читать
        if (currentPositionInput.QuadPart < fileSize.QuadPart)
            ReadFileEx(originalFileHandle, // Читаем из оригинального файла
                       buffersArray[i], // Читаем в буфер
                       blockSize, // Размера блока для чтения
                       &overlappedInput[i], // Здесь информация о позиции в файле
                       onReadDone // Процедура завершения записи
            );
        // Перемещаем позицию в оригинальном файле на одни блок вперед
        currentPositionInput.QuadPart += (LONGLONG)blockSize;
    }

    doneCount = 0;
    // Ждем, пока не завершатся все процедуры завершения, поставленные в очередь в цикле выше
    // 2 * blocksCount потому что у нас две процедуры завершения ( для чтения и для записи)
    while (doneCount < 2 * blocksCount)
        SleepEx(INFINITE, true);

    std::cout << "File copied successfully" << std::endl;

    delete[] overlappedInput;
    delete[] overlappedOutput;
    for (int i = 0; i < numberOfOverlappingOperations; ++i)
        delete[]buffersArray[i];
    delete[] buffersArray;

    // Избавляемся от "мусора" в конце скопированного файла
    endOfFile.QuadPart = fileSize.QuadPart;
    endOfFile.HighPart = fileSize.HighPart;
    SetFilePointerEx(destinationFileHandle, endOfFile, nullptr, FILE_BEGIN);
    SetEndOfFile(destinationFileHandle);

    // Закрываем открытые HANDLE'ы
    CloseHandle(originalFileHandle);
    CloseHandle(destinationFileHandle);

    endCopyTime = GetTickCount();
    std::cout << "Time elapsed " << endCopyTime - startCopyTime << " ms" << std::endl;
}

VOID WINAPI onReadDone(DWORD Code, DWORD nBytes, LPOVERLAPPED lpOv) {
    // Чтение закончилось, инициируем запись
    doneCount++;

    LARGE_INTEGER currentPositionInput, currentPositionOutput;
    // Получаем индекс текущей перекрывающейся операции
    auto i = (DWORD)(lpOv->hEvent);

    currentPositionInput.LowPart = overlappedInput[i].Offset;
    currentPositionInput.HighPart = overlappedInput[i].OffsetHigh;
    currentPositionOutput.QuadPart = currentPositionInput.QuadPart;

    // Пишем с того же места в копию файла
    overlappedOutput[i].Offset = currentPositionOutput.LowPart;
    overlappedOutput[i].OffsetHigh = currentPositionOutput.HighPart;

    // Записываем в копию файла и вызываем процедуру завершения записи
    WriteFileEx(destinationFileHandle, buffersArray[i], blockSize, &overlappedOutput[i], onWriteDone);

    // Подготавливаем структуру OVERLAPPED для следующего чтения
    currentPositionInput.QuadPart += blockSize * (LONGLONG)(numberOfOverlappingOperations);
    overlappedInput[i].Offset = currentPositionInput.LowPart;
    overlappedInput[i].OffsetHigh = currentPositionInput.HighPart;
}

VOID WINAPI onWriteDone(DWORD Code, DWORD nBytes, LPOVERLAPPED lpOv){
    // Запись закончилась, иницииурем чтение
    doneCount++;

    LARGE_INTEGER currentPositionInput;

    // Получаем индекс текущей перекрывающейся операции
    auto i = (DWORD)(lpOv->hEvent);

    // Продолжаем чтение в текущей операции с новой позиции
    currentPositionInput.LowPart = overlappedInput[i].Offset;
    currentPositionInput.HighPart = overlappedInput[i].OffsetHigh;

    // Если в файле еще есть что читать, читаем и запускаем процедуру завершения чтения
    if (currentPositionInput.QuadPart < fileSize.QuadPart){
        ReadFileEx(originalFileHandle, buffersArray[i], blockSize, &overlappedInput[i], onReadDone);
    }
}
#include "DrivesFoldersFiles.h"

void listDrives(){

    // GetLogicalDrives() - get logical drives as bitmask, e.g. 0101 is disk A and C

    // Get bitmask of available drives
    std::bitset<26> bs =  GetLogicalDrives();

    std::cout << "Result of GetLogicalDrives()):" << std::endl;
    std::cout << bs << std::endl;

    // GetLogicalDriveStrings() - get logical drives strings, e.g. c:\<null>d:\<null><null> is disk C and disk D
    char availableDrivesStrings[MAX_PATH] = {0};
    DWORD resultSize = GetLogicalDriveStrings(MAX_PATH, availableDrivesStrings);

    std::cout << "Result of GetLogicalDriveStrings()" << std::endl;
    if (resultSize > 0 && resultSize <= MAX_PATH) {
        // Get pointer to first drive name
        char *singleDrive = availableDrivesStrings;

        while(*singleDrive) {
            // Print current drive
            std::cout << singleDrive << std::endl;
            // Move to next drive
            singleDrive += strlen(singleDrive) + 1;
        }
        std::cout << std::endl;
    }
}

void showDiskInfo(){
    std::cout << "\nEnter disk letter (A-Z):" << std::endl;

    char selectedDisk;
    std::cin >> selectedDisk;

    while (!('A' <= selectedDisk && selectedDisk <= 'Z')) {
        std::cout << "Disk letter is invalid. Please try again" << std::endl;
        std::cin >> selectedDisk;
    }

    // Make drive path from selectedDisk
    std::string selectedDrivePath = std::string(1, selectedDisk) + ":\\";
    LPCSTR drivePath = selectedDrivePath.c_str();

    // Perform GetDriveType()
    UINT drive_type = GetDriveType(drivePath);

    std::cout << "\nResult of GetDriveType():" << std::endl;
    std::cout << "Drive type: " << drive_type << " " <<  DRIVES_TYPE[drive_type] << std::endl;

    // Prepare variables to perform GetVolumeInformation()
    char driveName[MAX_PATH] = {0};
    char FSName[MAX_PATH] = {0};
    DWORD driveSerial = 0;
    DWORD FSMaxLen = 0;
    DWORD FSFlag = 0;

    // Perform GetVolumeInformation()
    std::cout << "\nResult of GetVolumeInformation():" << std::endl;
    BOOL isSuccess = GetVolumeInformation(
            drivePath,
            driveName,
            sizeof(driveName),
            &driveSerial,
            &FSMaxLen,
            &FSFlag,
            FSName,
            sizeof(FSName)
    );

    // Show result
    if (isSuccess) {
        std::cout << "Drive name: " << driveName << std::endl;
        std::cout << "Drive FS: " << FSName << std::endl;
        std::cout << "Volume serial: " << driveSerial << std::endl;
        std::cout << "FS flag: " << FSFlag << std::endl << std::endl;

        // Show flags
        std::cout << "Flags:" << std::endl;
        /* e.g. FSFlag is 65472255 and FSFlag is 1 (FILE_CASE_SENSITIVE_SEARCH)
         * 65472255 -> 11111001110000011011111111
         * 1        ->                          1
         * Bitwise conjunction gives 1
         * It means FS supports case-sensitive file names.
         * */

        for (auto flag: FS_FLAGS) {
            if (flag.first & FSFlag) {
                std::cout << "Supports " << flag.second << std::endl;
            }
        }
    }

    std::cout << "\nResult of GetDiskFreeSpace():" << std::endl;

    // Prepare variables for GetDiskFreeSpace()
    DWORD sectorsPerCluster, bytesPerSector, freeClusters, totalClusters;
    // Perform GetDiskFreeSpace()
    GetDiskFreeSpace(
            drivePath,
            &sectorsPerCluster,
            &bytesPerSector,
            &freeClusters,
            &totalClusters
    );

    // Count free and total space
    DWORDLONG free = freeClusters * sectorsPerCluster / 1024 * bytesPerSector / 1024;
    DWORDLONG total = totalClusters * sectorsPerCluster / 1024 * bytesPerSector / 1024;

    // Show result
    std::cout << "free space: " << free << " / " << total << " MB" << std::endl;
}

void createDirectory(){
    std::cout << "Enter directory name" << std::endl;
    std::string directoryName;
    std::cin >> directoryName;

     if (CreateDirectory(directoryName.c_str(), nullptr)){
         std::cout << "Directory created successfully" << std::endl;
     } else {
         std::cout << "Can not create directory" << std::endl;
     }
}

void removeDirectory(){
    std::cout << "Enter directory name" << std::endl;
    std::string directoryName;
    std::cin >> directoryName;
    if (RemoveDirectory(directoryName.c_str())){
        std::cout << "Directory deleted successfully" << std::endl;
    } else {
        std::cout << "Can not delete directory" << std::endl;

    }
}

void createFile(){
    std::cout << "Enter file name" << std::endl;
    std::string fileName;
    std::cin >> fileName;
    /*
     * GENERIC_READ - open file for write
     * 0 (dwSharedMode) - Prevents other processes from opening a file or device if they request delete, read, or write access
     * nullptr - Security attributes
     * CREATE_ALWAYS (dwCreationDisposition) - Creates a new file, always
     * FILE_ATTRIBUTE_NORMAL (dwFlagsAndAttributes) -  the most common default value for files
     * nullptr (hTemplateFile) - The template file supplies file attributes and extended attributes for the file that is being created
     * */

    HANDLE fileCreateHandle = CreateFile(
            fileName.c_str(),
            GENERIC_READ,
            0,
            nullptr,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);
    // Check file creation status
    if (fileCreateHandle != INVALID_HANDLE_VALUE){
        std::cout << "File created successfully" << std::endl;
    } else {
        std::cout << "Can not create file" << std::endl;
    }

    // Close file handle
    CloseHandle(fileCreateHandle);
}

void copyFile(){
    std::cout << "Enter file name" << std::endl;
    std::string fileName;
    std::cin >> fileName;

    std::cout << "Enter file name of copy" << std::endl;
    std::string copyFileName;
    std::cin >> copyFileName;

    // TRUE (bFailIfExists) - If this parameter is TRUE and the new file specified by lpNewFileName already exists, the function fails.
    // If this parameter is FALSE and the new file already exists, the function overwrites the existing file and succeeds.
    BOOL isSuccess = CopyFile(fileName.c_str(), copyFileName.c_str(), TRUE);

    // Check file copying status
    if (isSuccess){
        std::cout << "Copy of file created successfully" << std::endl;
    } else {
        std::cout << "Can not create copy of file" << std::endl;
    }
}

void moveFile(){
    std::cout << "Enter file name" << std::endl;
    std::string fileName;
    std::cin >> fileName;

    std::cout << "Enter new file name" << std::endl;
    std::string newFileName;
    std::cin >> newFileName;

    BOOL isSuccess = MoveFile(fileName.c_str(), newFileName.c_str());

    if (isSuccess){
        std::cout << "File moved successfully" << std::endl;
    } else {
        std::cout << "Can not move file" << std:: endl;
    }

}

void getFileInfo(){
    std::string fileName;
    std::cout << "Enter file name" << std::endl;
    std::cin >> fileName;

    // Get file attributes
    DWORD fileAttributes = GetFileAttributes(fileName.c_str());

    // Show attributes if GetFileAttributes is successful
    if (fileAttributes != INVALID_FILE_ATTRIBUTES){
        std::cout << "File attributes 0x" << std::hex << fileAttributes << std::dec << std::endl;

        // Show description of file attributes
        std::cout << "File attributes description:" << std::endl;
        for (auto atrribute: FILE_ATTRIBUTES) {
            if (atrribute.first & fileAttributes) {
                std::cout << "This is " << atrribute.second << std::endl;
            }
        }
        std::cout << std::endl;
    } else {
        std::cout << "Can not get file attributes" << std::endl;
    }

    // Get file handle
    // "r" - Open file for input operations. The file must exist.
    FILE *filePointer = fopen(fileName.c_str(), "r");
    /*
     * _get_osfhandle - Returns an operating-system file handle.
     * fileno - Gets the file descriptor associated with a stream.
     *
     * */
    HANDLE fileHandle = (HANDLE)(_get_osfhandle(fileno(filePointer)));
    if (fileHandle == INVALID_HANDLE_VALUE){
        std::cout << "Can not get file handle" << std::endl;
        fclose(filePointer);
    } else {
        // Get file information by handle
        BY_HANDLE_FILE_INFORMATION fileInformation;
        if (GetFileInformationByHandle(fileHandle, &fileInformation)){
            std::cout << "Volume serial number: " << fileInformation.dwVolumeSerialNumber << std::endl;
            std::cout << "Number of links: " << fileInformation.nNumberOfLinks << std::endl;
            std::cout << std::endl;
        }

        // Get file time
        FILETIME fileCreationTime, fileLastAccessTime, fileLastWriteTime;
        SYSTEMTIME timeUTC, timeLocal;

        if (GetFileTime(fileHandle, &fileCreationTime, &fileLastAccessTime, &fileLastWriteTime)){
            // handle creation time
            FileTimeToSystemTime(&fileCreationTime, &timeUTC);
            /*
             * Convert UTC to local time
             * NULL - function uses the currently active time zone.
             * */
            SystemTimeToTzSpecificLocalTime(nullptr, &timeUTC, &timeLocal);

            std::cout << "Creation time: ";
            std::cout << timeLocal.wMonth << "/" << timeLocal.wDay << "/" << timeLocal.wYear << " " << timeLocal.wHour << ":" << timeLocal.wMinute << std::endl;

            // Handle last access time
            FileTimeToSystemTime(&fileLastAccessTime, &timeUTC);
            SystemTimeToTzSpecificLocalTime(nullptr, &timeUTC, &timeLocal);

            std::cout << "Last access time: ";
            std::cout << timeLocal.wMonth << "/" << timeLocal.wDay << "/" << timeLocal.wYear << " " << timeLocal.wHour << ":" << timeLocal.wMinute << std::endl;

            // Handle last write time
            FileTimeToSystemTime(&fileLastWriteTime, &timeUTC);
            SystemTimeToTzSpecificLocalTime(nullptr, &timeUTC, &timeLocal);

            std::cout << "Last write time: ";
            std::cout << timeLocal.wMonth << "/" << timeLocal.wDay << "/" << timeLocal.wYear << " " << timeLocal.wHour << ":" << timeLocal.wMinute << std::endl;
            fclose(filePointer);
        } else {
            std::cout << "Can not get file time" << std::endl;
            fclose(filePointer);
        }
    }
}

void setFileAttributes(){
    std::string fileName;
    std::cout << "Enter file name" << std::endl;
    std::cin >> fileName;

    DWORD newAttributes = 0;
    char answer;

    // Use bitwise 'OR' and 'AND' to get new attributes
    std::cout << "Set normal attribute? (y/n)" << std::endl;
    std::cin >> answer;
    if (answer == 'y'){
        newAttributes |= FILE_ATTRIBUTE_NORMAL;
    } else {
        std::cout << "Set archive attribute? (y/n)" << std::endl;
        std::cin >> answer;
        if (answer == 'y'){
            newAttributes |= FILE_ATTRIBUTE_ARCHIVE;
        } else {
            newAttributes &= ~FILE_ATTRIBUTE_ARCHIVE;
        }

        std::cout << "Set hidden attribute? (y/n)" << std::endl;
        std::cin >> answer;
        if (answer == 'y'){
            newAttributes |= FILE_ATTRIBUTE_HIDDEN;
        } else {
            newAttributes &= ~FILE_ATTRIBUTE_HIDDEN;
        }

        std::cout << "Set not content indexed attribute? (y/n)" << std::endl;
        std::cin >> answer;
        if (answer == 'y'){
            newAttributes |= FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
        } else {
            newAttributes &= ~FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
        }

        std::cout << "Set offline attribute? (y/n)" << std::endl;
        std::cin >> answer;
        if (answer == 'y'){
            newAttributes |= FILE_ATTRIBUTE_OFFLINE;
        } else {
            newAttributes &= ~FILE_ATTRIBUTE_OFFLINE;
        }

        std::cout << "Set readonly attribute? (y/n)" << std::endl;
        std::cin >> answer;
        if (answer == 'y'){
            newAttributes |= FILE_ATTRIBUTE_READONLY;
        } else {
            newAttributes &= ~FILE_ATTRIBUTE_READONLY;
        }

        std::cout << "Set system attribute? (y/n)" << std::endl;
        std::cin >> answer;
        if (answer == 'y'){
            newAttributes |= FILE_ATTRIBUTE_SYSTEM;
        } else {
            newAttributes &= ~FILE_ATTRIBUTE_SYSTEM;
        }

        std::cout << "Set temporary attribute? (y/n)" << std::endl;
        std::cin >> answer;
        if (answer == 'y'){
            newAttributes |= FILE_ATTRIBUTE_TEMPORARY;
        } else {
            newAttributes &= ~FILE_ATTRIBUTE_TEMPORARY;
        }
    }

    // Set new attributes
    if (SetFileAttributes(fileName.c_str(), newAttributes)){
        std::cout << "File attributes set successfully" << std::endl;
    } else {
        std::cout << "Can not set file attributes" << std::endl;
    }
}

void setFileTime(){
    std::string fileName;
    std::cout << "Enter file name" << std::endl;
    std::cin >> fileName;

    // Get file handle
    HANDLE fileHandle = CreateFile(fileName.c_str(), GENERIC_WRITE , 0, nullptr, OPEN_EXISTING, 0, nullptr);

    // Check handle
    if (fileHandle != INVALID_HANDLE_VALUE){
        // Variables to get system time and file time
        SYSTEMTIME systemCurrentTime;
        FILETIME fileTime;

        // Get current system time and convert to file time
        GetSystemTime(&systemCurrentTime);
        SystemTimeToFileTime(&systemCurrentTime, &fileTime);

        // Set file time
        // fileHandle, creationTime, lastAccessTime, lastWriteTime
        if (SetFileTime(fileHandle, &fileTime, nullptr, nullptr)){
            std::cout << "New file creation time set successfully" << std::endl;
        } else {
            std::cout << "Can not set file creation time" << std::endl;
        }
        CloseHandle(fileHandle);
    } else {
        std::cout << "Can not open file";
    }
}
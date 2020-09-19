#include "DrivesFoldersFiles.h"

void listDrives(){

    // GetLogicalDrives() - get logical drives as bitmask, e.g. 0101 is disk A and C

    // Get bitmask of available drives
    std::bitset<26> bs =  GetLogicalDrives();

    std::cout << "Result of GetLogicalDrives()):\n";
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
            std::cout << singleDrive << "\n";
            // Move to next drive
            singleDrive += strlen(singleDrive) + 1;
        }
        std::cout << std::endl;
    }
}

void showDiskInfo(){
    std::cout << "\nEnter disk letter (A-Z):\n";

    char selectedDisk;
    std::cin >> selectedDisk;

    while (!('A' <= selectedDisk && selectedDisk <= 'Z')) {
        std::cout << "Disk letter is invalid. Please try again\n";
        std::cin >> selectedDisk;
    }

    // Make drive path from selectedDisk
    std::string selectedDrivePath = std::string(1, selectedDisk) + ":\\";
    LPCSTR drivePath = selectedDrivePath.c_str();

    // Perform GetDriveType()
    UINT drive_type = GetDriveType(drivePath);

    std::cout << "\nResult of GetDriveType():\n";
    std::cout << "Drive type: " << drive_type << " " <<  DRIVES_TYPE[drive_type] << "\n";

    // Prepare variables to perform GetVolumeInformation()
    char driveName[MAX_PATH] = {0};
    char FSName[MAX_PATH] = {0};
    DWORD driveSerial = 0;
    DWORD FSMaxLen = 0;
    DWORD FSFlag = 0;

    // Perform GetVolumeInformation()
    std::cout << "\nResult of GetVolumeInformation():\n";
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
        std::cout << "Drive name: " << driveName << "\n";
        std::cout << "Drive FS: " << FSName << "\n";
        std::cout << "Volume serial: " << driveSerial << "\n";
        std::cout << "FS flag: " << FSFlag << "\n\n";

        // Show flags
        std::cout << "Flags:\n";
        /* e.g. FSFlag is 65472255 and FSFlag is 1 (FILE_CASE_SENSITIVE_SEARCH)
         * 65472255 -> 11111001110000011011111111
         * 1        ->                          1
         * It means FS supports case-sensitive file names.
         * */

        for (auto flag: FS_FLAGS) {
            if (flag.first & FSFlag) {
                std::cout << "Supports " << flag.second << "\n";
            }
        }
    }

    std::cout << "\nResult of GetDiskFreeSpace():\n";

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
    std::cout << "free space: " << free << " / " << total << " MB\n";
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
    // GENERIC_READ - open file for write
    // 0 (dwSharedMode) - Prevents other processes from opening a file or device if they request delete, read, or write access
    // nullptr - Security attributes
    // CREATE_ALWAYS (dwCreationDisposition) - Creates a new file, always
    // FILE_ATTRIBUTE_NORMAL (dwFlagsAndAttributes) -  the most common default value for files
    // nullptr (hTemplateFile) - The template file supplies file attributes and extended attributes for the file that is being created
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

}
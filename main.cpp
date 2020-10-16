#include <iostream>
#include <sstream>
#include "DrivesFoldersFiles.h"
#include "copyFileAsync.h"

void menuPrint();
int menuSelect(int smallestItemNumber, int biggestItemNumber);

// Array of pointers to functions
void (*functionsList[])() = {
        listDrives,
        showDiskInfo,
        createDirectory,
        removeDirectory,
        createFile,
        copyFile,
        moveFile,
        getFileInfo,
        setFileAttributes,
        setFileTime,
        copyFileAsync
};

int main(){
    int smallestItem = 0, biggestItem = 11;
    menuPrint();
    int val = menuSelect(smallestItem , biggestItem);

    while(val != 0){
        if (val != -1){
            (*functionsList[val-1])();
        }

        // Clearing input to avoid input buffering
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        menuPrint();
        val = menuSelect(smallestItem , biggestItem);
    }
    return 0;
}

// Print menu items
void menuPrint(){
    std::cout << "\n --- Main menu --- \n";
    std::cout << "1. Retrieve the names of the logical drives (GetLogicalDrives)\n";
    std::cout << "2. Retrieve information about drive (GetDriveType, GetVolumeInformation, GetDiskFreeSpace)\n";
    std::cout << "3. Create directory (CreateDirectory)\n";
    std::cout << "4. Remove directory (RemoveDirectory)\n";
    std::cout << "5. Create file (CreateFile)\n";
    std::cout << "6. Copy file (CopyFile)\n";
    std::cout << "7. Move file (MoveFile)\n";
    std::cout << "8. Retrieve file info (GetFileAttributes, GetFileInformationByHandle, GetFileTime)\n";
    std::cout << "9. Set file attributes (SetFileAttributes)\n";
    std::cout << "10. Set file attributes (SetFileTime)\n";
    std::cout << "11. Copy file async\n";
    std::cout << "0. Exit";
    std::cout << "\n";
}

// Select menu item
int menuSelect(int smallestItemNumber, int biggestItemNumber){
    std::string selectedItemString;
    std::cout << "Enter selected item:\n";
    std::cin >> selectedItemString;

    // Try to get integer number from entered string
    std::stringstream stringStream;
    stringStream << selectedItemString;

    int selectedItem;
    stringStream >> selectedItem;

    if (stringStream.fail()){
        // Entered string is incorrect
        std::cout << "Entered string is incorrect" << std::endl;
        std::cout << "Try again" << std::endl;
        selectedItem = -1;
    } else if ((selectedItem < smallestItemNumber) || (selectedItem > biggestItemNumber)) {
        // Entered string is correct, but number is out of range [smallestItemNumber; biggestItemNumber]
        std::cout << "There is no item with number " << selectedItem << std::endl;
        std::cout << "Try again" << std::endl;
        selectedItem = -1;
    }
    return selectedItem;
}


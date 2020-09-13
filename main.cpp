#include <iostream>
#include <fileapi.h>
#include <bitset>

void menuPrint();
int menuSelect();
void getLogicalDrives();


int main() {
    menuPrint();
    int val = menuSelect();
    while(val != 0){
        switch (val) {
            case 1:
                getLogicalDrives();
                std::cout << "\nPress enter to return";
                std::cin.get();
                std::cin.get();
                break;

        }
        menuPrint();
        val = menuSelect();
    }


    std::cin.get();
    return 0;
}

// Print menu items
void menuPrint(){
    std::cout << "\n --- Main menu --- \n";
    std::cout << "1. Retrieve the names of the logical drives (Directory.GetLogicalDrives)";
    std::cout << "\n0. Exit";
    std::cout << "\n";
}

// Select menu item
int menuSelect(){
    // TODO Input check (str to int convert)

    int selectedItem = 0;
    std::cout << "Enter selected item:\n";
    std::cin >> selectedItem;
    return selectedItem;
}


void getLogicalDrives(){
    // Get logical drives as bitmask, e.g. 0101 is disk A and C
    std::bitset<26> bs =  GetLogicalDrives();
    std::cout << "List of available drives:\n";
    for (int i = 0; i < 26; i++) {
        if (bs[i-1] == 1){
            std::cout << char(65+i-1) << ":\\" << std::endl;
        }
    }
}
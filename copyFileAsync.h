#ifndef INC_8307_ERSHOV_OS_LAB1_COPYFILEASYNC_H
#define INC_8307_ERSHOV_OS_LAB1_COPYFILEASYNC_H


#include <windows.h>
#include <iostream>
#include <string>

void copyFileAsync();
VOID WINAPI onReadDone(DWORD Code, DWORD nBytes, LPOVERLAPPED lpOv);
VOID WINAPI onWriteDone(DWORD Code, DWORD nBytes, LPOVERLAPPED lpOv);

#endif //INC_8307_ERSHOV_OS_LAB1_COPYFILEASYNC_H

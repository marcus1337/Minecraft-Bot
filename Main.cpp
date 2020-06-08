#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <string>
#include "Main.h"

using namespace std;

DWORD getCurrentProcess() {
    DWORD tmpPID = 0;
    TCHAR wnd_title[256];
    HWND hwnd = GetForegroundWindow();
    GetWindowTextA(hwnd, wnd_title, 256);
    HWND hand2 = FindWindow(0, wnd_title);
    GetWindowThreadProcessId(hand2, &tmpPID);
    HANDLE pHandle2 = OpenProcess(PROCESS_ALL_ACCESS, FALSE, tmpPID);
    return tmpPID;
}

DWORD getTargetProcess(std::string windowName) {
    DWORD tmpPID = 0;
    HWND hand = FindWindow(0, windowName.c_str());
    if (!hand) {
        cout << "ERROR \n";
        exit(EXIT_FAILURE);
    }
    GetWindowThreadProcessId(hand, &tmpPID);
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, tmpPID);
    if (!pHandle) {
        cout << "ERROR 2";
        exit(EXIT_FAILURE);
    }
    return tmpPID;
}


void runBot()
{
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = MapVirtualKey(0x41, MAPVK_VK_TO_VSC);
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = KEYEVENTF_SCANCODE;

    ip.ki.wVk = 0x41;
    ip.ki.dwFlags = 0;
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
    ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));
}

bool isCorrectProcess(DWORD targetProcessID) {
    return targetProcessID == getCurrentProcess();
}

int main() {

    DWORD pID = getTargetProcess("Minecraft 1.15.2 - Singleplayer");

    while (true) { //send array of INPUT structs to combine key presses

        if (isCorrectProcess(pID))
            runBot();

        Sleep(10);
    }

    return 0;
}
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <string>
#include "Main.h"
#include "Timer.h"
#include <vector>

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

INPUT getKeyDownInput(int unicode) {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = MapVirtualKey(unicode, MAPVK_VK_TO_VSC);
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = KEYEVENTF_SCANCODE;
    ip.ki.wVk = unicode;
    ip.ki.dwFlags = 0;
    return ip;
}

INPUT getKeyUpInput(int unicode) {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = MapVirtualKey(unicode, MAPVK_VK_TO_VSC);
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
    ip.ki.wVk = unicode;
    ip.ki.dwFlags = 0;
    return ip;
}

void sendPressedKeyInputs(std::vector<INPUT> inputs) {
    SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
}

void sendLiftedKeyInputs(std::vector<INPUT> inputs) {
    for (auto& ip : inputs)
        ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
    SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
}

void simulateKeys(std::vector<INPUT> &inputs)
{
    sendPressedKeyInputs(inputs);
    Sleep(50);
    sendLiftedKeyInputs(inputs);
}

void opengMCTerminal() {
    Sleep(200);
    std::vector<INPUT> _keys;
    _keys.push_back(getKeyDownInput('T'));
    simulateKeys(_keys);
    Sleep(200);
}

void pressEnter() {
    Sleep(200);
    std::vector<INPUT> _keys;
    _keys.push_back(getKeyDownInput(VK_RETURN));
    simulateKeys(_keys);
    Sleep(200);
}

void pressWithShift(std::vector<char> _keys) {
    std::vector<INPUT> result;
    result.push_back(getKeyDownInput(VK_SHIFT));
    for (auto _key : _keys)
        result.push_back(getKeyDownInput(_key));
    Sleep(50);
    simulateKeys(result);
    Sleep(50);
}

std::vector<INPUT> getKeyInputs() {
    std::vector<INPUT> result;

    result.push_back(getKeyDownInput('H'));
    result.push_back(getKeyDownInput('O'));
    result.push_back(getKeyDownInput('M'));
    result.push_back(getKeyDownInput('E'));

    return result;
}

void runBot()
{
    opengMCTerminal();
    pressWithShift({ '7' });

    auto inputs = getKeyInputs();
    simulateKeys(inputs);

    pressEnter();
}

bool isCorrectProcess(DWORD targetProcessID) {
    return targetProcessID == getCurrentProcess();
}

bool paused = false;

bool isPaused() {
    return paused;
}

void togglePause() {
    if (GetKeyState('P') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
    {
        paused = !paused;
    }
}

int main() {

    DWORD pID = getTargetProcess("Minecraft 1.15.2 - Singleplayer");
    paused = true;

    Timer timer;
    timer.startClock();

    while (true) {

        togglePause();

        if (isCorrectProcess(pID) && !isPaused())
            runBot();

        Sleep(60);
    }

    return 0;
}
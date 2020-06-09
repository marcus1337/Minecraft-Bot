
#include "MCBot.h"

using namespace std;

MCBot::MCBot() {
    init("Minecraft 1.15.2 - Singleplayer");
}

MCBot::MCBot(std::string _targetProcessName) {
    init(_targetProcessName);
}

void MCBot::init(std::string _targetProcessName)
{
    shouldThreadRun = true;
    targetProcessName = _targetProcessName;
    targetProcess = getTargetProcess(targetProcessName);
    paused = true;
    _thread = std::thread(&MCBot::pollCommands, this);
}

void MCBot::pollCommands() {
    while (shouldThreadRun) {
        cout << "hello world 123\n";
        Sleep(500);
    }
}

MCBot::~MCBot() {
    shouldThreadRun = false;
    _thread.join();
}

DWORD MCBot::getCurrentProcess() {
    DWORD tmpPID = 0;
    TCHAR wnd_title[256];
    HWND hwnd = GetForegroundWindow();
    GetWindowTextA(hwnd, wnd_title, 256);
    HWND hand2 = FindWindow(0, wnd_title);
    GetWindowThreadProcessId(hand2, &tmpPID);
    HANDLE pHandle2 = OpenProcess(PROCESS_ALL_ACCESS, FALSE, tmpPID);
    return tmpPID;
}

DWORD MCBot::getTargetProcess(std::string windowName) {
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

INPUT MCBot::getKeyDownInput(int unicode) {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = MapVirtualKey(unicode, MAPVK_VK_TO_VSC);
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = KEYEVENTF_SCANCODE;
    ip.ki.wVk = unicode;
    ip.ki.dwFlags = 0;
    return ip;
}

INPUT MCBot::getKeyUpInput(int unicode) {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = MapVirtualKey(unicode, MAPVK_VK_TO_VSC);
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
    ip.ki.wVk = unicode;
    ip.ki.dwFlags = 0;
    return ip;
}

void MCBot::sendPressedKeyInputs(std::vector<INPUT> inputs) {
    SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
}

void MCBot::sendLiftedKeyInputs(std::vector<INPUT> inputs) {
    for (auto& ip : inputs)
        ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
    SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
}

void MCBot::simulateKeys(std::vector<INPUT> &inputs)
{
    sendPressedKeyInputs(inputs);
    Sleep(50);
    sendLiftedKeyInputs(inputs);
}

void MCBot::opengMCTerminal() {
    Sleep(200);
    std::vector<INPUT> _keys;
    _keys.push_back(getKeyDownInput('T'));
    simulateKeys(_keys);
    Sleep(200);
}

void MCBot::pressEnter() {
    Sleep(200);
    std::vector<INPUT> _keys;
    _keys.push_back(getKeyDownInput(VK_RETURN));
    simulateKeys(_keys);
    Sleep(200);
}

void MCBot::pressWithShift(std::vector<char> _keys) {
    std::vector<INPUT> result;
    result.push_back(getKeyDownInput(VK_SHIFT));
    for (auto _key : _keys)
        result.push_back(getKeyDownInput(_key));
    Sleep(50);
    simulateKeys(result);
    Sleep(50);
}

std::vector<INPUT> MCBot::getKeyInputs(std::vector<char> _keys) {
    std::vector<INPUT> result;
    for (auto _key : _keys) {
        result.push_back(getKeyDownInput(_key));
    }
    return result;
}

void MCBot::teleportHomeCommand() {
    opengMCTerminal();
    pressWithShift({ '7' });
    simulateKeys(getKeyInputs({ 'H','O','M','E' }));
    pressEnter();
}

void MCBot::mouseLeftClick() {
    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.dx = 0;
    input.mi.dy = 0;
    input.mi.dwFlags = ( MOUSEEVENTF_LEFTDOWN);
    input.mi.mouseData = 0;
    input.mi.dwExtraInfo = NULL;
    input.mi.time = 0;
    SendInput(1, &input, sizeof(INPUT));

    Sleep(50);

   /* input.type = INPUT_MOUSE;
    input.mi.dx = 0;
    input.mi.dy = 0;
    input.mi.dwFlags = (MOUSEEVENTF_LEFTUP);
    input.mi.mouseData = 0;
    input.mi.dwExtraInfo = NULL;
    input.mi.time = 0;
    SendInput(1, &input, sizeof(INPUT));*/

}

void MCBot::moveMouseRight() {
    SetProcessDPIAware();
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    POINT p;
    GetCursorPos(&p);

    p.x = screenWidth / 2;
    p.y = screenHeight / 2;
    INPUT in[1] = { 0 };
    in[0].type = INPUT_MOUSE;
    in[0].mi.dx = 10;
    in[0].mi.dy = 0;
    in[0].mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, in, sizeof(INPUT));
    GetCursorPos(&p);
}

void MCBot::mouseRightClick() {

}

void MCBot::moveForward() {
    simulateKeys(getKeyInputs({ 'W' }));
}

void MCBot::runBot()
{
    moveForward();
    mouseLeftClick();

    if (timer.getSeconds() > 2.7f * 60) {
        simulateKeys(getKeyInputs({ (char)('1'+ slot) }));
        slot++;
        timer.startClock();
    }

}

bool MCBot::isCorrectProcess(DWORD _targetProcessID) {
    return _targetProcessID == getCurrentProcess();
}

bool MCBot::isPaused() {
    return paused;
}

void MCBot::togglePause() {
    if (GetKeyState('P') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
    {
        paused = !paused;
        timer.startClock();
    }
}

bool MCBot::shouldRun() {
    return isCorrectProcess(targetProcess) && !isPaused();
}
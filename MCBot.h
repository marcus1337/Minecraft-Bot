#pragma once
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <string>
#include "Timer.h"
#include <vector>
#include <string>

class MCBot {
    Timer timer;
    DWORD targetProcess;
    bool paused = false;
    int slot = 1;
    std::string targetProcessName;
public:
    MCBot();
    MCBot(std::string _targetProcessName);

    DWORD getCurrentProcess();
    DWORD getTargetProcess(std::string windowName);
    INPUT getKeyDownInput(int unicode);
    INPUT getKeyUpInput(int unicode);
    std::vector<INPUT> getKeyInputs(std::vector<char> _keys);

    void sendPressedKeyInputs(std::vector<INPUT> inputs);
    void sendLiftedKeyInputs(std::vector<INPUT> inputs);
    void simulateKeys(std::vector<INPUT> &inputs);
    void opengMCTerminal();
    void pressEnter();
    void pressWithShift(std::vector<char> _keys);

    void teleportHomeCommand();
    void runBot();
    bool isCorrectProcess(DWORD _targetProcessID);
    bool isPaused();
    void togglePause();

    bool shouldRun();

    void mouseRightClick();
    void mouseLeftClick();
    void moveMouseRight();
    void moveForward();
};
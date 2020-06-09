
#include "MCBot.h"

using namespace std;


int main() {

    MCBot mcBot("Minecraft 1.15.2 - Multiplayer (3rd-party)");

    while (true) {

        if (mcBot.shouldRun())
            mcBot.runBot();

        Sleep(60);
    }

    return 0;
}
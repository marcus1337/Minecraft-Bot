
#include "MCBot.h"

using namespace std;


int main() {

    MCBot mcBot("hej");

    while (true) {

        mcBot.togglePause();

        if (mcBot.shouldRun())
            mcBot.runBot();

        Sleep(60);
    }

    return 0;
}
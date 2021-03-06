#include "common_console.hpp"
#include "console.hpp"

#include <stdio.h>

bool shouldQuit = false;

void setQuit(Console *console, std::vector<std::string> args) { shouldQuit = true; }

//
// Similar to 'seta' from Quake, it creates / edits a variable using the value following it
//

void setv(Console *console, std::vector<std::string> args) {
    if (args.size() >= 2) {
        ConVar *var = console->GetCVar(args[0], true);

        if (var)
            var->Execute(console, std::vector<std::string>{args[1]});
        else
            console->CreateCVar(args[0], args[1]);
    }
}

void help(Console *console, std::vector<std::string> args) {
    if (args.size() > 0) {
        auto iter = console->objects.find(args[0]);

        if (iter != console->objects.end()) {
            printf("%s: %s\n", iter->first.c_str(), iter->second->help.c_str());
        }
    } else {
        printf("Proper syntax is help [COMMAND_NAME_HERE], ex: help help, would print the help string for this command\n");
    }
}

//
// Executes all commands from a given file
//

void exec(Console *console, std::vector<std::string> args) {
    if (args.size() >= 1)
        console->ParseExecFile(args[0]);
}

void disableProtection(Console *console, std::vector<std::string> args) {
    console->protectionDisabled = true;
    printf("Console protection is now disabled, risky / cheat CFuncs and CVars can now be executed / set!\n");
}

void CreateCommonConObjects(Console *console) {
    if (!console) {
        printf("Could not create common ConObjects, the given console points to nullptr!\n");
        return;
    }

    //
    // Common cvars
    //

    console->CreateCVar("gamename", "Manta Game");
    console->CreateCVar("fov", "90"); // Can be ignored by cameras


    // Redundant until further notice... console->CreateCVar("e_autoexec_path", "./data/autoexec");

    //
    // Common cfuncs
    //

    console->CreateCFunc("quit", &setQuit);
    console->CreateCFunc("setv", &setv);
    console->CreateCFunc("exec", &exec);
    console->CreateCFunc("e_disable_protection", &disableProtection);
}

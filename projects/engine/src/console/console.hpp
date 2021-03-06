#ifndef MANTA_CONSOLE_H
#define MANTA_CONSOLE_H

#include <unordered_map>
#include <string>
#include <vector>
#include <functional>

#include "manta_macros.hpp"

#define CONSOLE_MAX_INPUT_SIZE 255

class Console;

class ConObject {
public:
    ConObject(std::string signature, bool isProtected);

    virtual ~ConObject() {};

    virtual void Execute(Console *console, std::vector<std::string> args) = 0;

    bool isProtected; // Restricts usage / execution of this ConObject
    std::string signature;

    std::string help = ""; // What is outputted when help [command] is ran
};

bool canCreateConObject();

//
// A binding to C/C++ code that allows for invoking it through its name
//
using ConFuncBinding = std::function<void(Console *, std::vector<std::string>)>;

#define BIND_MEMBER_FUNC(MEMBER, INSTANCE) std::bind(MEMBER, INSTANCE, _1, _2)

class ConFunc : public ConObject {
public:
    ConFunc(std::string signature, ConFuncBinding func, bool isProtected);

    void Execute(Console *console, std::vector<std::string> args);

protected:
    ConFuncBinding function = nullptr;
};

//
// A piece of data that can be written and read to by the console and application
//
class ConVar : public ConObject {
public:
    ConVar(std::string signature, std::string data, bool isProtected);

    void Execute(Console *console, std::vector<std::string> args);

    std::string data = "";

    int ParseInt(int default_ = 0);

    void ParseInt(int *data, int default_ = 0);

    bool ParseBool(bool default_ = false);

    void ParseBool(bool *data, bool default_ = false);
};

//
// Structure that holds the console data and does error checking
//
class Console {
public:
    Console();

    ~Console();

    std::unordered_map<std::string, ConObject *> objects;
    bool protectionDisabled = false; // Allows cheat / risky ConObjects to execute

    bool showWindow = false; // For ImGui

    void CreateCFunc(std::string signature, ConFuncBinding func, std::string help = "", bool isProtected = false);

    void
    CreateCVar(const std::string &signature, std::string data, const std::string &help = "", bool isProtected = false);

    bool RegisterObject(ConObject *object);

    void ParseExecFile(std::string path);

    void ParseCommandLine(int argc, char **argv);

    ConVar *GetCVar(std::string signature, bool silent = false);

    int CVarGetInt(std::string signature, int default_);

    bool CVarGetBool(std::string signature, bool default_);

    std::string CVarGetData(std::string signature, std::string default_);

    bool canCreateConObject(std::string signature);

    std::string inputBuffer;

    void DrawImGuiWindow();
};

#endif

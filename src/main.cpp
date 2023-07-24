#include <windows.h>
#include "Game.h"
#include "common/Engine.h"

LONG WINAPI crashHandler(EXCEPTION_POINTERS *exceptionInfo) {
    DWORD exceptionCode = exceptionInfo->ExceptionRecord->ExceptionCode;
    PVOID exceptionAddress = exceptionInfo->ExceptionRecord->ExceptionAddress;

    char buffer[256];
    char codeString[32];
    const char *exceptionMessage;

    switch (exceptionCode) {
        case EXCEPTION_ACCESS_VIOLATION:
            exceptionMessage = "Access violation";
            break;
        case EXCEPTION_ILLEGAL_INSTRUCTION:
            exceptionMessage = "Illegal instruction";
            break;
        case EXCEPTION_STACK_OVERFLOW:
            exceptionMessage = "Stack overflow";
            break;
        default:
            snprintf(codeString, sizeof(codeString), "UNEXPECTED (%lu)", exceptionCode);
            exceptionMessage = codeString;
            break;
    }

    snprintf(buffer, sizeof(buffer), "Exception: %s\nAddress: 0x%p", exceptionMessage, exceptionAddress);
    MessageBoxA(nullptr, buffer, "Crash", MB_ICONERROR);

    exit(1);
}

int main() {
    SetUnhandledExceptionFilter(crashHandler);

    Game game{};

    Engine::Engine::run(game);
    return 0;
}
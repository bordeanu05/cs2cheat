#include "../inc/memory.hpp"
#include <TlHelp32.h>

Memory::Memory(const wchar_t* processName) {
    HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    PROCESSENTRY32W processEntry = {};
    processEntry.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(snapshotHandle, &processEntry)) {
        do {
            if (_wcsicmp(processEntry.szExeFile, processName) == 0) {
                mProcessId = processEntry.th32ProcessID;
                mProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, mProcessId);
                break;
            }
        } while (Process32NextW(snapshotHandle, &processEntry));
    }

    CloseHandle(snapshotHandle);
}

Memory::~Memory() {
    if (mProcessHandle != NULL) {
        CloseHandle(mProcessHandle);
    }
}

DWORD Memory::getProcessId() {
    return mProcessId;
}

HANDLE Memory::getProcessHandle() {
    return mProcessHandle;
}

uintptr_t Memory::getModuleAddress(const wchar_t* moduleName) {
    HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, mProcessId);

    MODULEENTRY32W moduleEntry = {};
    moduleEntry.dwSize = sizeof(MODULEENTRY32W);

    if (Module32FirstW(snapshotHandle, &moduleEntry)) {
        do {
            if (_wcsicmp(moduleEntry.szModule, moduleName) == 0) {
                CloseHandle(snapshotHandle);
                return reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
            }
        } while (Module32NextW(snapshotHandle, &moduleEntry));
    }

    CloseHandle(snapshotHandle);

    return static_cast<uintptr_t>(0);
}
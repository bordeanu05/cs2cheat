#pragma once

#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>

class Memory {
public:
    Memory(const wchar_t* processName);
    ~Memory();

    DWORD getProcessId() const;
    HANDLE getProcessHandle() const;

    uintptr_t getModuleAddress(const wchar_t* moduleName);

    void memReadString(uintptr_t, std::string&);

    template <typename T>
    T memRead(uintptr_t address) {
        T value;
        ReadProcessMemory(mProcessHandle, (LPCVOID)address, &value, sizeof(T), NULL);
        return value;
    }

    template <typename T>
    bool memWrite(uintptr_t address, T value) {
        return WriteProcessMemory(mProcessHandle, (LPCVOID)address, &value, sizeof(T), NULL);
    }

private:
    DWORD mProcessId = 0;
    HANDLE mProcessHandle = NULL;
};

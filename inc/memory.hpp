#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <windows.h>
#include <iostream>

class Memory {
private:
    DWORD mProcessId = 0;
    HANDLE mProcessHandle = NULL;

public:
    Memory(const wchar_t* processName);
    ~Memory();

    DWORD getProcessId();
    HANDLE getProcessHandle();

    uintptr_t getModuleAddress(const wchar_t* moduleName);

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
};

#endif
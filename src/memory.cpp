#include "../inc/memory.hpp"
#include <TlHelp32.h>

Memory::Memory(const wchar_t* processName) {
    bool isProcessOpened = false;

    // Search for cs2.exe and retry if not found
    do {
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

        if (mProcessHandle != NULL) {
            isProcessOpened = true;
            std::cout << "CS2 game process found!" << std::endl;
        }
        else {
            std::cerr << "CS2 is not opened, waiting..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(2500));
        }
    } while (!isProcessOpened);

}

Memory::~Memory() {
    if (mProcessHandle != NULL) {
        CloseHandle(mProcessHandle);
    }
}

DWORD Memory::getProcessId() const {
    return mProcessId;
}

HANDLE Memory::getProcessHandle() const {
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

void Memory::memReadString(uintptr_t address, std::string& out) {
    char buffer[64];
    ReadProcessMemory(mProcessHandle, (LPCVOID)address, buffer, sizeof(buffer), NULL);
    out = buffer;
}

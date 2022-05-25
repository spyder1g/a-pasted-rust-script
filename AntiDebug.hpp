#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <thread>
#include <random>
#include <d3d9.h>
#include <dinput.h>
#include <Mmsystem.h>
#include <mciapi.h>
#include <Mmsystem.h>
#include <TlHelp32.h>
#include <mciapi.h>
#include <strsafe.h> 
#include <windows.h>
#include <string>
#include <stdio.h>
#include <array>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <ctime>
#include <time.h>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>
#include <regex>
#include <tchar.h>
#include <ProcessSnapshot.h>

#include "XorStr.hpp"

void error()
{
    exit(1);
}

DWORD_PTR FindProcessId2(const std::string processName)
{
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE)
        return 0;

    Process32First(processesSnapshot, &processInfo);
    if (!processName.compare(processInfo.szExeFile))
    {
        CloseHandle(processesSnapshot);
        return processInfo.th32ProcessID;
    }

    while (Process32Next(processesSnapshot, &processInfo))
    {
        if (!processName.compare(processInfo.szExeFile))
        {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }
    }

    CloseHandle(processesSnapshot);
    return 0;
}

typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION {
    BOOLEAN DebuggerEnabled;
    BOOLEAN DebuggerNotPresent;
} SYSTEM_KERNEL_DEBUGGER_INFORMATION, * PSYSTEM_KERNEL_DEBUGGER_INFORMATION;

typedef NTSTATUS(NTAPI* pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);

struct protection_struct {

    struct alert_struct {
        void show_debug() {

            system("cmd.exe / C ping 1.1.1.1 - n 1 - w 3000 > Nul & Del / f / q \"%s\"");
            BOOLEAN bEnabled;
            ULONG uResp;
            LPVOID lpFuncAddress = GetProcAddress(LoadLibraryA("ntdll.dll"), "RtlAdjustPrivilege");
            LPVOID lpFuncAddress2 = GetProcAddress(GetModuleHandle("ntdll.dll"), "NtRaiseHardError");
            pdef_RtlAdjustPrivilege NtCall = (pdef_RtlAdjustPrivilege)lpFuncAddress;
            pdef_NtRaiseHardError NtCall2 = (pdef_NtRaiseHardError)lpFuncAddress2;
            NTSTATUS NtRet = NtCall(19, TRUE, FALSE, &bEnabled);
            NtCall2(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, 6, &uResp);
            return;
        }
    } *alert;

    bool initialize() {
        { // anti dump

            antidump->erase_pe();

        }

        return true;
    }

    struct kernel_struct {
        bool query_information() {
            const ULONG_PTR UserSharedData = 0x7FFE0000;

            const UCHAR KdDebuggerEnabledByte = *(UCHAR*)(UserSharedData + 0x2D4);

            const BOOLEAN KdDebuggerEnabled = (KdDebuggerEnabledByte & 0x1) == 0x1;
            const BOOLEAN KdDebuggerNotPresent = (KdDebuggerEnabledByte & 0x2) == 0;

            if (KdDebuggerEnabled || !KdDebuggerNotPresent)
                return TRUE;

            return FALSE;
        }
    } *kernel;

    struct dump_struct {
        void erase_pe() {

            DWORD old;

            char* baseAddress = (char*)GetModuleHandle(NULL);


            VirtualProtect(baseAddress, 4096, PAGE_READWRITE, &old);

            SecureZeroMemory(baseAddress, 4096);

        }
    } *antidump;
};

void driverdetect()
{
    const TCHAR* devices[] = {
_T("\\\\.\\NiGgEr"),
_T("\\\\.\\KsDumper")
    };

    WORD iLength = sizeof(devices) / sizeof(devices[0]);
    for (int i = 0; i < iLength; i++)
    {
        HANDLE hFile = CreateFile(devices[i], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        TCHAR msg[256] = _T("");
        if (hFile != INVALID_HANDLE_VALUE) {
            error();
        }
        else
        {

        }
    }
}
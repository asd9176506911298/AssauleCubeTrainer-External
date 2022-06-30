// External.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include "proc.h"
#include "mem.h"

bool bDelay = false, bAmmo = false, bContinue = false, bRecoil = false, bNoBlack = false;

std::string isOpen(bool status)
{
    return status ? "ON" : "OFF";
}


void printOnOff()
{
    system("cls");
    std::cout << std::setw(40) << std::left << "[NUMPAD1] Shoot not delay" << isOpen(bDelay) << std::endl;
    std::cout << std::setw(40) << std::left << "[NUMPAD2] infinite ammo" << isOpen(bAmmo) << std::endl;
    std::cout << std::setw(40) << std::left << "[NUMPAD3] Continue shoot" << isOpen(bContinue) << std::endl;
    std::cout << std::setw(40) << std::left << "[NUMPAD4] Gun Recoil" << isOpen(bRecoil) << std::endl;
    std::cout << std::setw(40) << std::left << "[NUMPAD5] Rifle not black screen" << isOpen(bNoBlack) << std::endl;
    std::cout << std::setw(40) << std::left << "[END] Exit" << std::right << std::endl;
}


int main()
{
    DWORD procId = GetProcId(L"ac_client.exe");

    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

    HANDLE hProcess = 0;

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);

    uintptr_t BaseAddress = (uintptr_t)moduleBase + 0x10f4f4;

    uintptr_t ammoAddr = FindDMAAdy(hProcess, BaseAddress, { 0x374, 0x14, 0x0 });

    printOnOff();

    while (!GetAsyncKeyState(VK_END) & 1)
    {
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bDelay = !bDelay;
            printOnOff();
            if (bDelay)
            {
                mem::NopEx((BYTE*)0x4637DA, 7, hProcess);
            }
            else
            {
                mem::PatchEx((BYTE*)0x4637DA, (BYTE*)"\x0f\xbf\x88\x0a\x01\x00\x00", 7, hProcess);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bAmmo = !bAmmo;
            printOnOff();
            if (bAmmo)
            {
                mem::PatchEx((BYTE*)0x4637E9, (BYTE*)"\xFF\x06", 2, hProcess);
            }
            else
            {
                mem::PatchEx((BYTE*)0x4637E9, (BYTE*)"\xFF\x0E", 2, hProcess);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            bContinue = !bContinue;
            printOnOff();
            if (bContinue)
            {
                mem::PatchEx((BYTE*)0x463716, (BYTE*)"\xeb\x09", 2, hProcess);
            }
            else
            {
                mem::PatchEx((BYTE*)0x463716, (BYTE*)"\x75\x09", 2, hProcess);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD4) & 1)
        {
            bRecoil = !bRecoil;
            printOnOff();
            if (bRecoil)
            {
                mem::NopEx((BYTE*)0x463786, 10, hProcess);
            }
            else
            {
                mem::PatchEx((BYTE*)0x463786, (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10, hProcess);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD5) & 1)
        {
            bNoBlack = !bNoBlack;
            printOnOff();
            if (bNoBlack)
            {
                mem::PatchEx((BYTE*)0x463CE7, (BYTE*)"\xEB\x0E", 2, hProcess);
            }
            else
            {
                mem::PatchEx((BYTE*)0x463CE7, (BYTE*)"\x74\x0E", 2, hProcess);
            }
        }

        Sleep(10);
    }

    
    return 0;

}

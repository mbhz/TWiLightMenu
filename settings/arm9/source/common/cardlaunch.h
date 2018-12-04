#include <nds.h>

#ifndef __CARD_LAUNCH__
#define __CARD_LAUNCH__
extern const char *unlaunchAutoLoadID;
extern char hiyaNdsPath[14];

void unlaunchSetHiyaBoot(void) {
	memcpy((u8*)0x02000800, unlaunchAutoLoadID, 12);
	*(u16*)(0x0200080C) = 0x3F0;		// Unlaunch Length for CRC16 (fixed, must be 3F0h)
	*(u16*)(0x0200080E) = 0;			// Unlaunch CRC16 (empty)
	*(u32*)(0x02000810) |= BIT(0);		// Load the title at 2000838h
	*(u32*)(0x02000810) |= BIT(1);		// Use colors 2000814h
	*(u16*)(0x02000814) = 0x7FFF;		// Unlaunch Upper screen BG color (0..7FFFh)
	*(u16*)(0x02000816) = 0x7FFF;		// Unlaunch Lower screen BG color (0..7FFFh)
	memset((u8*)0x02000818, 0, 0x20+0x208+0x1C0);		// Unlaunch Reserved (zero)
	int i2 = 0;
	for (int i = 0; i < 14; i++) {
		*(u8*)(0x02000838+i2) = hiyaNdsPath[i];		// Unlaunch Device:/Path/Filename.ext (16bit Unicode,end by 0000h)
		i2 += 2;
	}
	while (*(u16*)(0x0200080E) == 0) {	// Keep running, so that CRC16 isn't 0
		*(u16*)(0x0200080E) = swiCRC16(0xFFFF, (void*)0x02000810, 0x3F0);		// Unlaunch CRC16
	}
}

void cardLaunch()
{
    *(u32 *)(0x02000300) = 0x434E4C54; // Set "CNLT" warmboot flag
    *(u16 *)(0x02000304) = 0x1801;
    *(u32 *)(0x02000308) = 0x43415254; // "CART"
    *(u32 *)(0x0200030C) = 0x00000000;
    *(u32 *)(0x02000310) = 0x43415254; // "CART"
    *(u32 *)(0x02000314) = 0x00000000;
    *(u32 *)(0x02000318) = 0x00000013;
    *(u32 *)(0x0200031C) = 0x00000000;
    while (*(u16 *)(0x02000306) == 0x0000)
    { // Keep running, so that CRC16 isn't 0
        *(u16 *)(0x02000306) = swiCRC16(0xFFFF, (void *)0x02000308, 0x18);
    }

	unlaunchSetHiyaBoot();

    fifoSendValue32(FIFO_USER_02, 1); // Reboot into DSiWare title, booted via Launcher
}

void dsiSysMenuLaunch()
{
    *(u32 *)(0x02000300) = 0x434E4C54; // Set "CNLT" warmboot flag
    *(u16 *)(0x02000304) = 0x1801;
    *(u32 *)(0x02000310) = 0x4D454E55; // "MENU"
	unlaunchSetHiyaBoot();
    fifoSendValue32(FIFO_USER_02, 1);  // ReturntoDSiMenu
}

void dsiLaunch(u32 *tid)
{

    *(u32 *)(0x02000300) = 0x434E4C54; // Set "CNLT" warmboot flag
    *(u16 *)(0x02000304) = 0x1801;
    *(u32 *)(0x02000308) = tid[0];
    *(u32 *)(0x0200030C) = tid[1];
    *(u32 *)(0x02000310) = tid[0];
    *(u32 *)(0x02000314) = tid[1];
    *(u32 *)(0x02000318) = 0x00000017;
    *(u32 *)(0x0200031C) = 0x00000000;
    while (*(u16 *)(0x02000306) == 0x0000)
    { // Keep running, so that CRC16 isn't 0
        *(u16 *)(0x02000306) = swiCRC16(0xFFFF, (void *)0x02000308, 0x18);
    }

	unlaunchSetHiyaBoot();

    fifoSendValue32(FIFO_USER_02, 1); // Reboot into DSiWare title, booted via Launcher
}
void dsiLaunchSystemSettings()
{
    char tmdpath[256];
    u8 titleID[4];
    for (u8 i = 0x41; i <= 0x5A; i++)
    {
        snprintf(tmdpath, sizeof(tmdpath), "sd:/title/00030015/484e42%x/content/title.tmd", i);
        if (access(tmdpath, F_OK) == 0)
        {
            titleID[0] = i;
            titleID[1] = 0x42;
            titleID[2] = 0x4e;
            titleID[3] = 0x48;
            break;
        }
    }

    *(u32 *)(0x02000300) = 0x434E4C54; // Set "CNLT" warmboot flag
    *(u16 *)(0x02000304) = 0x1801;
    *(u8 *)(0x02000308) = titleID[0];
    *(u8 *)(0x02000309) = titleID[1];
    *(u8 *)(0x0200030A) = titleID[2];
    *(u8 *)(0x0200030B) = titleID[3];
    *(u32 *)(0x0200030C) = 0x00030015;
    *(u8 *)(0x02000310) = titleID[0];
    *(u8 *)(0x02000311) = titleID[1];
    *(u8 *)(0x02000312) = titleID[2];
    *(u8 *)(0x02000313) = titleID[3];
    *(u32 *)(0x02000314) = 0x00030015;
    *(u32 *)(0x02000318) = 0x00000017;
    *(u32 *)(0x0200031C) = 0x00000000;
    while (*(u16 *)(0x02000306) == 0x0000)
    { // Keep running, so that CRC16 isn't 0
        *(u16 *)(0x02000306) = swiCRC16(0xFFFF, (void *)0x02000308, 0x18);
    }

	unlaunchSetHiyaBoot();

    fifoSendValue32(FIFO_USER_08, 1); // Reboot into System Settings
}

#endif
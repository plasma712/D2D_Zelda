#pragma once

#ifndef __EXTERN_H__

extern HWND g_hWnd;


#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif


#define __EXTERN_H__
#endif
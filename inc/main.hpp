#ifndef MAIN_HPP
#define MAIN_HPP

#include <windows.h>

#define IDM_MYMENURESOURCE   100
#define IDM_MYMENURESOURCE   100
#define IDM_SAVE 101
#define IDM_LOAD 102
#define IDI_BUTTON_PENCIL 150
#define IDI_ICON 151
#define IDI_BUTTON_CLEAR 152
#define IDI_BUTTON_START 153
#define IDI_BUTTON_FINISH 154
#define IDI_BUTTON_BEGIN 155

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM); 

#ifdef DEBUG
	#include <time.h>
	#include <iomanip>
	#include <fstream>
	extern std::fstream dbg_out;

#define INFO(x) 												\
{ 																\
	float t = ((float)clock()) / (CLOCKS_PER_SEC);				\
	dbg_out << "[i]" << "[" << __FILE__ << "][" 				\
	<< __FUNCTION__ << "][" << __LINE__ << "] :" << std::fixed 	\
	<< std::setprecision(2) << t << " " << x << std::endl; 		\
} 																\
do {} while (0)

#define ERR(x) 													\
{																\
	float t = ((float)clock()) / (CLOCKS_PER_SEC);				\
	dbg_out << "[-]" << "[" << __FILE__ << "][" 				\
	<< __FUNCTION__ << "][" << __LINE__ << "] :" << std::fixed	\
	<< std::setprecision(2) << t << " " << x << std::endl; 		\
} 																\
do {} while (0)

#define ERR2(x, dwErr) 											\
{ 																\
	char *msg = get_api_err_msg(dwErr);							\
	float t = ((float)clock()) / (CLOCKS_PER_SEC);				\
	dbg_out << "[-]" << "[" << __FILE__ << "][" 				\
	<< __FUNCTION__ << "][" << __LINE__ << "] :" << std::fixed	\
	<< std::setprecision(2) << t << " " << x << " : " << msg 	\
	<< std::endl; 												\
	delete[] msg;												\
} 																\
do {} while (0)

#define SUCC(x) 												\
{ 																\
	float t = ((float)clock()) / (CLOCKS_PER_SEC);				\
	dbg_out << "[+]" << "[" << __FILE__ << "][" 				\
	<< __FUNCTION__ << "][" << __LINE__ << "] :" << std::fixed 	\
	<< std::setprecision(2) << t << " " << x << std::endl; 		\
} 																\
do {} while (0)

#else	// not debug

#define INFO(x) do {} while (0)
#define ERR(x) do {} while (0)
#define ERR2(x, dwErr) { 					\
	PrintCSBackupAPIErrorMessage(dwErr); 	\
} 											\
do {} while (0)
#define SUCC(x) do {} while (0)
	
#endif

#define GET_X_LPARAM(lParam) lParam & 0xFFFF
#define GET_Y_LPARAM(lParam) lParam >> 0x10

#endif 	// MAIN_HPP
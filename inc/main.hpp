#ifndef MAIN_HPP
#define MAIN_HPP

#include <windows.h>

#define IDM_MYMENURESOURCE   100
#define IDM_MYMENURESOURCE   100
#define IDM_SAVE 101
#define IDM_LOAD 102
#define IDI_BUTTON_PENCIL 150
#define IDI_ICON 151

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
	PrintCSBackupAPIErrorMessage(dwErr);						\
	float t = ((float)clock()) / (CLOCKS_PER_SEC);				\
	dbg_out << "[-]" << "[" << __FILE__ << "][" 				\
	<< __FUNCTION__ << "][" << __LINE__ << "] :" << std::fixed	\
	<< std::setprecision(2) << t << " " << x << std::endl; 		\
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

#endif 	// MAIN_HPP
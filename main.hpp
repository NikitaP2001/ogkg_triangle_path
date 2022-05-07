#ifndef MAIN_HPP
#define MAIN_HPP

#define IDM_MYMENURESOURCE   100
#define IDM_MYMENURESOURCE   100
#define IDM_SAVE 101
#define IDM_LOAD 102


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM); 

#ifdef DEBUG

	#include <fstream>
	extern std::fstream dbg_out;

#define INFO(x) 												\
{ 																\
	dbg_out << "[i]" << "[" << __FILE__ << "][" 				\
	<< __FUNCTION__ << "][" << __LINE__ << "] " x << std::endl; \
} 																\
do {} while (0)

#define ERR(x) 													\
{																\
	dbg_out << "[-]" << "[" << __FILE__ << "][" 				\
	<< __FUNCTION__ << "][" << __LINE__ << "] " x << std::endl; \
} 																\
do {} while (0)

#define ERR2(x, dwErr) 											\
{ 																\
	PrintCSBackupAPIErrorMessage(dwErr);						\
	dbg_out << "[-]" << "[" << __FILE__ << "][" 				\
	<< __FUNCTION__ << "][" << __LINE__ << "] " x << std::endl; \
} 																\
do {} while (0)

#define SUCC(x) 												\
{ 																\
	dbg_out << "[+]" << "[" << __FILE__ << "][" 				\
	<< __FUNCTION__ << "][" << __LINE__ << "] " x << std::endl; \
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
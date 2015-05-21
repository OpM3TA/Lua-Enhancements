#include <iostream>
#include <fstream>
#include <Windows.h>
#include <cstdint>
#include <tchar.h>



extern "C" {
#include <C:\\Users\\Jake\\Desktop\\lua\\include\\lua.h>
#include <C:\\Users\\Jake\\Desktop\\lua\\include\\lualib.h>
#include <C:\\Users\\Jake\\Desktop\\lua\\include\\lauxlib.h>

#pragma comment(lib, "C:\\Users\\Jake\\Desktop\\lua\\lib\\lua5.1.lib")
}



//luaopen_io(L); // provides io.*
//luaopen_table(L);
//luaopen_string(L);
//luaopen_math(L);


DWORD TARGET = 0000;

// 0=OK, 1=CannotWriteMem, 2=CannotOpenProcess
// int write_addr(DWORD pid, LPCVOID addr, int newdata);
// int read_addr(DWORD PID, LPCVOID ADDR, int bytes);

int write_addr(LPCVOID addr, int newdata);
int read_addr(LPCVOID ADDR, int bytes);
DWORD Pid_from_Title(LPCSTR winTitle);



static int Get_Target(lua_State *L) {
	lua_pushnumber(L, TARGET);
	return 1;
}

static int set_title(lua_State *L) {

	HWND hand;
	const char* nTitle = lua_tostring(L, 2);
	int thand = lua_tonumber(L, 1);
	hand = (HWND)thand;

	SetWindowTextA(hand, (LPCSTR)nTitle);
	return 0;
}

static int terminateW(lua_State *L) {
	
	int pid;
	if (lua_gettop(L) == 1){
		int pid = lua_tonumber(L, 1);
		HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (phandle != NULL){
			TerminateProcess(phandle, 0);
			lua_pushnumber(L, 0);
		}
		else {
			lua_pushnumber(L, -1);
		}
	}
	else if (lua_gettop(L)==0)
	{
		pid = TARGET;
		HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (phandle != NULL){
			TerminateProcess(phandle, 0);
			lua_pushnumber(L, 0);
		}
		else {
			lua_pushnumber(L, -1);
		}
	}
	else
	{
		lua_pushnumber(L, -1);
	}
	

	
	return 1;
}

static int get_hwnd(lua_State *L) {
	HWND hand;
	hand = FindWindowA(NULL, (LPCSTR)lua_tostring(L, 1));
	lua_pushnumber(L, (int)hand);
	return 1;
}


static int display(lua_State *L) {
	if (lua_gettop(L) == 1){
		const char* msg = lua_tostring(L, 1);
		MessageBoxA(0, (LPCSTR)msg, (LPCSTR)"", 0);
	}
	else {
		const char* msg = lua_tostring(L, 1);
		const char *title = lua_tostring(L, 2);
		MessageBoxA(0, (LPCSTR)msg, (LPCSTR)title, 0);
	}
	
	
	return 0;
}



static int set_target_from_pid(lua_State *L) {
	TARGET = lua_tonumber(L, 1);
	return 0;
}
static int set_target_from_window(lua_State *L) {
	const char* title = lua_tostring(L, 1);
	TARGET = Pid_from_Title(title);
	lua_pushnumber(L, TARGET);
	return 1;
}


static int mem_write(lua_State *L) 
{
	//int pid = lua_tonumber(L, 1);
	int addr = lua_tonumber(L, 1);  /* get argument */
	int new_value = lua_tonumber(L, 2);

	lua_pushnumber(L, write_addr((LPCVOID)addr, new_value)); // push return 

	return 1;  /* number of results */
}

static int mem_read(lua_State *L)
{
	// int pid = lua_tonumber(L, 1);
	// int bytes = lua_tonumber(L, 3);
	int addr = lua_tonumber(L, 1);  /* get argument */

	// push return 
	lua_pushnumber(L, read_addr((LPCVOID)addr, 4));


	return 1;  /* number of results */
}



int read_addr(LPCVOID ADDR, int bytes)
{

	int value = 0;
	// hwnd = FindWindow(NULL, L"Minesweeper");
	// GetWindowThreadProcessId(hwnd, &pid);
	// HWND hwnd;
		
	HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, TARGET);
		if (!phandle)
		{
			return -1;
		}
		else
		{
			if (ReadProcessMemory(phandle, (void*)ADDR, &value, sizeof(bytes), 0)) {
				return value;
			}
		}

}

int write_addr(LPCVOID addr, int newdata) 
{
	int ret = 0;
	
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, TARGET);
		if (!hProcess) {
			ret = 2;
		}
		else 
		{
			DWORD newdatasize = sizeof(newdata);
			if (WriteProcessMemory(hProcess, (LPVOID)addr, &newdata, newdatasize, NULL))
			{
				ret = 0;
			}
			else {
				ret = 1;
			}
			CloseHandle(hProcess);

	   }
		return ret;
}



 
DWORD Pid_from_Title(LPCSTR winTitle) {

	HWND hwnd;
	DWORD pid;
	hwnd = FindWindowA(NULL, winTitle);
	if (hwnd == NULL){
		printf("Error Finding Window");
		return -1;
	}
	else {
		GetWindowThreadProcessId(hwnd, &pid);
	}
	return pid;

}



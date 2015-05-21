#include "Mods.h"



// Read file.
// run the file as a lua script.
// then using lua get the vars for each thing the memory editor needs:
// process = Proc.exe
// module = Proc
// pointers = {0x00, 0x00, 0x00}
// etc then do the mem editing
// ?



// TODO:
// Add use of pointers (base_addr+offsets)
// Reading a lua table into a Vector or directly into an array??
void register_functions(lua_State *L)
{
	lua_pushcfunction(L, mem_write);
	lua_setglobal(L, "Write");

	lua_pushcfunction(L, mem_read);
	lua_setglobal(L, "Read");

	lua_pushcfunction(L, set_target_from_pid);
	lua_setglobal(L, "SetTargetFromPid");

	lua_pushcfunction(L, set_target_from_window);
	lua_setglobal(L, "SetTargetFromWindow");

	lua_pushcfunction(L, display);
	lua_setglobal(L, "Display");

	//lua_pushcfunction(L, get_hwnd);
	//lua_setglobal(L, "GetHandle");

	//lua_pushcfunction(L, set_title);
	//lua_setglobal(L, "SetTitle");

	lua_pushcfunction(L, terminateW);
	lua_setglobal(L, "TerminateProcess");
	lua_pushcfunction(L, Get_Target);
	lua_setglobal(L, "GetTarget");

}
int main(int argc, CHAR* ARGV[])
{

	lua_State *lua = luaL_newstate();
	luaL_openlibs(lua);
	register_functions(lua);

// Replace with file you want to load, or use ARGV[1]
	if (luaL_loadfile(lua, "magic.lua") || lua_pcall(lua, 0, 0, 0))
	{
		printf("error: %s", lua_tostring(lua, -1));
	}


	getchar();
	return 0;
}


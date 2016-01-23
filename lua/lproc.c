// All functions composited with lua c api

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdint.h>
#include <stdio.h>
#include "_cgo_export.h"


lua_State * ll_newstate(lua_State *L){
	const char *chunk = luaL_checkstring(L, 1);
	lua_State *L1 = luaL_newstate();

	if(L1 == NULL) {
		luaL_error(L, "unable to create new state");
	}

	if(luaL_loadstring(L1, chunk) != 0) {
		luaL_error(L, "error starting thread: %s", lua_tostring(L1, -1));
	}

	return L1;
}

void ll_thread(lua_State *L) {
	// lua_State *L = (lua_State *)arg;
	// openlibs(L);
	// luaL_requiref(L, "lproc", luaopen_lproc, 1);
	luaL_openlibs(L);
	// lua_pop(L, 1);

	if(lua_pcall(L, 0, 0, 0) != 0) {
		fprintf(stderr, "thread error: %s", lua_tostring(L, -1));
	}

	// pthread_cond_destroy(&getself(L)->cond);
	lua_close(L);
}

// void openlibs(lua_State *L) {
//   luaL_requiref(L, "_G", luaopen_base, 1);
//   luaL_requiref(L, "package", luaopen_package, 1);
//   lua_pop(L, 2);
//   registerlib(L, "io", luaopen_io);
//   registerlib(L, "os", luaopen_os);
//   registerlib(L, "table", luaopen_table);
//   registerlib(L, "string", luaopen_string);
//   registerlib(L, "math", luaopen_math);
//   registerlib(L, "debug", luaopen_debug);
// }





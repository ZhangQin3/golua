// All functions composited with lua c api

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdint.h>
#include <stdio.h>
#include "_cgo_export.h"


void glua_newmetatable(lua_State *L, char* name){
	luaL_newmetatable(L, name);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
}





// All functions composited with lua c api

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "_cgo_export.h"

typedef struct {
	char *data;
	size_t cap;
    size_t len;
} dump_func;

int ll_lua_Writer(lua_State* ls, const void* p, size_t sz, void* ud) {
    dump_func* bd = (dump_func*)ud;

    char* newData;
    if (bd->len + sz > bd->cap) {
	    newData = (char*)realloc(bd->data, bd->cap + 512);
	}

    if (newData) {
        memcpy(newData + bd->len, p, sz);
        bd->data = newData;
        bd->len += sz;
    } else {
        free(newData);
        return 1;
    }

    return 0;
}

lua_State * ll_newstate(lua_State *L) {
	if (lua_isfunction(L, 1) == 0) {
		luaL_error(L, "It should be a lua function");
	}

	dump_func func = {0L, 0, 0};
	if (lua_dump(L, ll_lua_Writer, &func, 0) != 0) {
		luaL_error(L, "dump the lua function failed");
	}
	lua_pop(L, -1);

	lua_State *L1 = luaL_newstate();
	if (L1 == NULL) {
		luaL_error(L, "unable to create new state");
	}

	if (luaL_loadbuffer(L1, func.data, func.len, "lproc_func") != 0) {
		luaL_error(L, "error starting thread: %s", lua_tostring(L1, -1));
	}

	free(func.data);
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





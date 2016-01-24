// Support lua multi-thread backed by goroutine of golang

#include <lua.h>
#include <lauxlib.h>
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

static void transfer_args(lua_State *dst, lua_State *src) {
  int n = lua_gettop(src);
  int i;
  for(i = 2; i <= n; i++) {
    int t = lua_type(src, i);
    switch(t) {
      case LUA_TSTRING:
        lua_pushstring(dst, lua_tostring(src, i));
        break;

      case LUA_TBOOLEAN:
        lua_pushboolean(dst, lua_toboolean(src, i));
        break;

      case LUA_TNUMBER:
        lua_pushnumber(dst, lua_tonumber(src, i));
        break;

      default:
        luaL_error(src, "unable to src type");
        break;
    }
  }
  // remove the thread function's args from src to leave the function on the stack top
  lua_pop(src, n-1);
}

lua_State * ll_newstate(lua_State *L) {
	if (lua_isfunction(L, 1) == 0) {
		luaL_error(L, "It should be a lua function");
	}

	lua_State *L1 = luaL_newstate();
	if (L1 == NULL) {
		luaL_error(L, "unable to create new state");
	}

	int n = lua_gettop(L);
	if (n>1) {
		// reserve an empty slot for the thread function
		lua_settop(L1, 1);
		transfer_args(L1, L);
	}

	dump_func func = {0L, 0, 0};
	if (lua_dump(L, ll_lua_Writer, &func, 0) != 0) {
		luaL_error(L, "dump the lua thread function failed");
	}
	// pop the thread function form L because lua_dump does not pop it
	lua_pop(L, 1);

	if (luaL_loadbuffer(L1, func.data, func.len, "lproc_func") != 0) {
		luaL_error(L, "error starting thread: %s", lua_tostring(L1, -1));
	}

	// Move the function at top to index 1
	if (n>1) {
		lua_replace(L1, 1);
	}
    
    // push the thread function's arg numbers
	lua_pushnumber(L1, n-1);

	free(func.data);
	return L1;
}

void ll_thread(lua_State *L) {
	luaL_openlibs(L);
    int nargs = lua_tointeger(L, -1);
    lua_pop(L, 1);
	if(lua_pcall(L, nargs, 0, 0) != 0) {
		fprintf(stderr, "thread error: %s", lua_tostring(L, -1));
	}

	lua_close(L);
}

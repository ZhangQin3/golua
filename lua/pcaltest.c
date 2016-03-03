#include <stdio.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
// #include <dlfcn.h>
// #include <math.h>

static int cont(lua_State *L, int status, lua_KContext ctx) {
  printf("in cont!!\n");
  return 0;
}

static int pcall_test(lua_State *L) {
  return lua_yield(L,0);
}

static int mytest(lua_State *L) {
  printf("mytest\n");
  lua_yield(L,0);
  printf("after yield\n");
  // lua_pushcfunction(L, pcall_test);
  // int ret = lua_pcallk(L, 0, 0, 0, 0, cont);
  // printf("after pcallk\n");
  return 1;
}

void test(lua_State *L1) {
  // lua_State *L = luaL_newstate();
  lua_State *L = lua_newthread(L1);
  // luaL_openlibs(L);
  lua_pushcfunction(L, mytest);

  // //lua_pushcfunction(L, pcall_test);
  // //lua_callk(L, 0, 0, 0, cont);
  // if(ret != 0)
  //   {
  //   const char* err = luaL_checkstring(L, -1);
  // //err : attempt to yield from outside a coroutine
  // printf("%s\n", err);
  // }

  // //lua_resume(L, NULL, 0);
  puts("-------------------------------1");
  int ret = lua_resume(L, NULL, 0);
  puts("-------------------------------2");
  if((ret!=LUA_OK) && (ret!=LUA_YIELD))
  {
    const char* err = luaL_checkstring(L, -1);
    printf("===%s\n", err);
    return;
  }
  puts("-------------------------------3");
  ret = lua_resume(L, NULL, 0);
  printf("------------------------------- %d\n", ret);
  ret = lua_resume(L, NULL, 0);
  printf("------------------------------- %d\n", ret);
  // lua_close(L);
  // return 0;
}
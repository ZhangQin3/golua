// Support lua multi-thread backed by goroutine of golang

lua_State * ll_newstate(lua_State *L);
void ll_thread(lua_State *L);
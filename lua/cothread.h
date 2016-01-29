
typedef struct co_thread  {
	lua_State * s;
	lua_State * p;
	int nargs;
}co_thread;


co_thread ll_cothread(lua_State *L);
int kickoff(co_thread * ct);
int resume(co_thread * ct);

void test(lua_State *L);
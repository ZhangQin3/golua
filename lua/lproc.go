package lua

//#include <lua.h>
//#include "golua.h"
//#include "lproc.h"
import "C"

func (L *State) thread() {
	C.ll_thread(L.s)
}

func (L *State) PStart() {
	L1 := newState(C.ll_newstate(L.s))

	go L1.thread()
}

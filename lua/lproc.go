package lua

//#include <lua.h>
//#include "golua.h"
//#include "lproc.h"
import "C"

import (
	"sync"
	"time"
)

type Proc struct {
	L    *State
	cond *sync.Cond
	// channel string
}

var (
	sender map[string]Proc = make(map[string]Proc)
	recver map[string]Proc = make(map[string]Proc)
	gMutex *sync.Mutex     = new(sync.Mutex)
)

func (L *State) thread() {
	RegisterLProc(L)
	C.ll_thread(L.s)
}

// func (L *State) PStart() {
// 	L1 := newState(C.ll_newstate(L.s))

// 	go L1.thread()
// }

// func (L *State) Send() int {
// 	ch := L.CheckString(1)
// 	gMutex.Lock()

// 	p, ok := recver[ch]
// 	if ok {
// 		C.transfer_args(p.L.s, L.s)
// 		delete(recver, ch)
// 		p.cond.Signal()
// 	} else {
// 		p = Proc{L, sync.NewCond(gMutex)}
// 		sender[ch] = p
// 		p.cond.Wait()
// 	}
// 	gMutex.Unlock()

// 	return 0
// }

// func (L *State) Recv() int {
// 	ch := L.CheckString(1)
// 	C.lua_settop(L.s, 1)
// 	gMutex.Lock()

// 	p, ok := sender[ch]
// 	if ok {
// 		C.transfer_args(L.s, p.L.s)
// 		delete(sender, ch)
// 		p.cond.Signal()
// 	} else {
// 		p = Proc{L, sync.NewCond(gMutex)}
// 		recver[ch] = p
// 		p.cond.Wait()
// 	}
// 	gMutex.Unlock()

// 	return L.GetTop() - 1
// }

// -------------------------------------------
func start(L *State) int {
	// L.PStart()
	// return 0
	L1 := newState(C.ll_newstate(L.s))

	go L1.thread()
	return 0
}

func send(L *State) int {
	// return L.Send()
	ch := L.CheckString(1)
	gMutex.Lock()

	p, ok := recver[ch]
	if ok {
		C.transfer_args(p.L.s, L.s)
		delete(recver, ch)
		p.cond.Signal()
	} else {
		p = Proc{L, sync.NewCond(gMutex)}
		sender[ch] = p
		p.cond.Wait()
	}
	gMutex.Unlock()

	return 0
}

func recv(L *State) int {
	// return L.Recv()
	ch := L.CheckString(1)
	C.lua_settop(L.s, 1)
	gMutex.Lock()

	p, ok := sender[ch]
	if ok {
		C.transfer_args(L.s, p.L.s)
		delete(sender, ch)
		p.cond.Signal()
	} else {
		p = Proc{L, sync.NewCond(gMutex)}
		recver[ch] = p
		p.cond.Wait()
	}
	gMutex.Unlock()

	return L.GetTop() - 1
}

func sleep(L *State) int {
	L.GCheckFunctionArgs("sleep", 1)
	n := time.Duration(L.CheckInteger(1))
	time.Sleep(n * time.Millisecond)
	return 0
}

func RegisterLProc(L *State) {
	L.NewLib("lproc", GoFuncs{"start": start, "send": send, "recv": recv, "sleep": sleep})
}

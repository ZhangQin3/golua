package lua

//#include <lua.h>
//#include "golua.h"
//#include "cothread.h"
import "C"

import (
	"fmt"
	// "sync"
	"time"
)

type thread struct {
	ct *C.co_thread
}

var (
	gTrigerChan chan int          = make(chan int, 1)
	gYieldChan  chan *C.co_thread = make(chan *C.co_thread, 32)
	// gMutex      *sync.Mutex       = new(sync.Mutex)
)

func ct_schedule() {
	t := time.NewTicker(35 * time.Millisecond)
	for {
		select {
		case <-gTrigerChan:
			<-t.C // let the main thread run when waiting the ticker t to expire
			if len(gYieldChan) > 0 {
				ct := <-gYieldChan
				resume(ct)
			}
		}
	}
}

func ct_start(L *State) int {
	ct := C.ll_cothread(L.s)
	kickoff(&ct)
	go ct_schedule()

	return 0
}

func resume(ct *C.co_thread) {
	r := int(C.resume(ct))
	if r == 1 {
		gYieldChan <- ct
	}
	gTrigerChan <- r
}

func kickoff(ct *C.co_thread) {
	r := int(C.kickoff(ct))
	if r == 1 {
		gYieldChan <- ct
	}
	gTrigerChan <- r
}

func ct_sleep(L *State) int {
	// L.GCheckFunctionArgs("sleep", 1)
	nargs := L.GetTop()
	for i := 1; i <= nargs; i++ {
		fmt.Printf("===%d== %d\n", i, L.Type(i))
	}
	n := L.CheckInteger(-1)
	for i := 0; i < n*50; i++ {
		time.Sleep(20)
	}
	return 0
}

func RegisterCothread(L *State) {
	L.NewLib("cot", GoFuncs{"start": ct_start, "sleep": ct_sleep})
}

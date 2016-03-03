package lua

//#include <lua.h>
//#include <stdlib.h>
//#include "golua.h"
//#include "cothread.h"
import "C"

import (
	"time"
	"unsafe"
)

type thread struct {
	ct *C.co_thread
}

var (
	gTrigerChan chan int          = make(chan int)
	gYieldChan  chan *C.co_thread = make(chan *C.co_thread, 32)
	gSchedule   bool              = true
)

func ct_schedule() {
	t := time.NewTicker(15 * time.Millisecond)
done:
	for {
		select {
		case <-gTrigerChan:
			<-t.C // let the main thread run when waiting the ticker t to expire
			if len(gYieldChan) == 0 {
				break done
			} else {
				ct := <-gYieldChan
				go resume(ct)
			}
		}
	}
}

func ct_start(L *State) int {
	ct := C.ll_cothread(L.s)
	go kickoff(&ct)
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

// func ct_sleep(L *State) int {
// 	// if gSchedule {
// 	// 	gSchedule = false
// 	// 	go ct_schedule()
// 	// }
// 	// L.GCheckFunctionArgs("sleep", 1)
// 	// nargs := L.GetTop()
// 	// for i := 1; i <= nargs; i++ {
// 	// 	fmt.Printf("===%d== %d\n", i, L.Type(i))
// 	// }
// 	fmt.Println("before in sleep")
// 	<-gSleepChan
// 	fmt.Println("before in sleep, after get")
// 	n := time.Duration(L.CheckInteger(1))
// 	// for i := 0; i < n*50; i++ {
// 	time.Sleep(n * time.Millisecond)
// 	gSleepChan <- 1
// 	fmt.Println("after in sleep")
// 	// }
// 	return 0
// }

func RegisterCothread(L *State) {
	L.NewLib("coth", GoFuncs{"start": ct_start})

	cs := C.CString("coth")
	defer C.free(unsafe.Pointer(cs))
	C.register_sleep(L.s, cs)
}

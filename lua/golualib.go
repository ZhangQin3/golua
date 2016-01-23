// All functions composited with lua c api

package lua

//#include <lua.h>
//#include <lauxlib.h>
//#include <lualib.h>
//#include <stdlib.h>
//#include "golualib.h"
import "C"
import "unsafe"
import "fmt"

// glua_newmetatable
func (L *State) GNewMetaTable(tname string) {
	cname := C.CString(tname)
	defer C.free(unsafe.Pointer(cname))
	C.glua_newmetatable(L.s, cname)
}

func (L *State) GCheckFunctionArgs(name string, num int) {
	if L.GetTop() != num {
		L.RaiseError(fmt.Sprintf("Function %s expects exactly %d arguments", name, num))
	}
}

func (L *State) GCheckMethodArgs(name string, num int) {
	if L.GetTop() != num {
		L.RaiseError(fmt.Sprintf("Method %s expects exactly %d arguments", name, num-1))
	}
}

package main

import "../lua"
import "fmt"

func test(L *lua.State) int {
	fmt.Println("hello world! from go!")
	return 0
}

func test2(L *lua.State) int {
	arg := L.CheckInteger(-1)
	argfrombottom := L.CheckInteger(1)
	fmt.Print("test2 arg: ")
	fmt.Println(arg)
	fmt.Print("from bottom: ")
	fmt.Println(argfrombottom)
	return 0
}

func main() {
	L := lua.NewState()
	defer L.Close()
	L.OpenLibs()

	L.GetGlobal("print")
	L.PushString("Hello World!")
	L.Call(1, 0)

	L.RegisterFuncs("mod", lua.GoFuncs{"test2": test2, "test": test})

	err := L.DoString("mod.test2(42)")
	L.DoString("mod.test()")

	fmt.Printf("Ciao %v\n", err)
}

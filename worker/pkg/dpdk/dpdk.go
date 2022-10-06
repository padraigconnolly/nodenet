package dpdk

import (
	"errors"
	"fmt"
)

/*
  #cgo CFLAGS: -I${SRCDIR}/clib
  #cgo LDFLAGS: -Wl,-rpath,${SRCDIR}/clib
  #cgo LDFLAGS: -L${SRCDIR}/clib
  #cgo LDFLAGS: -linit
  #include <init.h>
*/
import "C"

func Test() {
	fmt.Println("Hello, moon123.")
}

func DPDK() error {
	//cs := C.CString("Go string")
	//p := [6]C.int{2, 3, 5, 7, 11, 13}
	//s := [2]*C.char{C.CString("Item 1"), C.CString("Item 2")}
	ports := [2]*C.char{C.CString("0000:01:00.1"), C.CString("0000:01:02.1")}
	cpus := [3]C.int{2, 3, 5}
	// In Cgo, we need to pass pointer to first element in array i.e &p[0]
	//eal := C.struct_EALParams{myNum: &p[0], myLetter: cs, myString: &s[0], ports: &ports[0], numargs: 2}
	eal := C.struct_EALParams{ports: &ports[0],
		numPorts: 2,
		cpus:     &cpus[0],
		numCpus:  3,
		numArgs:  6,
	}
	//fmt.Printf("%+v", eal)
	_, err := C.DPDK(eal) //We ignore first result as it is a void function
	if err != nil {
		return errors.New("error calling DPDK function: " + err.Error())
	}
	return nil
}

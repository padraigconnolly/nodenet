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
	_, err := C.DPDK() //We ignore first result as it is a void function
	if err != nil {
		return errors.New("error calling DPDK function: " + err.Error())
	}
	return nil
}

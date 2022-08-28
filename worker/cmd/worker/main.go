package main

import (
	"fmt"

	"github.com/padraigconnolly/nodenet/worker/pkg/dpdk"
)

func main() {
	fmt.Println("Hello, world.")

	dpdk.Test()
}

.PHONY: all c go run

DPDKLIBPATH="./pkg/dpdk/clib"

all: c go run

c:
	gcc -c -Wall -Werror -o ${DPDKLIBPATH}/init.o ${DPDKLIBPATH}/init.c
	gcc -shared -o ${DPDKLIBPATH}/libinit.so ${DPDKLIBPATH}/init.o

go:
	go build -o app cmd/worker/main.go

run:
	./worker

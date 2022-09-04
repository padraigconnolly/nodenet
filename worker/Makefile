.PHONY: all c go run

DPDKLIBPATH="./pkg/dpdk/clib"

all: c go run

CFLAGS += -O3 $(shell pkg-config --cflags libdpdk)
LDFLAGS_SHARED = $(shell pkg-config --libs libdpdk)

c:
	gcc -c -Wall -Werror -o ${DPDKLIBPATH}/init.o ${DPDKLIBPATH}/init.c
	gcc -shared $(CFLAGS) -o ${DPDKLIBPATH}/libinit.so ${DPDKLIBPATH}/init.o $(LDFLAGS_SHARED)

go:
	go build -o worker cmd/worker/main.go

run:
	./worker

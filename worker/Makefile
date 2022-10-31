.PHONY: all c go run

DPDKLIBPATH="./pkg/dpdk/clib"

all: c go run

CFLAGS += -O3 -shared $(shell pkg-config --cflags libdpdk) -DALLOW_EXPERIMENTAL_API
LDFLAGS_SHARED = $(shell pkg-config --libs libdpdk)

c:
	cc -c -Wall -Werror -fPIC -march=native -o ${DPDKLIBPATH}/init.o ${DPDKLIBPATH}/init.c
	cc $(CFLAGS) ${DPDKLIBPATH}/init.o -o ${DPDKLIBPATH}/libinit.so $(LDFLAGS_SHARED)

go:
	go build -o worker cmd/worker/main.go

run:
	./worker

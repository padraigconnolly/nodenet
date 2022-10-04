# NodeNet Worker

This package contains Go code for the nodenet worker which will
be tasked with building the network applications that the nodenet
controller asks it to.

## Installation

First install Go as per the docs here: https://go.dev/doc/install

Then install DPDK as per the docs here: https://doc.dpdk.org/guides/linux_gsg/build_dpdk.html

Then in `nodenet/worker` run:
```bash
make c go
```

## Running

To run:
```bash
./worker
```

package domain

type WorkerAPI struct {
	Ports []string
	Cores []int
}

type Worker interface {
}

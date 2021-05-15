package npy

import (
	"testing"
	"math"
	"gonum.org/v1/gonum/mat"
	"log"
)

func TestNpy(t *testing.T) {
	row, col, data, err := Read("data/dense.npy")
	if err != nil {
		t.Errorf("Error: %v reading file", err)
	}

	if row != 100 || col != 100 || math.Floor(data[0] * 1e24) != 1614457 {
		t.Errorf("Error: max not matched")
	}

	x := mat.NewDense(row, col, data)
	tr := mat.Trace(x)
	log.Println(tr)
}
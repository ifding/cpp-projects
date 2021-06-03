// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
package mat

import (
	"errors"
	"fmt"
	"math"
	"math/rand"
	"sync"
	"sync/atomic"
	"unsafe"
)

// Errors
var (
	ErrNumElem = errors.New("bad number of elements")
	ErrMatSize = errors.New("bad size of matrix")
)

// Dense implements Matrix interface
// dense matrix underlying data struct
type Dense struct {
	m, n int
	data []float64
}

// addFloat64 add delta to given address atomically
// From: https://github.com/changkun/pkg/blob/master/lockfree/arithmetic.go
func addFloat64(addr *float64, delta float64) (new float64) {
	var old float64
	for {
		old = math.Float64frombits(atomic.LoadUint64((*uint64)(unsafe.Pointer(addr))))
		if atomic.CompareAndSwapUint64((*uint64)(unsafe.Pointer(addr)),
	        math.Float64bits(old), math.Float64bits(old+delta)) {
	        	break
	        }
	}
	return
}

// Zero matrix
func Zero(m, n int) *Dense {
	return &Dense{m: m, n:n, data: make([]float64, m*n)}
}

// Rand creates a size by size random matrix
func Rand(m, n int) *Dense {
	A := Zero(m, n)
	for i := 0; i < m; i++ {
		for j := 0; j < n; j++ {
			A.Set(i, j, rand.Float64())
		}
	}
	return A
}

// RandP concurrently creates a size by size random matrix
func RandP(m, n int) *Dense {
	A := Zero(m, n)
	wg := sync.WaitGroup{}
	for i := 0; i < m; i++ {
		wg.Add(1)
		go func(i int) {
			defer wg.Done()
			for j := 0; j < n; j++ {
				A.Set(i, j, rand.Float64())
			}
		}(i)
	}
	wg.Wait()
	return A
}

// NewDense creates a size by size matrix
func NewDense(m, n int) func(...float64) (*Dense, error) {
	A := Zero(m, n)
	return func(es ...float64) (*Dense, error) {
		if len(es) != m*n {
			return nil, ErrNumElem
		}
		// per row
		for i := 0; i < m; i++ {
			for j := 0; j < n; j++ {
				A.Set(i, j, es[i*n+j])
			}
		}
		return A, nil
	}
}

// NewDenseP concurrently creates a size by size matrix
func NewDenseP(m, n int) func(...float64) (*Dense, error) {
	A := Zero(m, n)
	return func(es ...float64) (*Dense, error) {
		if len(es) != m*n {
			return nil, ErrNumElem
		}

		// per row
		wg := sync.WaitGroup{}
		for i := 0; i < m; i++ {
			wg.Add(1)
			go func(i int) {
				defer wg.Done()
				for j := 0; j < n; j++ {
					A.Set(i, j, es[i*n+j])
				}
			}(i)
		}
		wg.Wait()
		return A, nil
	}
}

// Print the matrix
func (A *Dense) Print() {
	for i := 0; i < A.m; i++ {
		for j := 0; j < A.n; j++ {
			fmt.Printf("%.2f ", A.At(i, j))
		}
		fmt.Printf("\n")
	}
}

// Size of matrix
func (A *Dense) Size() (int, int) {
	return A.m, A.n
}

// Row of matrix
func (A *Dense) Row() int {
	return A.m
}

// Col of matrix
func (A *Dense) Col() int {
	return A.n
}

// At access element (i, j)
func (A *Dense) At(i, j int) float64 {
	return A.data[i*A.n+j]
}

// Set element (i, j) with val
func (A *Dense) Set(i, j int, val float64) {
	A.data[i*A.n+j] = val
}

// Inc adds element (i, j) with wal
func (A *Dense) Inc(i, j int, val float64) {
	addFloat64(&A.data[i*A.n+j], val)
}

// Mult multiple element (i, j) with wal
func (A *Dense) Mult(i, j int, val float64) {
	A.data[i*A.n+j] *= val
}

// Pow computes power of n of element (i, j)
func (A *Dense) Pow(i, j int, n float64) {
	A.data[i*A.n+j] = math.Pow(A.data[i*A.n+j], n)
}

// EqualShape check A.Size() == B.Size()
func (A *Dense) EqualShape(B Matrix) bool {
	am, an := A.Size()
	bm, bn := B.Size()
	if am != bm || an != bn {
		return false
	}
	return true
}

// Equal A and B?
func (A *Dense) Equal(B Matrix) bool {
	if !A.EqualShape(B) {
		return false
	}

	for i := 0; i < A.Row(); i++ {
		for j := 0; j < A.Col(); j++ {
			if A.At(i, j) != B.At(i, j) {
				return false
			}
		}
	}
	return true
}
package mat

// Add adds matrix B to A
func (A *Dense) Add(B Matrix) error {

	if !A.EqualShape(B) {
		return ErrMatSize
	}

	for i := 0; i < A.m; i++ {
		for j := 0; j < A.n; j++ {
			A.Inc(i, j, B.At(i, j))
		}
	}
	return nil
}

// Add A+B
func Add(A, B *Dense) (*Dense, error) {

	if !A.EqualShape(B) {
		return nil, ErrMatSize
	}

	C := Zero(A.m, A.n)
	for i := 0; i < A.m; i++ {
		for j := 0; j < A.n; j++ {
			C.Set(i, j, A.At(i, j) + B.At(i, j))
		}
	}
	return C, nil
}
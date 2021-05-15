# npy

Read in .npy files produced by numpy into Go

- Only supports dense float64 matrices
- .npz files are not currently supported
- Assumes a single matrix in the file


```
=== RUN   TestNpy
2021/05/15 00:03:21 File data/dense.npy is an npy file of version 1.0 with hdrLen 70
2021/05/15 00:03:21 Read 70 bytes
2021/05/15 00:03:21 Matrix shape: 100 X 100, Data size:80000 bytes
2021/05/15 00:03:21 -0.002062780308862442
--- PASS: TestNpy (0.00s)
PASS
ok  	npy	0.011s
```

### Usage


```
import (
  "github.com/gonum/gonum/mat"
)
...

rows, cols, data, err := npy.Read("data/dense.py")
x := mat.NewDense(rows, cols, data)
```


### npy format documentation

https://github.com/numpy/numpy/blob/master/doc/neps/npy-format.rst

### Reference

https://github.com/cquotient/go-npy
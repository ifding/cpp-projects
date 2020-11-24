#ifndef __COMPLEX__
#define __COMPLEX__

class complex;
// do assign plus
complex&
  __doapl (complex* ths, const complex& r);

class complex
{
private:
    /* data */
    double re, im;

    friend complex& __doapl (complex *, const complex&);
public:
    complex(double r = 0.0, double i = 0.0): re(r), im(i) { }
    complex& operator += (const complex&);
    double real() const { return re; }
    double imag() const { return im; }
};

inline complex&
__doapl (complex* ths, const complex& r)
{
    ths->re += r.re;
    ths->im += r.im;
    return *ths;
}

inline complex&
complex::operator += (const complex& r)
{
    return __doapl(this, r);
}

inline double
real (const complex& x)
{
    return x.real();
}

inline double
imag (const complex& x)
{
    return x.imag();
}

inline complex
operator + (const complex& x, const complex& y)
{
    return complex(real(x) + real(y), imag(x) + imag(y));
}

inline complex
operator + (double x, const complex& y)
{
    return complex(x + real(y), imag(y));
}

inline complex
operator + (const complex& x)
{
    return x;
}

inline complex
operator - (const complex& x)
{
    return complex(-real(x), -imag(x));
}

inline bool
operator == (const complex& x, const complex& y)
{
    return real(x) == real(y) and imag(x) == imag(y); 
}

#include <iostream>

using std::ostream;

ostream&
operator << (ostream& os, const complex& x)
{
    return os << '(' << x.real() << ',' << x.imag() << ')';
}

#endif
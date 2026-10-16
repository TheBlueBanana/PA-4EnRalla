using namespace std;
#include<stdio.h>
#include <iostream>

class Complex {
    public: double re, im;

    Complex (double r, double i) {
        re = r;
        im = i;
    }

    void show(){
        if (im >= 0) {
            cout << re << " + " << im << "i\n";
        } else {
            cout << re << " - " << im << "i\n";
        }
    }

    Complex operator+(const Complex &c)
    {
        return Complex(re+c.re, im+ c.im);
    }
};


int main(){
    Complex c(2, 3);
    Complex ca = c+c;
    ca.show();

    Complex *c2 = new Complex(c.re, c.im);
    cout << c2->re;

    cout << "Holi\n" << 10 << "\n";
    return 0;
}
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

string __kima_read() {
    string s;
    getline(cin, s);
    return s;
}

int __kima_toint(string s) { return stoi(s); }
double __kima_tofloat(string s) { return stod(s); }
string __kima_tostring(int x) { return to_string(x); }
string __kima_tostring(double x) { return to_string(x); }
string __kima_tostring(bool x) { return x ? "verdadeiro" : "falso"; }

int soma(int a, int b) {
    return (a + b);
}

int factorial(int n) {
    if ((n <= 1)) {
        return 1;
    } else {
        return (n * factorial((n - 1)));
    }
}

int main() {
    int x = 5;
    cout << soma(x, 3) << endl;
    cout << factorial(x) << endl;
    for (int i = 1; i < 5; ++i) {
        cout << i << endl;
    }
    return 0;
}

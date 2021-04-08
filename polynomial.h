#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>

const bool DEBUG = false;
using namespace std;
class Polynomial
{
public:
    Polynomial();
    Polynomial(double val);
    Polynomial(const Polynomial &other);
    Polynomial(const std::pair <double, unsigned int> term);
    Polynomial(vector<std::pair<double, unsigned int> >  coefs);

    void print() const;
    double solveWithVectors(vector<double> x);
    double solveWithValue(double val);
    bool getApp(double exact, double app, double tolerance);
    double getValueAtPosition(unsigned int position) const;
    Polynomial scale(const double scalar);

    Polynomial operator+(const Polynomial &other);
    Polynomial operator+=(const Polynomial &other);
    Polynomial operator*(const Polynomial &other);
    Polynomial operator*=(const Polynomial &other);
    Polynomial operator=(const Polynomial &other);
    friend ostream& operator<<(ostream &outs, const Polynomial &p);

private:
    //Polynomial coefficients and their locations
    vector<std::pair<double, unsigned int> > coefficients;
    int getIndexAtPosition(unsigned int position) const;
    void trim();
    void sort(int low, int high);
    int partition(int low, int high);


};

#endif // POLYNOMIAL_H

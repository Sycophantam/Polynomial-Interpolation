#include "polynomial.h"

Polynomial::Polynomial()
{
    coefficients.push_back(std::pair<double, double>{0,0});
}

Polynomial::Polynomial(double val)
{
    coefficients.push_back(std::pair<double, double>{val,0});
}

Polynomial::Polynomial(const Polynomial &other)
{
    coefficients = other.coefficients;
    sort(0, coefficients.size() - 1);
}

Polynomial::Polynomial(const std::pair<double, unsigned int> term)
{
    coefficients.push_back(term);
}

Polynomial::Polynomial(vector<std::pair <double, unsigned int> > coef)
{
    coefficients = coef;
    sort(0, coefficients.size() - 1);
}

double Polynomial::solveWithVectors(vector<double> x)
{
    double total = 0;
    std::vector<double>::size_type to = coefficients.size();
//    if(extra)
//    {
//        total += coefficients.back().first;
//        to = coefficients.size() - 1;
//    }
    for(unsigned u = 0; u < to; u++)
    {
        total += coefficients.at(u).first * x.at(u);
    }

    return total;
}

double Polynomial::solveWithValue(double val)
{
    double sum = coefficients.at(0).first;
    for(int i = coefficients.at(0).second; i > 0; i--)
    {
        //cout << sum << "*" << val << " = " << sum*val << endl;
        double prod;
        if(getValueAtPosition(i - 1) == 0)
        {
            prod = 0;
        }
        else
        {
            prod = getValueAtPosition(i - 1);
        }
        //cout << sum*val << " + " << prod << " = " << sum*val + prod << endl;
        sum = sum*val + prod;
    }
    return sum;
}

void Polynomial::print() const
{
    if(coefficients.at(0).first < 0)
        cout << "-";
    for(unsigned u = 0; u < coefficients.size() - 1; u++)
    {
        cout << abs(coefficients.at(u).first);
        cout << "x^" << coefficients.at(u).second << " ";
        if(coefficients.at(u + 1).first > 0)
            cout << "+ ";
        else
            cout << "- ";

    }
    unsigned u = coefficients.size() - 1;
    cout << abs(coefficients.at(u).first);
    if(coefficients.at(u).second != 0)
    {
        cout << "x";
        if(coefficients.at(u).second != 1)
            cout << "^" << coefficients.at(u).second << " ";
    }
    cout << endl;
}

bool Polynomial::getApp(double exact, double app, double tolerance)
{
    return abs(exact - app)/exact < tolerance;
}

double Polynomial::getValueAtPosition(unsigned int position) const
{
    for(unsigned u = 0; u < coefficients.size(); u++)
    {
        if(coefficients.at(u).second == position)
            return coefficients.at(u).first;
    }
    return 0;
}

Polynomial Polynomial::scale(const double scalar)
{
    for(unsigned u = 0; u < coefficients.size(); u++)
    {
        coefficients.at(u).first *= scalar;
    }
    return *this;
}

Polynomial Polynomial::operator+(const Polynomial &other)
{
    Polynomial first = *this;
    Polynomial *bpoly = &first;

    Polynomial second = other;
    Polynomial* spoly = &second;

    if(spoly->coefficients.size() > bpoly->coefficients.size())
    {
        Polynomial *temp = bpoly;
        bpoly = spoly;
        spoly = temp;
        temp = nullptr;
    }

    //Adding like terms in the polynomial
    for(unsigned u = 0; u < bpoly->coefficients.size(); u++)
    {
        double add = spoly->getValueAtPosition(bpoly->coefficients.at(u).second);
        if(add != 0)
        {
            bpoly->coefficients.at(u).first += add;
            spoly->coefficients.erase
                    (spoly->coefficients.begin() +
                   spoly->getIndexAtPosition(bpoly->coefficients.at(u).second));
        }
    }
    for(unsigned u = 0; u < spoly->coefficients.size(); u++)
    {
        bpoly->coefficients.push_back(spoly->coefficients.at(u));
    }
    vector<std::pair<double, unsigned int> > next = bpoly->coefficients;
    bpoly = nullptr;
    spoly = nullptr;
    Polynomial p(next);
    p.sort(0, p.coefficients.size() - 1);
    p.trim();
    return p;
}

Polynomial Polynomial::operator+=(const Polynomial &other)
{
    *this = *this + other;
    return *this;
}

Polynomial Polynomial::operator*(const Polynomial &other)
{

    vector<std::pair<double, unsigned int> > coefs = coefficients;
    vector<std::pair<double, unsigned int> > others = other.coefficients;

    Polynomial product(vector< std::pair<double, unsigned> >{{0,0}});
    //cout << "coefs.size(): " << coefs.size() << endl;
    for(unsigned u = 0; u < coefs.size(); u++)
    {
        if(coefs.at(u).first != 0)
        {
            Polynomial temp;
            //cout << "others.size is: " << others.size() << endl;
            for(unsigned v = 0; v < others.size(); v++)
            {
                //cout << "Pushing back" << endl;
                temp.coefficients.push_back({coefs.at(u).first * others.at(v).first,
                                             coefs.at(u).second + others.at(v).second});
            }
            product = product + temp;
            //cout << "Product is: " << endl;
            //product.print();
        }
    }

    Polynomial p(product);
    //cout << "p is: " << p << endl;
    //p.print();
    return p;
}

Polynomial Polynomial::operator*=(const Polynomial &other)
{
    *this = *this * other;
    return *this;
}

Polynomial Polynomial::operator=(const Polynomial &other)
{
    coefficients = other.coefficients;
    return *this;
}

int Polynomial::getIndexAtPosition(unsigned int position) const
{
    for(unsigned u = 0; u < coefficients.size(); u++)
    {
        if(coefficients.at(u).second == position)
            return static_cast<int>(u);
    }
    return -1;
}

void Polynomial::trim()
{
    for(unsigned u = 0; u < coefficients.size(); u++)
    {
        if(coefficients.at(u).first == 0)
        {
            coefficients.erase(coefficients.begin() + u);
            u--;
        }
    }
}

//Code for sorting algorithm taken from
//https://www.geeksforgeeks.org/quick-sort/
void Polynomial::sort(int low, int high)
{

//    cout << "low: " << low << endl;
//    cout << "high: " << high << endl;
    if (low < high)
    {
        int pi = partition(low, high);
//        cout << "pi is: " << pi << endl;
//        cout << "After partition" << endl;
//        print();
        //sort(pi + 1, high); // After pi

        sort(low, pi - 1);  // Before pi
//        cout << "After first sort" << endl;
//        print();
        sort(pi + 1, high); // After pi
//        cout << "After second sort" << endl;
//        print();

    }

}

//Code for sorting algorithm taken from
//https://www.geeksforgeeks.org/quick-sort/
int Polynomial::partition(int low, int high)
{
    // pivot (Element to be placed at right position)
    unsigned pivot = coefficients.at(static_cast<unsigned>(high)).second;

    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++)
    {
        // If current element is greater than the pivot
        if (coefficients.at(static_cast<unsigned>(j)).second > pivot)
        {
            i++;    // increment index of smaller element
            std::swap(coefficients.at(static_cast<unsigned>(i)),
                      coefficients.at(static_cast<unsigned>(j)));
        }
    }
    std::swap(coefficients.at(static_cast<unsigned>(i + 1)),
              coefficients.at(static_cast<unsigned>(high)));
    return (i + 1);
}

ostream& operator<<(ostream &outs, const Polynomial &p)
{
    if(p.coefficients.at(0).first < 0)
        outs << "-";
    for(unsigned u = 0; u < p.coefficients.size() - 1; u++)
    {
        //cout << "In for" << endl;
        if(p.coefficients.at(u).first != 0)
        {
            if(p.coefficients.at(u).first != 1)
            {
                outs << abs(p.coefficients.at(u).first);
            }

            outs << "x";
            if(p.coefficients.at(u).second != 1)
            {
                outs << "^" << p.coefficients.at(u).second << " ";
            }
            //cout << "The next term is " << p.coefficients.at(u+1).first << endl;
            if(p.coefficients.at(u + 1).first > 0)
            {
                //cout << p.coefficients.at(u + 1) << " is greater than 0" << endl;
                outs << " + ";
            }
            else if(p.coefficients.at(u + 1).first < 0)
            {
                outs << " - ";
            }
        }
    }
    unsigned u = p.coefficients.size() - 1;
    if(p.coefficients.at(u).first != 0)
        outs << abs(p.coefficients.at(u).first);
    if(p.coefficients.at(u).second != 0)
    {
        outs << "x";
        if(p.coefficients.at(u).second != 1)
            outs << "^" << p.coefficients.at(u).second << " ";
    }

    return outs;
}

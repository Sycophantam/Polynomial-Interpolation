#include <iostream>
#include <iomanip>
#include <fstream>
#include "polynomial.h"
using namespace std;

void dividedDifference(vector< std::pair<double, double> > base);
void LaGrange(vector< std::pair<double, double> > base);
void printUnsimplifiedPolynomial
(vector< std::pair<double, vector<Polynomial> > > terms);
void printSimplifiedPolynomial
(vector< std::pair<double, vector<Polynomial> > > terms);
int main()
{
//    Polynomial p(vector< std::pair <double, unsigned int> >{{1,1}});
//    Polynomial p2(vector< std::pair <double, unsigned int> >{{1,1}, {-1,0}});
//    cout << "p: " << p << endl;
//    cout << "p2: " << p2 << endl;
//    Polynomial p3 = p * p2;
//    cout << "p3: " << p3 << endl;

//    p2 *= p;
//    cout << "p2: " << p2 << endl;

    cout << "Welcome to the Polynomial Interpolator" << endl;
    int input = 0;
    while(true)
    {
        cout << "Input 1 to manually enter a set of values and their functional"
             << " values, or press 2 to enter a file. Press 0 to quit" << endl;
        cin >> input;
        if(input == 0)
            break;
        else if(input == 1)
        {
            vector< std::pair<double, double> > in;
            std::pair<double, double> index;
            string input2;
            int size = 0;
            while(true)
            {
                cout << "Enter the number of values for the polynomial" << endl;
                cin >> input2;
                if(isdigit(*input2.c_str()))
                {
                    size = atoi(input2.c_str());
                    break;
                }
                else
                {
                    cout << "Please enter a valid number" << endl;
                }
            }
            for(int i = 0; i < size; i++)
            {
                string value;
                cout << "Enter x value #" << i + 1 << ": ";
                while(true)
                {
                    cin >> value;
                    if(isdigit(*value.c_str()))
                    {
                        index.first = atof(value.c_str());
                        break;
                    }
                    else
                    {
                        cout << "Please enter a valid number" << endl;
                    }
                }
                cout << "Enter the functional value at " << index.first << ": ";
                while(true)
                {
                    cin >> value;
                    //if(isdigit(*value.c_str()))
                    //{
                        index.second = atof(value.c_str());
                        break;
                    //}
                    //else
                    //{
                    //    cout << "Please enter a valid number" << endl;
                    //}
                }
                in.push_back(index);
            }
            cout << "Interpolating with Newton's divided difference method\n";
            dividedDifference(in);
            cout << endl;
            cout << "Interpolating with the Lagrangian method" << endl;
            LaGrange(in);
            cout << endl;
        }
        else if(input == 2)
        {
            input_file: ifstream infile;
            string fname;
            while(true)
            {
                cout << "Input file name: ";
                cin >> fname;
                infile.open(fname);
                if(!infile)
                {
                    cout << "Name not valid" << endl;
                }
                else
                {
                    break;
                }
            }
            vector<double> temp1;
            vector<double> temp2;
            string x;
            while ((infile.peek()!='\n') && (infile >> x))
            {
                temp1.push_back(atof(x.c_str()));
            }
            infile.ignore(1000, '\n');
            while ((infile.peek()!='\n') && (infile >> x))
            {
                temp2.push_back(atof(x.c_str()));
            }
            if(temp1.size() != temp2.size())
            {
                cout << "Warning! Numbers in file not in correct format" <<endl;
                goto input_file;
            }
            else
            {
                vector< std::pair<double, double> > in;
                for(unsigned i = 0; i < temp1.size(); i++)
                {
                    in.push_back(std::pair<double, double> {temp1.at(i),
                                                            temp2.at(i)});
                }
                cout << "Divided difference" << endl;
                dividedDifference(in);
                cout << endl;
                cout << "Lagrange's form" << endl;
                LaGrange(in);
                cout << endl;
            }


        }
        else
        {
            cout << "Input not valid" << endl;
        }
    }

//    vector< std::pair<double, double> > in = {{2, 1.5713}, {3, 1.5719}, {5, 1.5738}, {6, 1.5751}};
//    dividedDifference(in);
//    cout << "Doing in2" << endl << endl;
//    vector< std::pair<double, double> > in2 = {{1, 3}, {1.5, 3.25}, {0, 3}, {2, 1.67}};
//    dividedDifference(in2);

//    vector< std::pair<double, double> > in3 = {{.33333333, 2}, {.25, -1}, {1, 7}};
    //LaGrange(in);
    return 0;
}

void dividedDifference(vector< std::pair<double, double> > base)
{
    vector< vector<double> > inception;
    vector<double> f(base.size(), 0);
    vector< std::pair<double, vector<Polynomial> > > terms;

//    cout << "x: ";
//    for(unsigned u = 0; u < base.size(); u++)
//    {
//        cout << base.at(u).first << " ";
//    }
    //cout << endl;
    //cout << "f(x): ";
    for(unsigned u = 0; u < base.size(); u++)
    {
        //cout << base.at(u).second << " ";
        f.at(u) = base.at(u).second;
    }
    //cout << endl;
    terms.push_back(std::pair<double, vector<Polynomial>>{f.at(0), 0});
    inception.push_back(f);

    int count = 0;
    vector<Polynomial> polys;
    //Polynomial p = polys.at(0) * polys.at(1);
    //Generates the arrays of the slopes
    for(unsigned long long u = base.size() - 1; u > 0; u--)
    {
        // Finds the slopes based on the previous values
        vector<double> f(u, 0);
        for(unsigned w = 0; w < u; w++)
        {
            //cout << "In for" << endl;
            double fx1 = inception.at(inception.size() - 1).at(w);
//            cout << "fx1: " << fx1 << endl;
//            cout << "Assigned fx1" << endl;
            double fx2 = inception.at(inception.size() - 1).at(w + 1);
//            cout << "fx2: " << fx2 << endl;
//            cout << "Assigned fx2" << endl;
            double x1 = base.at(w).first;
//            cout << "x1: " << x1 << endl;
//            cout << "Assigned x1" << endl;
            double x2 = base.at(w + inception.size()).first;
//            cout << "x2: " << x2 << endl;
//            cout << "Assigned x2" << endl;
            f.at(w) = (fx2 - fx1)/(x2 - x1);
            //cout << "Slope: " << f.at(w) << endl;
        }
        inception.push_back(f);
        polys.push_back(Polynomial
                        (vector< std::pair<double, unsigned> >
                         {{1, 1}, {-1 * base.at(count).first, 0}}));
        terms.push_back(std::pair<double, vector<Polynomial> >{f.at(0), polys});
        count++;
    }
    unsigned n = 2 * base.size();
    unsigned m = base.size() + 1;
    string output[n][m];

    // Printing the x values and the functional values
    for(unsigned u = 0; u < n; u++)
    {
        //cout << "In first for" << endl;
        if(u % 2 == 0)
        {
            output[u][0] = to_string(base.at(u/2).first).substr(0,6) + "\t";
            output[u][1] = to_string(base.at(u/2).second).substr(0,6) + "\t";
        }
        else
        {
            output[u][0] = "\t";
            output[u][1] = "\t";
        }
    }

    //Printing the divided differences in a clean format
    for(unsigned i = 2; i < m; i++)
    {
        unsigned offset = 0;
//        cout << "In second for" << endl;
//        cout << "i is: " << i << endl;
        for(unsigned j = 0; j < i - 2 + !(inception.at(i - 1).size() % 2); j++)
        {
            //cout << "In tab for" << endl;
            output[j][i] = "\t";
            offset++;
        }
        for(unsigned j = 0; j < 2 * inception.at(i - 1).size(); j++)
        {
            //cout << "In output for" << endl;
            if(j % 2 == inception.at(i - 1).size() % 2)
                output[j + offset][i] = to_string(inception.at(i - 1).at(j/2)).substr(0,6) + "\t";
            else
                output[j + offset][i] = "\t";

        }
    }
    cout << "x\t";
    for(unsigned u = 0; u < base.size(); u++)
    {
        cout << "f[";
        if(u < 5)
        {
            for(unsigned w = 0; w < u; w++)
            {
                cout << ",";
            }
        }
        else
        {
            cout << ",...(" << u << ")";
        }
        cout << "]\t";
    }
    cout << endl;
    for(unsigned i = 0; i < n; i++)
    {
        for(unsigned j = 0; j < m; j++)
        {
            cout << output[i][j];
        }
        cout << endl;
    }

    cout << "Unsimplified:" << endl;
    printUnsimplifiedPolynomial(terms);
    cout << endl;

    cout << "Simplified" << endl;
    printSimplifiedPolynomial(terms);
    cout << endl;
}
void LaGrange(vector< std::pair<double, double> > base)
{
    vector< std::pair<double, vector<Polynomial> > > terms;
    for(unsigned u = 0; u < base.size(); u++)
    {
        double denom = 1;
        std::pair<double, vector<Polynomial>> pair;
        for(unsigned v = 0; v < base.size(); v++)
        {
            //Calculating each term in the polynomial
            if(u != v)
            {
                denom *= (base.at(u).first - base.at(v).first);
                Polynomial p(vector< std::pair<double, unsigned int> >
                             {{1,1},{-1 * base.at(v).first, 0}});
                pair.second.push_back(p);
                //terms.at(u).second.push_back(p);
            }

            //terms.at(u).first = base.at(u).first/denom;
        }
        pair.first = base.at(u).second/denom;
        terms.push_back(pair);
//        cout << "Term: " << terms.at(u).first << " ";
//        for(unsigned v = 0; v < terms.at(u).second.size(); v++)
//        {
//            cout << terms.at(u).second.at(v) << " ";
//        }
//        cout << endl;
    }
    cout << "Unsimplified:" << endl;
    printUnsimplifiedPolynomial(terms);
    cout << endl;
    cout << "Simplified:" << endl;
    printSimplifiedPolynomial(terms);
    cout << endl;
}
void printUnsimplifiedPolynomial
(vector< std::pair<double, vector<Polynomial> > > terms)
{
    if(terms.at(0).first < 0)
        cout << " - ";
    for(unsigned i = 0; i < terms.size() - 1; i++)
    {
        if(terms.at(i).first != 0)
        {
            cout << abs(terms.at(i).first);
            for(unsigned j = 0; j < terms.at(i).second.size(); j++)
            {
                cout << "(" << terms.at(i).second.at(j) << ")";
            }
            if(terms.at(i + 1).first > 0)
                cout << " + ";
            else if(terms.at(i + 1).first < 0)
                cout << " - ";
        }
    }
    if(terms.at(terms.size() - 1).first != 0)
    {
        cout << abs(terms.at(terms.size() - 1).first);
        for(unsigned j = 0; j < terms.at(terms.size() - 1).second.size(); j++)
        {
            cout << "(" << terms.at(terms.size() - 1).second.at(j) << ")";
        }
    }
}

void printSimplifiedPolynomial
(vector< std::pair<double, vector<Polynomial> > > terms)
{
    Polynomial p(terms.at(0).first);
    if(terms.at(0).second.size() == 0)
    {
        Polynomial p(terms.at(0).first);
    }
    else
    {
        p = terms.at(0).second.at(0);
        for(unsigned u = 1; u < terms.at(0).second.size(); u++)
        {
            Polynomial temp(terms.at(0).second.at(u));
            p *= terms.at(0).second.at(u);
        }
        p = p.scale(terms.at(0).first);
    }

//    cout << p << endl;
    vector<Polynomial> polys;
    polys.push_back(p);
    for(unsigned u = 1; u < terms.size(); u++)
    {
        Polynomial p2 = terms.at(u).second.at(0);
//        cout << "p2: " << p2 << endl;
        //cout << "scale: " << terms.at(u).first << endl;
        for(unsigned v = 1; v < terms.at(u).second.size(); v++)
        {
            //cout << "p2: " << p2 << endl;
            //cout << "Next: " << terms.at(u).second.at(v) << endl;
            p2 *= terms.at(u).second.at(v);
            //cout << "p2 before scaling: " << p2 << endl;

        }
        p2 = p2.scale(terms.at(u).first);
        //cout << "New p2: " << p2 << endl;
        polys.push_back(p2);
        p = p + p2;
        //cout << "p: " << p << endl;

    }
    Polynomial final(0);
    for(unsigned u = 0; u < polys.size(); u++)
    {
        final = final + polys.at(u);
        //cout << polys.at(u) << endl;
    }
    cout << final << endl;

}

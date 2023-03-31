#include"../../../shujujiegou/Vector/Vector.h"
#include"../hw1/complex/Complex.h"

#include<iostream>
#include<cstdlib>

using namespace std;
int main()
{
	srand((unsigned)time(NULL));
    Complex a[10];
    Vector<Complex> vec(a, 10);

    for (int j = 0; j < 10; j++)
    {
        a[j].real = rand() % 200;
        a[j].image = rand() % 200;
        a[j].Print();
        a[j]._modulus();
    }
    vec.unsort(0, 10);
    cout<<"²éÕÒ£º"<<vec.find(a[6]) << endl;

    Complex x;
    x.real = rand() % 200;
    x.image = rand() % 200;
    vec.insert(5, x);
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        cout << it << " ";  
    }  
    cout << endl;
    vec.remove(5);
}

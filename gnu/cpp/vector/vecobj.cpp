#include <iostream>
#include <vector>
using namespace std;

class obj
{
    public :
	obj() { cout << ">> create obj " << endl; }
	obj(const obj& other) { cout << ">> copy create obj " << endl; }
};

vector<obj> foo()
{
    vector<obj> c;
    cout << "---- 1, capacity: " << c.capacity() << "---" << endl;
    c.push_back(obj());

    cout << "---- 2, capacity: " << c.capacity() << "---" << endl;
    c.push_back(obj());

    cout << "---- 3, capacity: " << c.capacity() << "---" << endl;
    c.push_back(obj());

    cout << "---- 4, capacity: " << c.capacity() << "---" << endl;
    c.push_back(obj());

    cout << "---- 5, capacity: " << c.capacity() << "---" << endl;
    c.push_back(obj());

    cout << "---- 6, capacity: " << c.capacity() << "---" << endl;
    c.push_back(obj());


    cout << "---- exit foo ----" << endl;
    return c;
}

int main()
{
    vector<obj> k;
    k = foo();
}

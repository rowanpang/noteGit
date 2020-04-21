#include <iostream>
#include <vector>
using namespace std;

class obj
{
    public :
	obj() { cout << ">> create obj " << endl; }
	obj(const obj& other) { cout << ">> copy create obj " << endl; }
};

template <class T>
class container
{
    public:
	T* value;

    public:
	container() : value(NULL) {};
	~container() { delete value; }

	container(const container& other)
	{
	    value = new T(*other.value);
	}

	const container& operator = (const container& other)
	{
	    delete value;
	    value = new T(*other.value);
	    return *this;
	}

	//const container& operator = (container& other) lvalue not const error
	const container& operator = (container&& other)
	{
	    /*
	     *T* tmp = value;
	     *value = other.value;
	     *other.value = tmp;
	     */

	    value = other.value;
	    other.value = NULL;
	    return *this;
	}

	void push_back(const T& item)
	{
	    delete value;
	    value = new T(item);
	}
};

container<obj> foo()
{
    container<obj> c;
    c.push_back(obj());

    cout << "---- exit foo ----" << endl;
    return c;
}

int main()
{
    container<obj> k ;
    k = foo();
}

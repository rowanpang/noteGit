#include <iostream>
#include <vector>
#include <list>
using namespace std;

class obj
{
    public :
	obj() { cout << ">> create obj " << endl; }
	obj(const obj& other) { cout << ">> copy create obj " << endl; }
};


class bigobj
{
    public :
	bigobj() { cout << ">> create bobj " << endl; }
	bigobj(const bigobj& other) { cout << ">> copy create bobj " << endl; }
	bigobj(bigobj&& other) { cout << ">> move create bobj " << endl; }
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
	    T* tmp = value;
	    value = other.value;
	    other.value = tmp;
	    return *this;
	}

	container (container && other){
	    value = other.value;
	    other.value = NULL;
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
    int i = 0;
    container<obj> k ;
    k = foo();

    list<bigobj> list;
    for (i = 0; i < 3; i++){
	bigobj bobj;
	list.push_back(bobj);
    }

}

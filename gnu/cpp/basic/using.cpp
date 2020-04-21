#include <iostream>
#include <string>

/*
 *1）配合命名空间，对命名空间权限进行管理
 *2）类型重命名
 *3）继承体系中，改变部分接口的继承权限。
 */

using namespace std;
#define DString std::string    //! 不建议使用！
typedef std::string TString;   //! 使用typedef的方式
using Ustring = std::string;   //！使用 using typeName_self = stdtypename;

//更直观
typedef void (*tFunc)(void);
using uFunc = void(*)(void);

void func(void)
{
    cout << "infunc func" << endl;
}


class Base
{
public:
    Base()
    {}
    ~Base(){}

    void dis1()
    {
        cout<<"dis1"<<endl;
    }
    void dis2()
    {
        cout<<"dis2"<<endl;
    }
};

class BaseA:private Base
{
public:
    using Base::dis1;//需要在BaseA的public下释放才能对外使用，
    void dis2show()
    {
        this->dis2();
    }
};

int main(int argc, char *argv[])
{

    TString ts("String!");
    Ustring us("Ustring!");
    string s("sdfdfsd");

    cout<<ts<<endl;
    cout<<us<<endl;
    cout<<s<<endl;

    tFunc tf = func;
    tf();
    tf;		//编译不报错,但没有运行.

    cout << "-------" << endl;
    uFunc uf = func;
    uf();
    uf;		//编译不报错,但没有运行.

    BaseA ba;
    ba.dis1();	    //虽然private继承,但使用using public了.
    ba.dis2show();

    return 0;
}

#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

class A{

    public:
	template <typename T> T funcA(T a){
	    return a;
	}

	int a;
};

template <typename T1,typename T2> class TA{
    public:
	T1 funcA(T1 a,T2 b);

};

/*
 *在类模板外部定义成员函数的方法为：
 *template<模板形参列表> 函数返回类型 类名<模板形参名>::函数名(参数列表){函数体}
 */

//如果不定义模板函数而仅仅定义实例化函数,则类型不匹配的函数将编译报错.
template<class T1,class T2> T1 TA<T1,T2>::funcA(T1 a,T2 b){
   return a+b;
}

template<> int TA<int,int>::funcA(int a,int b){
    return a+b;
}


int main()
{
    A a;
    cout << a.funcA(3) << endl;
    cout << a.funcA(3.3) << endl;

    TA<int,int> tinta;
    cout << tinta.funcA(3,3) << endl;
    cout << tinta.funcA(3.3,3.3) << endl;

    TA<char,char> tchara;
    cout << tchara.funcA(30,35) << endl;	    //因为返回值是char类型,所以显示A
    cout << tchara.funcA(30,36) << endl;	    //B

    TA<int,char> tichara;
    cout << tichara.funcA(3,3) << endl;
    cout << tichara.funcA(3.3,3.3) << endl;
    return 0;
}

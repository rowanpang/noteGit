#include <typeinfo>
#include <iostream>

using namespace std;

int main(){
    int a = 10;
    auto au_a = a;//自动类型推断，au_a为int类型

    std::cout << typeid(au_a).name() << std::endl;



    auto a4 = 10, a5 = 20, a6 = 30;//正确
    //auto b4 = 10, b5 = 20.0, b6 = 'a';//错误,没有推导为同一类型

    /*
     *如果初始化表达式是引用，则去除引用语义
     */
    int &b = a;

    auto c = b;	    //c的类型为int而非int&（去除引用）
    std::cout << typeid(c).name() << std::endl;
    auto &d = b;    //此时d的类型才为int&
    std::cout << typeid(d).name() << std::endl;

    c = 100;	    //a =10;
    d = 80;	    //a =100;

    std::cout << c << " , " << d << " , " << a << endl;


    /*
     *如果初始化表达式为const或volatile（或者两者兼有），则除去const/volatile语义
     */
    const int a1 = 10;
    auto  b1= a1;	    //b1的类型为int而非const int（去除const）
    b1 = 100;	//合法

    /*
     *const auto c1 = a1;	    //此时c1的类型为const int
     *c1 = 100;	//非法
     */


    /*
     *初始化表达式为数组时，auto关键字推导类型为指针。
     */

    int a3[3] = { 1, 2, 3 };
    auto b3 = a3;
    cout << typeid(b3).name() << endl;




    /*
     *若表达式为数组且auto带上&，则推导类型为数组类型。
     *
     */
    int a7[3] = { 1, 2, 3 };
    auto & b7 = a7;
    cout << typeid(b7).name() << endl;

    /*
     *程序输出
     *
     *    int [3]
     *
     *函数或者模板参数不能被声明为auto
     *
     *void func(auto a)  //错误
     *{
     *	...
     *}
     *
     *时刻要注意auto并不是一个真正的类型。
     *auto仅仅是一个占位符，它并不是一个真正的类型，不能使用一些以类型为操作数的操作符，如sizeof或者typeid。
     *
     *cout << sizeof(auto) << endl;//错误
     *cout << typeid(auto).name() << endl;//错误
     *
     *
     */

    /*
     *https://www.cnblogs.com/KunLunSu/p/7861330.html
     */

    return 0;
}

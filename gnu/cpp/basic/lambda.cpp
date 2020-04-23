#include <typeinfo>
#include <iostream>
#include <list>
#include <string>

#include <stdio.h>
#include <functional>

using namespace std;

int funcA(const char* msg,int a)
{
    printf("msg: %s, int: %d\n",msg,a);
}

/*
 *lambda
 *    [capture] (parameters) mutable ->return-type {statement}
 *
 *    []	    // 沒有定义任何变量。使用未定义变量会引发错误
 *    [x, &y] // x以传值方式传入（默认），y以引用方式传入
 *    [&]	    // 任何被使用到的外部变量都隐式地以引用方式加以引用
 *    [=]	    // 任何被使用到的外部变量都隐式地以传值方式加以引用
 *    [&, x]  // x显式地以传值方式加以引用。其余变量以引用方式加以引用
 *    [=, &z] // z显式地以引用方式加以引用。其余变量以传值方式加以引用
 *
 *
 *ref
 *    https://blog.csdn.net/lixiaogang_theanswer/article/details/80905445
 *    https://www.cnblogs.com/jimodetiantang/p/9016826.html
 *    https://www.jianshu.com/p/d4c0702d99d0
 */

int main(){
    int a = 10;
    const char* ctmp = string("aaaaaaa").data();

    std::list<int> li;
    li.push_back(10);
    li.push_back(11);

    std::function<int(int,char)> funB;
    /*
     *funB = [](int a,char b) -> int{
     *只有一个return表达式,返回类型可忽略,由编译器推导.
     */
    funB = [](int a,char b) {
	return 3;
    };
    cout << funB(3,'3') << endl;

    /*
     *auto func = [ ] (const char* msg, int a) {
     *lambda func use li,so li must be in capture list
     */
    auto func = [&li](const char* msg, int a) {
	if (a == 10){
	    printf("msg: %s, int: %d\n",msg,a);
	    return 1;
	}else if (a < 8){
	    printf("msg<8: %s, int: %d\n",msg,a);
	    //return "c";l  //lambda auto 推导返回值类型,所有return 返回值类型要相同.
	    return 2;
	}

	printf("msg-else: %s, int: %d\n",msg,a);
	li.push_back(90);
	//return 3;
    };
    cout << func(ctmp,10) << endl;
    cout << func(ctmp,7) << endl;
    cout << func(ctmp,9) << endl;   //9,没有具体的return 表达式,返回值错乱.


    funcA(ctmp, 10);
    func(string("bbbbbbbb").c_str(), 10);

    for (auto l : li){
	cout << l << endl;
    }


    return 0;
}

#include <iostream>
#include <string>

using namespace std;

//template <typename T> void swapr(T& a, T& b){}; same as
template <class T> void swapr(T& a, T& b){};

/*
 *显式实例化
 */
template void swapr<int>(int &a,int &b);

template<typename T1,typename T2>	    //模板类 定义
class Test{				    //Test后面不跟<类型>.
    public:
	Test(T1 i,T2 j):a(i),b(j){cout<<"模板类"<<endl;}
    private:
	T1 a;
	T2 b;
};

template<>	//模板类全特化，由于是全特化，参数都指定了，参数列表故为空。
class Test<int ,char>{
    public:
	Test(int i,char j):a(i),b(j){cout<<"全特化"<<endl;}
    private:
	int a;
	int b;
};

template<typename T2> //部分特化,由于只指定了一部分参数,剩下的未指定的需在参数列表中,否则报错
class Test<char,T2>{
    public:
	Test(char i,T2 j):a(i),b(j){cout<<"个数偏特化"<<endl;}
    private:
	char a;
	T2 b;
};

template<typename T1,typename T2> //这是范围上的偏特化
class Test<T1*,T2*>{
    public:
	Test(T1* i,T2* j):a(i),b(j){cout<<"指针偏特化"<<endl;}
    private:
	T1* a;
	T2* b;
};

template<typename T1,typename T2>//同理这也是范围上的偏特化
class Test<T1 const,T2 const>{
    public:
	Test(T1 i,T2 j):a(i),b(j){cout<<"const偏特化"<<endl;}
    private:
	T1 a;
	T2 b;
};

//模板函数 模板函数定义
template<typename T1,typename T2>
void fun(T1 a,T2 b){
    cout<<"模板函数"<<endl;
}
//全特化

template<> void fun(int a,char b){
    cout<<"int,char 全特化"<<endl;
}

template<> void fun(char a,int b){
    cout<<"char,int 全特化"<<endl;
}

template<> void fun(int a,int b){
    cout<<"int,int 全特化"<<endl;
}

template<> void fun(char a,char b){
    cout<<"char,char 全特化"<<endl;
}
/*
 *函数不存在偏特化，以下代码是错误的
 *
 *template<typename T2>
 *void fun(char a,T2 b){
 *    cout<<"偏特化"<<ednl;
 *}
 *
 */
int main()
{
    int a=8,b=5;
    std::string stra="stra", strb="strb";

    /*
     *隐士实例化,在运行时生成实例
     */
    swapr<int>(a,b);
    swapr<string>(stra,strb);

    Test<double,double> t1(0.1,0.2);
    Test<int,char> t2(1,'A');
    Test<char,bool> t3('A',true);
    Test<int*,int*> t4(&a,&a);
    Test<const int,const int> t5(1,2);


    int inta=3;
    int intb=4;
    char chara='a';
    char charb=5;
    fun(a,b);
    fun(1.1,3.2);

    fun(inta,chara);	//这两个都是全特化,只于类型有关.
    fun(intb,charb);
    fun(chara,charb);
    fun(chara,inta);

    return 0;
}

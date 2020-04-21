#include <iostream>
#include <vector>

#include <iostream>
#include <type_traits>

using namespace std;

class A {};

enum E : int {};

template <class T>
T f(T i)
{
    static_assert(std::is_integral<T>::value, "Integral required.");
    return i;
}

template <int n> struct factorial3 : std::integral_constant<int,n> {
    public:
	int a;
	void show(){
	    cout << "show: " << factorial3::value << endl;
	}
};
template <unsigned n> struct factorial : std::integral_constant<int,n * factorial<n-1>::value> {};
template <> struct factorial<0> : std::integral_constant<int,1> {};

template <class T> struct myjudge : std::integral_constant<bool,1> {};

//enable_if
//ref https://blog.csdn.net/catwan/article/details/93482648

// 1. the return type (bool) is only valid if T is an integral type:
// 需要显示使用typename 指明::type是类型. ref
// https://blog.csdn.net/andyjim/article/details/21019109
template <class T>
typename std::enable_if<std::is_integral<T>::value,bool>::type is_odd (T i) {
    return bool(i%2);
}

// 2. the second template argument is only valid if T is an integral type:
//template < class T, class = typename std::enable_if<std::is_integral<T>::value>::type>
//template < typename T, typename T2 = typename std::enable_if<std::is_integral<T>::value>::type >
//template < typename T, typename std::enable_if<std::is_integral<T>::value,int>::type T2 = 0 >
//enable_if 应用了SFINAE(Substitution Failure Is Not An Error)机制,如果最终找不到最佳匹配则报错.
//即无法实例此偏特化
//same as
template < typename T, typename std::enable_if<std::is_integral<T>::value,int>::type = 0 >
bool is_even (T i) {
    return !bool(i%2);
}

template < typename T, typename std::enable_if<! std::is_integral<T>::value,int>::type = 0 >
bool is_even (T i) {
    return (i == "c");
}

int main()
{
    int i = 1;

    std::cout << std::boolalpha;
    std::cout << std::is_integral<A>::value << '\n';
    std::cout << std::is_integral<E>::value << '\n';
    std::cout << std::is_integral<float>::value << '\n';
    std::cout << std::is_integral<int>::value << '\n';
    std::cout << std::is_integral<int>::value<< '\n';
    std::cout << std::is_integral<bool>::value << '\n';
    std::cout << f(123) << '\n';

    cout << factorial<3>::value << endl;
    cout << factorial<3>::value << endl;

    factorial3<2> fac3;
    cout << fac3.value << endl;
    fac3.show();
    cout << factorial<5>::value << endl;

    std::cout << std::boolalpha << endl;
    std::cout << "i is odd: " << is_odd(i) << std::endl;
    std::cout << "i is even: " << is_even(i) << std::endl;
    std::cout << "i is even: " << is_even('b') << std::endl;
    std::cout << "i is even: " << is_even('c') << std::endl;
    std::cout << "i is even: " << is_even("b") << std::endl;
    std::cout << "i is even: " << is_even("c") << std::endl;

    return 0;
}


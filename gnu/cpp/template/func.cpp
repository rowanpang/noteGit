#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;
//泛型版本
template <typename T> int compare(const T &v1, const T &v2)
{
  std::cout << "template <typename T>" << std::endl;
  if(v1 < v2) return -1;
  if(v2 < v1) return 1;
  return 0;
}

//为实参类型 const char * 提供特化版本
//template <> int compare(const char * const &v1, const char * const &v2) //省略了函数名后边的显示模板实参，因为可以从函数形参表推断出来，本定义与下边的定义都是正确的；
template <> int compare<const char *>(const char * const &v1, const char * const &v2)
{
  std::cout << "template <> int compare<const char *>" << std::endl;
  return strcmp(v1, v2);
}

//为实参类型 char * 提供特化版本
//template <> int compare(char * const &v1, char * const &v2)
template <> int compare<char *>(char * const &v1, char * const &v2)
{
  std::cout << "template <> int compare<char *>" << std::endl;
  return strcmp(v1, v2);
}

int main()
{
  cout << compare(1, 2) << endl;  //根据实参类型进行实参推断，将为该调用实例化int compare(int, int)

  char a[] = {"abc"}; //一个普通字符数组，不是指针，形参为引用时，数组大小成为形参的一部分，数组不转换为指针，类型不匹配；
  const char b[] = {"abc"}; //一个常量字符数组，不是指针，类型不匹配；
  //char *p = "ddd";
    //一个非const指针，指向非const数据，但是特化版本的形参类型是一个指向const数据的const引用，强调了指针指向的数据类型是const，也就是说实参指针指向的数据类型必须是const即指针存储的数据必须是const的，但这里不是因此类型不匹配；
  //char * const pc = "ddd"; //一个const指针，指向非const数据，类型不匹配，原因同上，和指针是否是const没关系，和指针存储的数据类型有关；
  //const char * const pc = "ddd"; //一个const指针，指向const数据，满足特化版本的形参（一指向const数据的const引用），类型匹配；
  //const char * pc = "ddd"; //一个非const指针，指向const数据，类型匹配，原因同上；

  //为实参类型 const char * 提供特化版本
  const char *pa = "abc"; // 或者 const char * const pa = "abc"; 与指针指向数据类型是const还是非const有关，而与指针是const还是非const没关系，因为，特化版本的形参类型是一个指向指针的cosnt引用，因此不会改变指针的值，所以指针本身是const还是非cosnt没有关系，但是由于特化版本形参的引用指向的指针所指向的数据类型是const，所以不能使用一个指向非const数据的指针调用特化版本，因为数据类型不匹配；
  const char *pb = "bbd";
  cout << compare(pa, pb) << endl; // 根据实参类型为该调用实例化特化版本int compare(const * const &v1, const * const &v2), 函数模板调用时的实参与模板形参不进行常规转换；由于编译器对特化版本不进行实参与形参的常规转换，所以调用的实参类型必须与特化版本的声明完全一致，否则将从泛型版本进行实例化，或者函数匹配错误；由于compare声明的形参都是const char *即char *型指针存储的是const数据，所以不能传递一个存储了非const数据的char *型指针（尽管此时的cosnt char *形参不会改变实参指针指向的值），也不能传递一个const数组名字（此时数组名不会转换为指针），必须传递一个指向const数据的指针，即代码中的 const char *pa，类型必须完全匹配；

  //为实参类型 char * 提供特化版本
  //char *pc = "ccc";
  //char *pd = "ddd";
  //cout << compare(pc, pd) << endl;
  return 0;

  //char * 与 const char * 是两个不同的数据类型(前者存储的数据是常量与后者存储的数据是非常量)，虽然可以将类型 char * 通过常规转换，转换成 const char *，但是作为模板实参，在模板实参推断时，不会把函数调用时的实参类型 char * 转换为模板形参类型const char *，所以必须提供两个特化版本。
}

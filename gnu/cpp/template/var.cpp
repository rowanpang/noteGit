#include <iostream>
using namespace std;

//template <class T, int N>
//It is also possible to set default values or types for class template parameters. For example, if the previous class template definition had been:
//ref http://www.cplusplus.com/doc/oldtutorial/templates/
template <class T=char, int N=10>
class mysequence {
    T memblock [N];
  public:
    void setmember (int x, T value);
    T getmember (int x);
};

template <class T, int N>
void mysequence<T,N>::setmember (int x, T value) {
  memblock[x]=value;
}

template <class T, int N>
T mysequence<T,N>::getmember (int x) {
  return memblock[x];
}

int main () {
  mysequence<int,5> myints;
  mysequence<double,5> myfloats;
  myints.setmember (0,100);
  myfloats.setmember (3,3.1416);
  cout << myints.getmember(0) << '\n';
  cout << myfloats.getmember(3) << '\n';

  mysequence<> myseq;
  myseq.setmember(0,'c');
  cout << myseq.getmember(0) << '\n';

  return 0;
}

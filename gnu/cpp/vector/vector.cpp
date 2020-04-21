#include <iostream>
#include <vector>

int main ()
{
    std::vector<int>::size_type sz;

    std::vector<int> foo;
    sz = foo.capacity();    //size unit is vector type. here is <int>
    std::cout << "making foo grow:\n";
    for (int i=0; i<100; ++i) {
	foo.push_back(i);
	if (sz!=foo.capacity()) {
	    sz = foo.capacity();
	    std::cout << "capacity changed:" << sz << " ,i:" << i << '\n';
	}
	/*
	 *std::cout << "foo.size():" << foo.size() << '\n';
	 */
    }

    std::vector<int> bar;
    sz = bar.capacity();
    std::cout << "making bar grow: " << sz << '\n';
    bar.reserve(100);   // this is the only difference with foo above
    std::cout << "bar.reserve(100)->bar.capacity: " << bar.capacity() << '\n';
    int i ;
    int *pi;
    for (i=0; i<10; ++i) {
	bar.push_back(i);
	if (sz!=bar.capacity()) {
	    sz = bar.capacity();
	    std::cout << "capacity changed:" << sz << " i:" << i << '\n';
	}

	/*
	 *std::cout << "bar.capacity: " << bar.capacity() << '\n';
	 *std::cout << "bar.size():" << bar.size() << '\n';
	 */
    }

    pi = bar.data();
    std::cout << "pi:" << pi << " ,pi[3]:" << *(pi+3) << '\n';

    std::cout << "bar.size(): " << bar.size() << " bar.capacity(): " << bar.capacity() << '\n';
    std::vector<int> empty;
    empty.swap(bar);
    std::cout << "after empty.swap(bar),bar: " << bar.size() << " empty: " << empty.size() << '\n';

    std::cout << "finished! i now: " << i << '\n';
    return 0;
}

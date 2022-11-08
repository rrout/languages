
#include "hdr.h"
#include "sm.h"
template <class T>
class test {
private:
	T _value;
public:
	T sq(T val)  {
		T v = val+val;
		std::cout << "Sq Val : " << v << std::endl;
		return v;
	}
	std::function<T(test<T>&,T)> sqFunc = &test::sq;
};

int main() {
	test<int> t;
	t.sq(20);
	t.sqFunc(t,20);
	test<std::string> s;
	s.sqFunc(s, "rashmiiiiii");
	//FSM Object
	fsm fm;
	fm.printStateTable();
	fm.fsmTest();
}

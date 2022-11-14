#include "brg.h"

int main() {
	Veichle *p;
	p = new car("SUV", "SILVER");
	p->buildVeichle();
	p->display();
	delete p;
	p = new truck("SEDAN", "METALIC");
    p->buildVeichle();
    p->display();
	delete p;
}

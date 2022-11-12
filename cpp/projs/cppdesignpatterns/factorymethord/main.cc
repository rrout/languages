#include "fm.h"

int main() {
	//Veichle *p = Veichle::getObj(MOPED);
	//Veichle *p = new moped();
	//std::unique_ptr<Veichle> p = std::make_unique<moped>();
	std::unique_ptr<Veichle> p(Veichle::CreateVeichle(MOPED));
	p->printVichle();
	std::unique_ptr<Veichle> q(Veichle::CreateVeichle(SUV));
	p = std::move(q);
	p->printVichle();
	if(q)
		q->printVichle();

	VeichleClient v(SEDAN);
	v.print();
	VeichleClient y(HATCHBACK);
	y.print();

}

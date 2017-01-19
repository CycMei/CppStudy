#include"SimpleFactory.h"
#include"Factory.h"

void test1() {
	SIMPLEFACTORY::SimpleFactory sf;
	SIMPLEFACTORY::PizzaStore ps(sf);
	ps.orderPizza("cyc");
}

void test2() {
	FACTORY::PizzaStore *ps = new FACTORY::PizzaStore1();
	FACTORY::Pizza pz=ps->orderPizza("cyc");
	delete ps;
	ps = nullptr;
}

void main() {
	test1();
}
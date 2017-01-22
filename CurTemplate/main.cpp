#include"CCounted.h"
#include"CCountedTemplate.h"

void test(){

	CCounted a;
	std::cout<<CCounted::getCount()<<std::endl;
	CCounted b;
	std::cout<<CCounted::getCount()<<std::endl;
	CCounted c;
	std::cout<<CCounted::getCount()<<std::endl;
	{
		CCounted d(c);
		std::cout<<CCounted::getCount()<<std::endl;
		a=d;
		std::cout<<CCounted::getCount()<<std::endl;
	}
	std::cout<<CCounted::getCount()<<std::endl;

}

void test2(){

	CCount1 a;
	std::cout<<CCount1::getCount()<<std::endl;
	CCount1 b;
	std::cout<<CCount1::getCount()<<std::endl;
	CCount2 c;
	std::cout<<CCount2::getCount()<<std::endl;

}

void test3(){

	C1 a;
	std::cout<<C1::getCount()<<std::endl;
	C1 b;
	std::cout<<C1::getCount()<<std::endl;
	C2 c;
	std::cout<<C2::getCount()<<std::endl;

}

int main(){

	//test();
	//test2();
	test3();
	return 0;

}

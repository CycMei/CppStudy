#include"Singleton.h"

#include<iostream>

void test() {
	SINGLETON::Cyc cyc1;
	cyc1.Instance();
	SINGLETON::Cyc cyc2;
	cyc2.Instance();
	if (cyc1.m_pInstance==cyc2.m_pInstance) {
		std::cout << ".....cyc1=====cyc2...." << std::endl;
	}

}

void main() {
	test();
}
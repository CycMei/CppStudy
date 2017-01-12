#include"Singleton.h"

#include<iostream>

void test() {
	SINGLETON::Cyc::Instance();
}

int main() {
	test();
}
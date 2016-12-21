#include<iomanip>
#include<bitset>
#include<iostream>
#include<string>

namespace Bitset{
	void test1(){
		std::cout<<"first bitset...:"<<std::endl;
		std::bitset<32> bitvec(10);

		std::bitset<13> bitvec1(0xbeef);
		std::cout<<std::setbase(2)<<bitvec1<<std::endl;

		std::bitset<20> bitver2(0xbeef);
		std::cout<<std::setbase(2)<<bitver2<<std::endl;

		std::bitset<128> bitvec3(~0ULL);
		std::cout<<std::setbase(2)<<bitvec3<<std::endl;	
	}
}


void BitsetTest1(){
	Bitset::test1();
}

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

		std::bitset<32> bitvec4("1100");
		std::cout<<std::setbase(2)<<bitvec4<<std::endl;

		std::string str("1111111000000011001101");
		std::bitset<32> bitvec5(str,5,4,'0','1');
		std::cout<<std::setbase(2)<<bitvec5<<std::endl;
		std::bitset<32> bitvec6(str,str.size()-4);
		std::cout<<std::setbase(2)<<bitvec6<<std::endl;
		std::cout<<"bitvec6->1.."<<bitvec6[1]<<std::endl;

		std::string str1("22223333");
		std::bitset<32> bitvec7(std::string("3333"),0,2,'2','3');
		std::cout<<bitvec7[0]<<std::endl;


	}


	void test2(){
		std::bitset<32> bitvec(1U);
		bool is_set=bitvec.any();
		std::cout<<std::boolalpha<<is_set<<std::endl;
		std::cout<<std::noboolalpha<<true<<std::endl;	
	
		bool is_allSet=bitvec.all();
		std::cout<<std::boolalpha<<is_allSet<<std::endl;

		bool is_not_set=bitvec.none();
		std::cout<<std::boolalpha<<"is_not_set...."<<is_not_set<<std::endl;

		std::size_t onBits=bitvec.count();
		std::cout<<std::noboolalpha<<"onBits...."<<onBits<<std::endl;

		std::size_t sz=bitvec.size();
		std::cout<<"bitvec ..size..:  "<<sz<<std::endl;

		bool bitPos=bitvec.test(0);
		std::cout<<std::boolalpha<<"bitPos....:  "<<bitPos<<std::endl;
		
		bitvec.set(1);
		bitvec.set(0,false);
		std::cout<<std::noboolalpha<<"bitvec..........  "<<bitvec<<std::endl;

		bitvec.set();
		std::cout<<"bitvec all set....   "<<bitvec<<std::endl;

		bitvec.reset(0);
		std::cout<<"bitvec all set....   "<<bitvec<<std::endl;
		
		bitvec.reset();
		std::cout<<"bitvec all set....   "<<bitvec<<std::endl;
		
		bitvec.flip(0);
		std::cout<<"bitvec all set....   "<<bitvec<<std::endl;
		bitvec.flip();
		std::cout<<"bitvec all set....   "<<bitvec<<std::endl;
		
		const std::bitset<32> cBitset(1U);
		std::cout<<"cBitset pos 0..: "<<std::boolalpha<<cBitset[0]<<std::endl;
		std::cout<<"cBitset pos 1..: "<<std::boolalpha<<cBitset[1]<<std::endl;

		std::cout<<"bitvec .pos 1:  "<<bitvec[1]<<std::endl;
		std::cout<<"bitvec .pos 0:  "<<bitvec[0]<<std::endl;
		
		unsigned long ulbitvec=bitvec.to_ulong();
		std::cout<<"unsigned long....  "<<std::setbase(2)<<ulbitvec<<std::endl;
		std::cout<<"unsigned long....  "<<std::setbase(2)<<10<<std::endl;
		
		std::string sreult=bitvec.to_string();
		std::cout<<std::noboolalpha<<"to_string....:  "<<sreult<<std::endl;
	}
}


void BitsetTest1(){
	Bitset::test1();
}

void BitsetTest2(){
	Bitset::test2();
}

#include<tuple>
#include<string>
#include<vector>
#include<list>
#include<iostream>

#include"UNUSED.h"

namespace Tuple{

	class mTyple{
		mTyple()=delete;
	};

	void test1(){
		std::cout<<"tuple::test1......   "<<std::endl;
		std::tuple<std::size_t,std::size_t,std::size_t> threeD;
		std::tuple<std::string,std::vector<double>,int,std::list<int>> someVal("constants",{3.14,2.781},42,{0,1,2,3});
		// std::tuple<mTyple> mty;

		
		std::tuple<const char*,int,double> item=std::make_tuple("cyc",3,2.0);
		auto UNUSED(book) =std::get<0>(item);
		book="lym";

		std::size_t sz=std::tuple_size<std::tuple<const char*,int,double>>::value;
		std::cout<<"item is size....: "<<sz<<std::endl;

		std::tuple_element<1,std::tuple<const char*,int,double>>::type UNUSED(cnt)=std::get<1>(item);
	}
}

void TypleTest1(){
	Tuple::test1();
}
#include<tuple>
#include<string>
#include<vector>
#include<list>
#include<utility>
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
		// book="lym";

		std::size_t sz=std::tuple_size<std::tuple<const char*,int,double>>::value;
		std::cout<<"item is size....: "<<sz<<std::endl;

		std::tuple_element<1,std::tuple<const char*,int,double>>::type UNUSED(cnt)=std::get<1>(item);


		std::tuple<int,double> id_tuple{2,2.2};
		std::tuple<std::string,int> di_tuple(std::string("cyc"),2);
		// if(id_tuple==di_tuple){
		// 	std::cout<<"id_tuple==di_tuple......"<<std::endl;
		// }
		std::tuple<int,double,int> idi_tuple(2,2.2,2);
		// if(id_tuple==idi_tuple){
		// 	std::cout<<"id_tuple==idi_tuple......"<<std::endl;
		// }
		std::tuple<int,double> cid_tuple(2,2.2);
		if(id_tuple==cid_tuple)
			std::cout<<"id_tuple==cid_tuple...."<<std::endl;

		std::tuple<int,int,int> iii_tuple(10,20,30);

		std::tuple<std::string,std::vector<std::string>,std::pair<std::string,int>> svp_tuple{
			std::string("cyc"),
			{std::string("cyc")},
			std::make_pair("cyc",520)
		};


	}
}

void TypleTest1(){
	Tuple::test1();
}
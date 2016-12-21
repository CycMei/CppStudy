#ifndef REQUIRE_H
#define REQUIRE_H

#include<iostream>
#include<exception>
#include<string>
void require(bool bl,const std::string &str=""){
	try{
		if(bl){
			std::logic_error erro(str);
			throw erro;
		}
	}
	catch(const std::logic_error &erro){
		std::cout<<erro.what()<<std::endl;
	}
}
#endif

#include"IntStack.h"
#include"StackTemplate.h"

void test(){
	IntStack is;
	for(int i=0;i<20;++i){
		is.push(fibonacci(i));	
	}
	for(int k=0;k<20;++k){
		std::cout<<is.pop()<<std::endl;
	}
}

void ArrayTest();

void test1(){
	void ArrayTest();
}

void test2(){
	StackTemplate<int> is;
	for(int i=0;i!=20;++i){
		is.push(fibonacci(i));
	}

	for(int k=0;k!=20;++k){
		std::cout<<is.pop()<<std::endl;	
	}
}


int main(){
	test2();

	return 0;
}

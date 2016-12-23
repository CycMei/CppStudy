#include"IntStack.h"

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
	
	test();

	void ArrayTest();

}

int main(){
	test1();
	return 0;
}

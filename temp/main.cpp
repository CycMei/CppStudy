#include"IntStack.h"

void test(){
	IntStack is;
	for(int i=0;i!=20;++i){
		is.push(fibonacci(i));	
	}
}

int main(){
	test();	

	return 0;
}

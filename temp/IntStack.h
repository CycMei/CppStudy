#ifndef INTSTACK_H
#define INTSTACK_H

#include"fibonacci.h"
#include<iostream>

void require(bool,const std::string&);
class IntStack{
private:
	enum {ssize=100};
	int stack[ssize];
	int top;
public:
	IntStack():top(0){}
	void push(int i){
		require(top>=ssize,"too many push()es........");
		stack[top++]=i;
	}
	
	int pop(){
		require(top<0,"too many pos()s............");
		return stack[top--];
	}
};

#endif

#ifndef STACKTEMPLATE_H
#define STACKTEMPLATE_H

#include<string>
void require(bool,const std::string&);
template<typename T> class StackTemplate{
private:
	enum{size=100};
	T stack[size];
	int top;
public:
	StackTemplate():top(0){}
	void push(const T &i){
		require(top>=size,"too many push() es");
		stack[top++]=i;
	}

	T pop(){
		require(top<0,"");
		return stack[top--];
	}

	int StackSize(){
		return top;
	}
};

#endif

#ifndef CCOUNTED_H
#define CCOUNTED_H

#include<iostream>

class CCounted{
private:
	static int Count;
public:
	CCounted(){++Count;}
	CCounted(const CCounted &other){++Count;}
	~CCounted(){--Count;}
	static int getCount(){return Count;}

};

int CCounted::Count=0;

class CCount1:public CCounted{};

class CCount2:public CCounted{};

#endif



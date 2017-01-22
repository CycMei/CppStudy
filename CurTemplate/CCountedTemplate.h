#ifndef CCOUNTEDTEMPLATE_H
#define CCOUNTEDTEMPLATE_H

#include<iostream>

template<typename T> class CTCounted{

private:
	static int count;
public:
	CTCounted(){++count;}
	CTCounted(const CTCounted &other){++count;}
	~CTCounted(){--count;}
	static int getCount(){return count;}
};

template<typename T> int CTCounted<T>::count=0;

class C1:public CTCounted<C1>{};
class C2:public CTCounted<C2>{};






#endif

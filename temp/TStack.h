#ifndef TSTACK_H
#define TSTACK_H

#include<iostream>

template<typename T> class TStack{
private:
	struct Link{
		T *data;
		Link *next;
		Link(T *dat,Link *nxt):data(dat),next(nxt){}
	} *head;
public:
	TStack():head(nullptr){}
	~TStack(){
		while(head){
			delete pop();
		}
	}

	void push(T *dat){
		head=new Link(dat,head);
	}	

	T *peek() const{
		return head?head->data:0;
	}
	
	T *pop(){
		if(!head)
			return 0;
		T *result=head->data;
		Link *oldHead=head;
		head=head->next;
		delete oldHead;
		return result;
	}

};



class X{

	virtual ~X(){

		std::cout<<"~x....."<<std::endl;

	}


};




#endif

#include<iostream>
#include<string>

void require(bool,const std::string&);

template<typename T,int size=100> class Array3{

private:
	T array[size];
public:
	T &operator[](int index){
		require(index<0||index>=size,"");
		return array[index];
	}
	
	int length() const {
		return size;
	}

};


class Number{

private:
	float f;
public:
	Number(float ff=0.0f):f(ff){}
	Number &operator=(const Number &n){
		f=n.f;
		return *this;
	}	


	operator float() const {
		return f;	
	}

	friend std::ostream &operator<<(std::ostream &os,const Number &x){
		return os<<x.f;
	}
};


template<typename T,int size=20> class Holder{
private:
	Array3<T,size> *np;
public:
	Holder():np(nullptr){}
	T &operator[](int i){
		if(!np)
			np=new Array3<T,size>;
		return np->operator[](i);

	}

	int length() const {
		return size;
	}

	~Holder(){
		delete np;
		np=nullptr;
	}

};

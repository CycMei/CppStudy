#include<iostream>
#include<string>
void require(bool,const std::string&);
namespace MArray{
	
	template<typename T> class  Array{
	private:
		enum {size=100};
		T A[size];
	public:
		T &operator[](int index);
	};

}

template<typename T> T &MArray::Array<T>::operator[](int index){

	require(index<0||index>=size,"index out of rang.....");	
	return A[index];

}


void ArrayTest(){
	MArray::Array<int> ia;
	MArray::Array<float> fa;
	for(int i=0;i<20;++i){
		ia[i]=i*i;
		fa[i]=float(i)*1.414;
	}

	for(int j=0;j<20;++j){
		std::cout<<j<<": "<<ia[j]<<", "<<fa[j]<<std::endl;
	}
}

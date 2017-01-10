#include<iostream>
#include<vector>
#include<algorithm>
#include<future>
#include<string>

namespace asyncs{
	template<typename RAIter>
	int parallel_sum(RAIter front,RAIter beg,RAIter end){
		auto len=end-beg;
		if(len<1000){
			//char out[50];
			std::string str1("calculate fron ");
			std::string str2(" to ");
			std::string str=str1+(beg-front)+str2+(end-front);
			//sprintf(out,"Calculate from %d to %d\n",beg-front,end-front);
			std::cout<<str;
			return std::accumulate(beg,end,0);
		}

		RAIter mid=beg+len/2;
		auto handle_right=std::async(std::launch::async,parallel_sum<RAIter>,front,mid,end);
		auto handle_left=std::async(std::launch::async,parallel_sum<RAIter>,front,beg,mid);
		return handle_left.get()+handle_right.get();
	}
}

void async(){
	std::vector<int> v(20000,1);
	std::cout<<"The sum is  "<<asyncs::parallel_sum(v.begin(),v.begin(),v.end())<<"\n";

}

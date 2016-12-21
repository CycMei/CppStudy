#ifndef FIBONACCI_H
#define FIBONACCI_H

#include"../require.h"

int fibonacci(const int n){
	const int sz=100;
	require(n<sz);
	static int f[sz];
	f[0]=f[1]=1;
	int i=0;
	for(i=0;i!=sz;++i){
		if(f[i]==0)
			break;
	}
	while(i<=n){
		f[i]=f[i-1]+f[i-2];
		++i;
	}
	return f[n];
}

#endif

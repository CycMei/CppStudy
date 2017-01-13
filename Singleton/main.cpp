#include"Singleton.h"
#include"Thread.h"


#include<Windows.h>
#include<iostream>
#include<thread>
#include<mutex>

std::mutex some_mutext;
void b();
void a() {

	while (1) {

		Sleep(250);

		std::lock_guard<std::mutex> guard(some_mutext);
		//b();
		std::cout << "a a.......->>>>>>>>>>>>   " << std::endl;
		
	}

}

void b() {
	while (1) {
		Sleep(100);
		std::lock_guard<std::mutex> guard(some_mutext);
		std::cout << "b b.......->>>>>>>>>>>>   " << std::endl;
	}

}

void test() {

	std::thread cyc1((SINGLETON::Cyc::Instance));
	std::thread cyc2{ SINGLETON::Cyc::Instance };
	cyc1.join();
	cyc2.join();


	//std::thread cyc1((a));
	//std::thread cyc2{ b };
	//cyc1.join();
	//cyc2.join();
}


void test1() {
	THREAD::background_task f;
	std::thread my_thread(f);
	my_thread.join();
}

void test2() {
	//´íÎóÊ¾Àý
	int local_status = 0;
	THREAD::Mfunc my_func(local_status);
	std::thread funThread(my_func);
	//funThread.detach();
	funThread.join();
}

void test3() {
	int local_status = 0;
	THREAD::Mfunc my_func(local_status);
	std::thread funThread(my_func);
	THREAD::thread_guard g(funThread);
	funThread.join();
}

void test4() {
	int i = 1;
	THREAD::Mfunc m1(i);
	m1.i = 10;
	THREAD::Mfunc m2=m1;
	m2.i = 4;
	std::cout << m1.i <<"  "<<m2.i<< std::endl;
}

int locaRef = 3;
void localRefes(int &rhs){
	rhs = 0;
}
void test5() {
	std::thread my_thread(localRefes, std::ref(locaRef));
	my_thread.join();
}

int main() {
	test5();
}
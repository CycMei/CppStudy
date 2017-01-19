#include"Singleton.h"
#include"Thread.h"
#include"mutex.h"
#include"sync.h"


#include<Windows.h>
#include<iostream>
#include<thread>
#include<mutex>
#include<vector>
#include<algorithm>
#include<functional>

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

void test6() {
	THREAD::X x;
	int i = 4;
	std::thread t(&THREAD::X::do_length_work,&x,i);
	t.join();
}

void test7() {
	std::thread t1(localRefes, std::ref(locaRef));
	std::cout << std::boolalpha << t1.joinable() << std::endl;
	t1.join();
	std::thread t2 = std::move(t1);
	std::cout<<std::boolalpha << t2.joinable() << std::endl;
	//t2.join();
	t1 = std::thread(localRefes, std::ref(locaRef));
	t1.join();
}

std::thread test8() {
	THREAD::X x;
	int i = 4;
	std::thread t(&THREAD::X::do_length_work, &x, i);
	//t.join();
	return t;
}

void test9() {
	std::vector<std::thread> threads;
	for (unsigned int i = 0; i != 20; ++i) {
		threads.push_back(std::thread(localRefes,std::ref(locaRef)));
	}
	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	std::cout << locaRef << std::endl;
	std::cout << std::thread::hardware_concurrency() << std::endl;
}

void test10() {
	std::vector<int> vec{15, 4, 57, 6, 5, 4, 6, 4, 65, 1, 2, 3, 3, 44, 25, 7, 4, 78,15, 4, 57, 6, 5, 4, 6, 4, 65, 1, 2, 3, 3, 44, 25, 7, 4, 78 ,15, 4, 57, 6, 5, 4, 6, 4, 65, 1, 2, 3, 3, 44, 25, 7, 4, 78 ,15, 4, 57, 6, 5, 4, 6, 4, 65, 1, 2, 3, 3, 44, 25, 7, 4, 78 };
	int result = 0;
	std::cout << "total is:  " << THREAD::parallel_accumulate(vec.begin(), vec.end(), result) << std::endl;
	std::cout << "total is:  " << std::accumulate(vec.begin(), vec.end(), 0) << std::endl;
}

void test11() {
	MUTEX::some_list sl;
	const int a = 3;
	std::thread t2(&MUTEX::some_list::list_contains, &sl, a);
	t2.join();
	std::thread t1(&MUTEX::some_list::add_to_list, &sl, a);
	t1.join();
	
}

void test12() {

	std::thread t2(MUTEX::foo);
	std::thread t1(MUTEX::foo);
	
	t1.join();
	t2.join();
}

void test13() {
	int i = 0;
	MUTEX::threadsafe_stack<int> ts1;
	std::thread t1(&MUTEX::threadsafe_stack<int>::pop, &ts1);
	MUTEX::threadsafe_stack<int> ts2(ts1);

	ts1.empty();
	t1.join();
}

MUTEX::some_big_object sbo1;
MUTEX::some_big_object sbo2;
MUTEX::X x1(sbo1);
MUTEX::X x2(sbo2);
void test14() {

	std::thread t1(&MUTEX::swaps, std::ref(x1), std::ref(x2));
	std::thread t2(&MUTEX::swapssss, std::ref(x1), std::ref(x2));
	t1.join();
	t2.join();
}

void test15() {
	MUTEX::Test t;
	std::thread A(MUTEX::fun1, &t);
	std::thread B(MUTEX::fun2, &t);
	A.join();
	B.join();
}

void test16() {
	MUTEX::high_level_func();
	MUTEX::thread_b();
}

void test17() {
	MUTEX::process_data();
}

int ya = 3;
int yb = 4;
MUTEX::Y yy1(ya);
MUTEX::Y yy2(yb);
void test18() {

	std::thread t1(MUTEX::operator==, std::ref(yy1), std::ref(yy2));
	std::thread t2(&MUTEX::Y::setDatail, &yy1);
	std::thread t3(&MUTEX::Y::setDatail, &yy2);

	t1.join();
	t2.join();
	t3.join();

}

MUTEX::dns_deche dd;
const std::string ss = "sss";

void test19() {
	std::thread t1(&MUTEX::dns_deche::find_entry, &dd, std::ref(ss));
	std::thread t2(&MUTEX::dns_deche::update_or_add_entry, &dd);
	t2.detach();
	t1.detach();
	
}

void test20() {
	std::thread t1(MUTEX::shared_lockss);
	std::thread t2(MUTEX::shared_dsfd);
	t1.join();
	t2.join();
}

void test21() {
	std::thread t1(MUTEX::bbbbfunc);
	t1.detach();
	std::thread t2(MUTEX::bbbbfunc2);
	t2.detach();
	system("pause");
}

void test22() {
	MUTEX::throwMain();
}

void test23() {
	MUTEX::glockMain();
}

void test24() {
	MUTEX::conMain();
}

void test25() {
	MUTEX::ConMain();
}



//sync.....
void stest1() {
	SYNC::thread_wait_for_flag();
}

void stest2() {
	SYNC::praDataMain();
}

void stest3() {
	SYNC::ccccpraDataMain();
}

void stest4() {
	SYNC::asyncMain();
}

void stest5() {
	SYNC::XMain();
}

void stest6() {
	SYNC::YMain();
}

void stest7() {
	SYNC::move_onlyMain();
}

void stest8() {
	SYNC::guiMain();
}

void stest9() {
	SYNC::thread_promise();
}

void stest10() {
	SYNC::threadpackagedTask();
}

void stest11() {
	SYNC::threadA();
}

void stest12() {
	
	SYNC::sqThrow();
}

void stest13() {
	SYNC::moveFuture();
}

void stest14() {
	SYNC::getCurTimeByChrono();
}

void stest15() {
	SYNC::wait_loop();
}

int main() {
	stest15();
}
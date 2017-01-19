#ifndef MUTEX_H
#define MUTEX_H

#include<Windows.h>
#include<list>
#include<mutex>
#include<algorithm>
#include<string>
#include<iostream>
#include<vector>
#include<stack>
#include<memory>
#include<set>
#include<exception>
#include<map>
#include<boost\thread\shared_mutex.hpp>
#include<boost\thread\shared_lock_guard.hpp>

namespace MUTEX {

	class some_list {
	private:
		std::list<int> somelist;
		std::mutex some_mutex;
	public:
		void add_to_list(const int new_value) {
			std::lock_guard<std::mutex> guard(some_mutex);
			somelist.push_back(new_value);
			std::cout << "add to list ...   " << std::endl;
		}
		void list_contains(const int value_to_find) {
			std::lock_guard<std::mutex> guard(some_mutex);
			bool bl = std::find(somelist.begin(), somelist.end(), value_to_find) != somelist.end();
			std::cout << std::boolalpha << "bl.....  :   " << bl << std::endl;
		}
	};



	class some_data {
	private:
		std::string b;
	public:
		int a = 3;
		void do_something() {
			a = 10;
		}
	};

	class data_wrapper {
	private:
		some_data data;
		std::mutex m;
	public:
		template<typename Fun> void process_data(Fun func) {
			std::lock_guard<std::mutex> lg(m);
			func(data);
		}
	};

	some_data *unprotected = nullptr;
	void malicious_function(some_data &protected_data) {
		unprotected = &protected_data;
	}

	data_wrapper x;
	void foo() {
		x.process_data(malicious_function);
		std::cout<<"first...  " << unprotected->a << std::endl;
		unprotected->do_something();
		std::cout<<"secode... " << unprotected->a << std::endl;
	}


	struct empty_stack :std::exception {
		const char *what() const throw(){
			return "cyc";
		}
	};


	template<typename T> class threadsafe_stack {
	private:
		std::stack<T> data;
		mutable std::mutex m;
	public:
		threadsafe_stack() {}
		explicit threadsafe_stack(const threadsafe_stack &other) {
			std::lock_guard<std::mutex> lock(other.m);
			std::cout << "copy constror......." << std::endl;
			data = other.data;
		}
		threadsafe_stack &operator=(const threadsafe_stack&) = delete;
		void push(T new_value) {
			std::lock_guard<std::mutex> lock(m);
			data.push(new_value);
		}
		
		std::shared_ptr<T> pop() {
			std::lock_guard<std::mutex> lock(m);
			std::cout << "pop.........." << std::endl;
			if (data.empty()) {
				return nullptr;
				throw empty_stack();
			}
				
			const std::shared_ptr<T> res(std::make_shared<T>(data.top()));
			data.pop();
			return res;
		}

		//void pop(T &value) {
		//	std::lock_guard<std::mutex> lock(m);
		//	if (data.empty())
		//		throw empty_stack();
		//	value = data.top();
		//	data.pop();
		//}

		bool empty() const {
			std::lock_guard<std::mutex> lock(m);
			std::cout << "empty empty......." << std::endl;
			return data.empty();
		}

	};




	class some_big_object {
	public:
		int a = 3;
		friend void swap(some_big_object&, some_big_object&);
	};

	void swap(some_big_object &lhs, some_big_object &rhs){
		int temp = lhs.a;
		lhs.a = rhs.a;
		rhs.a = temp;
	}
	

	class X {
	private:
		some_big_object some_datail;
		std::mutex m;
	public:
		explicit X(const some_big_object &sd):some_datail(sd){}
		friend void swaps(X &lhs, X &rhs);
		friend void swapssss(X &lhs, X &rhs);
	};

	void swaps(X &lhs, X &rhs) {
		if (&lhs == &rhs)
			return;
		std::lock(lhs.m, rhs.m);
		std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
		Sleep(1000);
		std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
		swap(lhs.some_datail, rhs.some_datail);

		//std::lock_guard<std::mutex> lock1(lhs.m);
		//Sleep(1000);
		//std::lock_guard<std::mutex> lock2(rhs.m);
		//swap(lhs.some_datail, rhs.some_datail);
	}

	void swapssss(X &lhs, X &rhs) {
		if (&lhs == &rhs)
			return;
		std::lock(lhs.m, rhs.m);
		std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
		Sleep(1000);
		std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
		swap(lhs.some_datail, rhs.some_datail);


		//std::lock_guard<std::mutex> lock2(rhs.m);
		//Sleep(1000);
		//std::lock_guard<std::mutex> lock1(lhs.m);
		//swap(lhs.some_datail, rhs.some_datail);
	}


	class Test {
	private:
		std::mutex m1;
		std::mutex m2;
	public:
		void fun1() {
			std::lock_guard<std::mutex> guard1(m1);
			Sleep(1000);
			std::lock_guard<std::mutex> guard2(m2);
		}

		void fun2() {
			std::lock_guard<std::mutex> guard2(m2);
			Sleep(1000);
			std::lock_guard<std::mutex> guard1(m1);
		}
	};

	void fun1(Test *p) {
		p->fun1();
	}
	void fun2(Test *p) {
		p->fun2();
	}







	class hierarchical_mutex {
	private:
		std::mutex internal_mutex;
		const unsigned long hierarchy_value;
		unsigned long previous_hierarchy_value;
		static thread_local unsigned long this_thread_hierarchy_value;
		void check_for_hierarchy_violation() {
			if (this_thread_hierarchy_value <= hierarchy_value) {
				throw std::logic_error("mutex hierarchy violated");
			}
		}
		void update_hierarchy_value() {
			previous_hierarchy_value = this_thread_hierarchy_value;
			this_thread_hierarchy_value = hierarchy_value;
		}
	public:
		explicit hierarchical_mutex(unsigned long value) :hierarchy_value(value), previous_hierarchy_value(0) {

		}

		void lock() {
			check_for_hierarchy_violation();
			internal_mutex.lock();
			update_hierarchy_value();
		}

		void unlock(){
			this_thread_hierarchy_value = previous_hierarchy_value;
			internal_mutex.unlock();
		}

		bool try_lock() {
			check_for_hierarchy_violation();
			if (!internal_mutex.try_lock())
				return false;
			update_hierarchy_value();
			return true;
		}
	};

	thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);



	hierarchical_mutex high_level_mutex(10000);
	hierarchical_mutex low_level_mutex(5000);

	int do_low_level_stuff() {
		return 0;
	}

	int low_level_func() {
		std::lock_guard<hierarchical_mutex> lk(low_level_mutex);
		return do_low_level_stuff();
	}

	void high_level_stuff(int some_param){}

	void high_level_func() {
		std::lock_guard<hierarchical_mutex> lk(high_level_mutex);
		high_level_stuff(low_level_func());
	}

	void thread_a() {
		high_level_func();
	}

	hierarchical_mutex other_mutex(100);

	void do_other_stuff(){}

	void other_stuff() {
		high_level_func();
		do_other_stuff();
	}


	void thread_b() {
		std::lock_guard<hierarchical_mutex> lk(other_mutex);
		other_stuff();
	}






	std::mutex somemutext;
	std::unique_lock<std::mutex> get_lock() {
		std::unique_lock<std::mutex> lk(somemutext);
		return lk;
	}
	void process_data() {
		std::unique_lock<std::mutex> lk(get_lock());
	}

	void get_and_process_data() {
		std::unique_lock<std::mutex> my_lock(somemutext);
		my_lock.unlock();
		my_lock.lock();
	}





	class Y {
	private:
		int some_datail;
		mutable std::mutex m;
		int get_datail() const {
			std::lock_guard<std::mutex> lock_a(m);
			std::cout << "some detail.......   " << some_datail << std::endl;
			return some_datail;
		}
	public:
		Y(int sd) :some_datail(sd) {}
		friend bool operator==(const Y &lhs, const Y &rhs);
		void setDatail() {
			std::lock_guard<std::mutex> lk(m);
			some_datail = 10;
			std::cout << "............    " << some_datail<<std::endl;
		}
	};
	bool operator==(const Y &lhs, const Y &rhs) {
		if (&lhs == &rhs)
			return false;
		const int lhs_value = lhs.get_datail();
		std::cout << "some detail.......1111   " << lhs.some_datail << std::endl;
		const int rhs_value = rhs.get_datail();
		std::cout << "some detail.......2222   " << rhs.some_datail << std::endl;

		std::cout << std::boolalpha << (lhs_value == rhs_value) << std::endl;
		return lhs_value == rhs_value;
	}



	std::shared_ptr<int> int_ptr;
	std::mutex int_moutex;
	std::once_flag int_flag;
	void undefined_behaviour_with_double_check_locking() {
		if (!int_ptr) {
			std::lock_guard<std::mutex> lk(int_moutex);
			if (!int_ptr) {
				int_ptr.reset(new int());
			}
		}
	}

	void init_int() {
		int_ptr.reset(new int());
	}

	void intfoo() {
		std::call_once(int_flag, init_int);
	}


	class my_class {

	};

	my_class &get_my_class_instance() {
		static my_class instance;
		return instance;
	}

	

	class dns_entry {};

	class dns_deche {
	private:
		std::map<std::string, dns_entry> entries;
		mutable boost::shared_mutex entry_mutex;
	public:
		dns_entry find_entry(std::string const &domain) const {
			boost::shared_lock_guard<boost::shared_mutex> lk(entry_mutex);
			std::map<std::string, dns_entry>::const_iterator const it = entries.find(domain);
			return (it == entries.end()) ? dns_entry() : it->second;
		}
		void update_or_add_entry() {
			std::lock_guard<boost::shared_mutex> lk(entry_mutex);
		}
	};


	boost::shared_mutex sh;
	void shared_lockss() {
		int i = 0;
		while (true) {
			std::cout << "shared............11111111" << std::endl;
			++i;
			
			boost::shared_lock_guard<boost::shared_mutex> lk(sh);
			std::cout << "shared............00000000" << std::endl;
			if (i >= 2)
				return;
		}
	}

	void shared_dsfd() {
		int i = 0;
		while (true) {
			++i;
			std::lock_guard<boost::shared_mutex> lk(sh);
			std::cout << "shared............22222222222" << std::endl;
			if (i >= 2)
				return;
		}
	}
	


	void bbbbfunc() {
		while (true) {
			std::cout << "func" << std::endl;
		}
	}

	void bbbbfunc2() {
		while (true) {
			std::cout << "func2" << std::endl;
		}
	}








	std::mutex g_mutex;
	std::vector<std::exception_ptr> g_exceptions;
	void throw_function() {
		throw std::exception("something wrong happened....");
	}
	void throwFunc() {
		try {
			throw_function();
		}
		catch (...) {
			std::lock_guard<std::mutex> lock(g_mutex);
			g_exceptions.push_back(std::current_exception());
		}
	}
	void throwMain() {
		g_exceptions.clear();
		std::thread t(throwFunc);
		t.join();
		for (auto &e : g_exceptions) {
			try {
				if (e != nullptr) {
					std::rethrow_exception(e);
				}
			}
			catch (const std::exception &e) {
				std::cout << e.what() << std::endl;
			}
		}
	}


	std::mutex g_lock;
	void glockFunc() {
		g_lock.lock();
		std::cout << "entered thread  " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(rand() % 10));
		std::cout << "leaving thread" << std::this_thread::get_id() << std::endl;
		g_lock.unlock();
	}
	void glockMain() {
		srand((unsigned int)time(0));
		std::thread t1(glockFunc);
		std::thread t2(glockFunc);
		std::thread t3(glockFunc);
		t1.join();
		t2.join();
		t3.join();
	}


	template<typename T> class container {
	private:
		mutable std::recursive_mutex _lock;
		std::vector<T> _element;
	public:
		void add(T element) {
			_lock.lock();
			_element.push_back(element);
			_lock.unlock();
		}

		void addrange(int num, ...) {
			va_list arguments;
			va_start(arguments, num);
			for (int i = 0; i != num; ++i) {
				_lock.lock();
				add(va_arg(arguments, T));
				_lock.unlock();
			}
			va_end(arguments);
		}
		void dump() const {
			_lock.lock();
			for (auto e : _element) {
				std::cout << e << std::endl;
			}
			_lock.unlock();
		}
	};

	void conFnc(container<int> &cont) {
		cont.addrange(3, rand(), rand(), rand());
	}
	void conMain() {
		srand((unsigned int)time(0));
		container<int> cont;
		std::thread t1(conFnc, std::ref(cont));
		std::thread t2(conFnc, std::ref(cont));
		std::thread t3(conFnc, std::ref(cont));
		t1.join();
		t2.join();
		t3.join();
		cont.dump();
	}



	template<typename T> class Con {
	public:
		std::mutex _lock;
		std::set<T> _element;
		void add(T element) {
			_element.insert(element);
		}
		void remove(T element) {
			_element.erase(element);
		}
	};
	void exchange(Con<int> &cont1, Con<int> &cont2, int value) {
		//cont1._lock.lock();
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		//cont2._lock.lock();
		//cont1.remove(value);
		//cont2.add(value);
		//cont1._lock.unlock();
		//cont2._lock.unlock();

		std::lock(cont1._lock, cont2._lock);
		std::lock_guard<std::mutex> lock_a(cont1._lock, std::adopt_lock);
		std::lock_guard<std::mutex> lock_b(cont2._lock, std::adopt_lock);
		cont1.remove(value);
		cont2.add(value);

	}

	void ConMain() {
		srand((unsigned int)time(nullptr));
		Con<int> cont1;
		cont1.add(1);
		cont1.add(2);
		cont1.add(3);
		Con<int> cont2;
		cont2.add(4);
		cont2.add(5);
		cont2.add(6);

		std::thread t1(exchange, std::ref(cont1), std::ref(cont2), 3);
		std::thread t2(exchange, std::ref(cont2), std::ref(cont1), 6);
		t1.join();
		t2.join();
	}


























}

#endif
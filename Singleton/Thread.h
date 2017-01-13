#ifndef THREAD_H
#define THREAD_H

#include<Windows.h>
#include<iostream>

namespace THREAD {
	class background_task {
	private:
		void do_something() const {}
		void do_something_else() const{}
	public:
		void operator()() const {
			do_something();
			do_something_else();
		}
	};

	struct Mfunc {
		void do_something(int i) {
			std::cout << "dong_something............"<< i << std::endl;
		}
		int &i;
		explicit Mfunc(int &i_) :i(i_) {}
		Mfunc(const Mfunc &rhs):i(rhs.i){}
		void operator()() {
			for (unsigned int j = 0; j != 10; ++j) {
				//Sleep(5000);
				do_something(i);
			}
		}
	};


	class thread_guard {
	private:
		std::thread &t;
		thread_guard(const thread_guard&) = delete;
		thread_guard &operator=(const thread_guard&) = delete;
	public:
		explicit thread_guard(std::thread &_t):t(_t){}
		~thread_guard() {
			if (t.joinable()) {
				std::cout << "this is joined....." << std::endl;
				t.join();
			}
		}

	};
}

#endif
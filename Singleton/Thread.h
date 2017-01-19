#ifndef THREAD_H
#define THREAD_H

#include<Windows.h>
#include<iostream>
#include<functional>
#include<exception>
#include<algorithm>
#include<numeric>
#include<vector>

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


	class X {
	public:
		X() {}
		~X() {}
		void do_length_work(int i){
			std::cout << i << " dsd " << std::endl;
		}

	};


	class scoped_thread {
	private:
		std::thread t;
	public:
		explicit scoped_thread(std::thread t) :t(std::move(t)) {
			if (!t.joinable())
				throw std::logic_error("no thread");
		}
		~scoped_thread() {
			t.join();
		}
		scoped_thread(const scoped_thread&) = delete;
		scoped_thread &operator=(const scoped_thread&) = delete;
	};


	template<typename Iter,typename T>
	struct accu_block {
		void operator()(Iter first, Iter last, T &result) {
			std::cout << "first...:  " << *first << std::endl;
			result = std::accumulate(first, last, result);
		}
	};

	template<typename Iterator,typename T>
	T parallel_accumulate(Iterator first, Iterator last, T &init) {
		const unsigned long length = std::distance(first, last);
		if (!length)
			return init;
		const unsigned long min_per_thread = 25;
		const unsigned long max_thread = (length + min_per_thread - 1) / min_per_thread;
		const unsigned long hardware_thread = std::thread::hardware_concurrency();
		const unsigned long num_thread = min(hardware_thread!=0? hardware_thread:2,max_thread);
		const unsigned long block_size = length / num_thread;
		std::vector<T> results(num_thread);
		std::vector<std::thread> threads(num_thread - 1);
		Iterator block_start = first;
		for (unsigned long i = 0; i != (num_thread - 1); ++i) {
			Iterator block_end = block_start;
			std::advance(block_end, block_size);
			threads[i] = std::thread(
				accu_block<Iterator,T>(),
				block_start,block_end,std::ref(results[i])
			);
			block_start = block_end;
		}
		accu_block<Iterator, T>()(block_start, last, results[num_thread - 1]);
		std::cout << "skfddfd.    " << std::endl;
		std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

		return std::accumulate(results.begin(), results.end(), init);
	}


}

#endif
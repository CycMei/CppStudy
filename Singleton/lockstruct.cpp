#include "lockstruct.h"

namespace std {
	template<> struct hash<LOCKSTRUCT::MTLT> {
		hash() {
			std::cout << "operator...." << std::endl;
		}
		size_t operator()() const {
		}
	};
}

namespace LOCKSTRUCT {

	const char *erro = "erro empty";
	const char *empty_stack::what() const throw() {
		std::cout << "erro emptoy.. " << std::endl;
		return erro;
	}

	void threadsafe() {
		threadsafe_stack<int> ts;
		const int new_value = 3;
		std::thread t1(&threadsafe_stack<int>::push, &ts, new_value);
		std::thread t2(&threadsafe_stack<int>::pop1, &ts);
		t2.detach();
		t1.detach();
		system("pause");
	}

	void threadsafequeue() {
		threadsafe_queue<int> tq;
		const int new_value = 3;
		std::thread t1(&threadsafe_queue<int>::push, &tq, new_value);
		std::thread t2(&threadsafe_queue<int>::wait_and_pop2, &tq);
		t2.join();
		t1.join();
	}

	void mqueuesss() {
		mqueue<int> mq;
		mq.push(3);
		mq.try_pop();
	}

	void lqueuess() {
		lqueue<int> lq;
		lq.push(3);
	}

	void nnthreadsafe_lock() {
		threadsafe_lock<int> tli;
		std::thread t1(&threadsafe_lock<int>::wait_and_pop1, &tli);
		std::thread t2(&threadsafe_lock<int>::push, &tli, 20);
		std::thread t3(&threadsafe_lock<int>::try_pop1, &tli);
		t1.detach();
		t2.detach();
		t3.detach();
		system("pause");
	}

	void threadsafelookuptable() {
		using T = threadsafe_lookup_table<std::string, int>;
		T tlt;
		std::string str("cyc");
		int value = 3;
		std::thread t1(&T::add_or_update_mapping, &tlt,std::ref(str),std::ref(value));
		std::thread t2(&T::add_or_update_mapping, &tlt, std::ref(str), std::ref(value));
		t1.join();
		t2.join();
	}



	void threadsafelist() {
		threadsafe_list<int> tl;
		int value = 3;
		//tl.push_front(3);
		//tl.push_front(4);
		std::thread t1(&threadsafe_list<int>::push_front, &tl, value);
		std::thread t2(&threadsafe_list<int>::push_front, &tl, value+1);
		t1.join();
		t2.join();
		std::thread t3(&threadsafe_list<int>::for_each<void(*)(int)>, &tl, [](int value)->void {
			std::cout << "value:   " << value << std::endl;
		});
		std::thread t4(&threadsafe_list<int>::find_first_if<bool(*)(int)>, &tl, [](int value)->bool {
			return true;
		});
		std::thread t5(&threadsafe_list<int>::remove_if<bool(*)(int)>, &tl, [](int value)->bool {
			return true;
		});
		std::thread t6(&threadsafe_list<int>::for_each<void(*)(int)>, &tl, [](int value)->void {
			std::cout << "value:   " << value << std::endl;
		});
		
		t5.join();
		t4.join();
		t3.join();
		t6.join();
	}

}




#ifndef ATOMIC_H
#define ATOMIC_H

#include<atomic>
#include<vector>
#include<iostream>
#include<thread>
#include<cassert>
#include<chrono>

namespace ATOMIC {
	void atobool();
	void readydata();
	void readxyz();
	void xxyyzz();

	class spinlock_mutex {
	private:
		std::atomic_flag flag=ATOMIC_FLAG_INIT;
	public:
		spinlock_mutex();
		void lock();
		void unlock();
	};
}


#endif // !ATOMIC_H

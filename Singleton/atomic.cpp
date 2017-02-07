#include "atomic.h"

namespace ATOMIC {

	spinlock_mutex::spinlock_mutex()  {

	}

	void spinlock_mutex::lock() {
		while (flag.test_and_set(std::memory_order_acquire)) {

		}
	}

	void spinlock_mutex::unlock() {
		flag.clear(std::memory_order_release);
	}

	void atobool() {
		std::atomic<bool> b;
		bool x = b.load(std::memory_order_acquire);
		b.store(true);
		x = b.exchange(false, std::memory_order_acq_rel);

	}




	std::vector<int> data;
	std::atomic<bool> data_ready(false);
	void reader_thread() {
		while (!data_ready.load()) {
			std::cout << "rady..data.." << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		std::cout << "the answer :  " << data[0] << std::endl;
	}
	void writer_thread() {
		data.push_back(42);
		data_ready = true;
	}
	void readydata() {
		std::thread t1(reader_thread);
		std::thread t2(writer_thread);
		t1.detach();
		t2.detach();
		system("pause");
	}



	std::atomic<bool> x, y;
	std::atomic<int> z;
	void write_x() {
		x.store(true, std::memory_order_seq_cst);
	}
	void write_y() {
		y.store(true);
	}
	void read_x_then_y() {
		while (!x.load(std::memory_order_seq_cst));
		if (y.load(std::memory_order_seq_cst))
			++z;
	}
	void read_y_then_x() {
		while (!y.load(std::memory_order_seq_cst));
		if (x.load(std::memory_order_seq_cst))
			++z;
	}
	void readxyz() {
		x = false;
		y = false;
		z = 0;
		std::thread a(write_x);
		std::thread b(write_y);
		std::thread c(read_x_then_y);
		std::thread d(read_y_then_x);
		a.join();
		b.join();
		c.join();
		d.join();
		assert(z.load() != 0);
	}





	std::atomic<int> xx(0), yy(0), zz(0);
	std::atomic<bool> go(false);
	unsigned const loop_count = 10;
	struct read_values {
		int x, y, z;
	};
	read_values values1[loop_count];
	read_values values2[loop_count];
	read_values values3[loop_count];
	read_values values4[loop_count];
	read_values values5[loop_count];
	void increment(std::atomic<int> *var_to_inc, read_values *values) {
		while (!go) {
			std::this_thread::yield();
		}
		for (unsigned int i = 0; i != loop_count; ++i) {
			values[i].x = xx.load(std::memory_order_relaxed);
			values[i].y = yy.load(std::memory_order_relaxed);
			values[i].z = zz.load(std::memory_order_relaxed);
			var_to_inc->store(i + 1, std::memory_order_relaxed);
			std::this_thread::yield();
		}
	}
	void read_vals(read_values *values) {
		while (!go) {
			std::this_thread::yield();
		}
		for (unsigned int i = 0; i != loop_count; ++i) {
			values[i].x = xx.load(std::memory_order_relaxed);
			values[i].y = yy.load(std::memory_order_relaxed);
			values[i].z = zz.load(std::memory_order_relaxed);
			std::this_thread::yield();
		}
	}
	void print(read_values *v) {
		for (unsigned int i = 0; i != loop_count; ++i) {
			if (i)
				std::cout << ",";
			std::cout << "(" << v[i].x << "," << v[i].y << "," << v[i].z << ")";
		}
		std::cout << std::endl;
	}
	void xxyyzz() {
		std::thread t1(increment, &xx, values1);
		std::thread t2(increment, &yy, values2);
		std::thread t3(increment, &zz, values3);
		std::thread t4(read_vals, values4);
		std::thread t5(read_vals, values5);
		go = true;
		t5.join();
		t4.join();
		t3.join();
		t2.join();
		t1.join();
		print(values1);
		print(values2);
		print(values3);
		print(values4);
		print(values5);
	}


}




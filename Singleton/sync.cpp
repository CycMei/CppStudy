#include"sync.h"

#include<random>
#include<cassert>
#include<typeinfo>
#include<ratio>

namespace SYNC {

	bool flag = false;
	std::mutex m;
	void changeFlag() {
		std::lock_guard<std::mutex> lk(m);
		flag = !flag;
	}
	void wait_for_flag() {
		std::unique_lock<std::mutex> lk(m);
		while (!flag) {
			lk.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			std::cout << "flag..flag...." << std::endl;
			lk.lock();
		}
	}
	void thread_wait_for_flag() {
		std::thread t(wait_for_flag);
		std::thread t1(changeFlag);
		t.join();
		t1.join();
	}





	std::mutex mut;
	typedef int data_chunk;
	std::queue<data_chunk> data_queue;
	std::condition_variable data_cond;
	std::vector<int> preVec;
	data_chunk index = 3;
	data_chunk prepare_data() {
		return 3;
	}
	bool more_data_to_prepare() {
		return index--;
	}
	void process(const data_chunk data) {
		std::cout << "data.... ::   " << data << std::endl;
	}
	void data_preparation_thread() {
		while (more_data_to_prepare()) {
			const data_chunk data = prepare_data();
			std::cout << "pre data.....   " << std::endl;
			std::lock_guard<std::mutex> lk(mut);
			data_queue.push(data);
			data_cond.notify_one();
		}
	}
	void data_processing_thread() {
		while (true) {
			std::unique_lock<std::mutex> lk(mut);
			data_cond.wait(lk, []()->bool {return !data_queue.empty(); });
			std::cout << "res data.. ....   " << std::endl;
			data_chunk data = data_queue.front();
			data_queue.pop();
			lk.unlock();
			process(data);
			if (data_queue.empty())
				break;
		}
	}
	void praDataMain() {
		std::thread t1(data_preparation_thread);
		std::thread t2(data_processing_thread);
		t1.join();
		t2.join();
	}





	threadsafe_queue<int> cdataqueue;
	void cdataprethread() {
		int index = 3;
		int value = 0;
		while (index) {
			cdataqueue.push(value++);
			--index;
		}
	}
	void dataprocessthread() {
		while (true) {
			int data = 0;
			cdataqueue.wait_and_pop(data);
			process(data);
			if (cdataqueue.empty())
				break;
		}
	}
	void ccccpraDataMain() {
		std::thread t1(cdataprethread);
		std::thread t2(dataprocessthread);
		t1.join();
		t2.join();
	}


	void do_other_stuff() {
		std::cout << "do other something......  " << std::endl;
	}
	int find_the_answer_to_ltuae(int value) {
		std::cout << "id:  " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		return value*value;
	}
	void asyncMain() {
		int value = 10;
		std::future<int> the_answer = std::async(find_the_answer_to_ltuae, value);
		do_other_stuff();
		std::cout<<"anseer....  " << the_answer.get() << std::endl;
	}








	void X::foo(int value,const std::string &str) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << "value:   " << value << "  str:   " << str << std::endl;
	}
	std::string X::bar(const std::string &str) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << "str:  " << str << std::endl;
		return "cyc";
	}
	void XMain() {
		X x;
		std::future<void> f1 = std::async(&X::foo, &x, 42, std::string("hello"));
		std::future<std::string> f2 = std::async(&X::bar, x, std::string("world"));
		std::thread t(do_other_stuff);
		t.detach();
		std::cout << "f2...  " << f2.get() << std::endl;
	}
	double Y::operator()(double value) {
		std::cout << "this_thread_id operator:   " << std::this_thread::get_id() << std::endl;
		std::cout << "value:   " << value << std::endl;
		dValue = value;
		return value*value;
	}
	Y baz(Y &ref) {
		return ref;
	}
	void YMain() {
		std::cout << "this_thread_id ymain:   " << std::this_thread::get_id() << std::endl;
		double dValue = 3.14;
		Y y;
		std::future<double> f3 = std::async(std::launch::async,Y(), 3.14);
		std::future<double> f4 = std::async(std::launch::deferred,std::ref(y), 2.718);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << "dvalue:   " << y.dValue << std::endl;
		std::async(std::launch::async|std::launch::deferred,baz, std::ref(y));
	}

	move_only::move_only() { 
		std::cout << "constror move_only" << std::endl; 
	}
	move_only::move_only(move_only &&other) { 
		std::cout << "move move_only" << std::endl; 
	}
	move_only &move_only::operator=(move_only &&other) { 
		return other; 
	}
	void move_only::operator()() { 
		std::cout << "operator move_only" << std::endl; 
	}

	void move_onlyMain() {
		std::future<void> f5 = std::async(move_only());
		//调用构造函数产生临时对象，让后调用std::move 调用移动构造,然后函数对象operator
	}






	std::mutex fmut;
	std::deque<std::packaged_task<void()>> tasks;
	bool gui_shutdown_message_received() {
		return true;
	}
	void get_and_process_gui_message(){}
	void gui_thread() {
		while (gui_shutdown_message_received()) {
			get_and_process_gui_message();
			std::cout << "gui message id:   " << std::this_thread::get_id() << std::endl;
			std::packaged_task<void()> task;
			{
				std::lock_guard<std::mutex> lk(fmut);
				if (tasks.empty())
					continue;
				task = std::move(tasks.front());
				tasks.pop_front();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			task();
		}
	}
	template<typename T> std::future<void> post_task_for_gui_thread(T f) {
		std::cout << "post_task_for_gui_thread" << std::endl;
		std::packaged_task<void()> task(f);
		std::future<void> res = task.get_future();
		std::lock_guard<std::mutex> lk(fmut);
		tasks.push_back(std::move(task));
		return res;
	}
	void mtask() {
		std::cout << "cyc .....   " << std::endl;
	}
	void getFuture() {
		std::future<void> fut = post_task_for_gui_thread(mtask);
	}
	void guiMain() {
		std::thread gui_bg_thread(gui_thread);
		std::thread guipostthread(getFuture);
		guipostthread.join();
		gui_bg_thread.join();
	}






	std::promise<std::string> val;
	void thread_promise() {
		std::thread ta([]() {
			std::cout << "waiting promise->future:   " << "sdk" << std::endl;
			std::future<std::string> fu = val.get_future();
			std::cout << "waiting promise->future:   " << fu.get() << std::endl;
		});
		std::thread tb([]() {
			std::cout << "set  promise  promise:   "  << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			val.set_value("promise is set...");
		});
		ta.join();
		tb.join();
	}





	std::mutex g_mutex;
	void threadpackagedTask() {
		auto run = [=](int index)->int {
			std::lock_guard<std::mutex> _s(g_mutex);
			std::cout << "tasklet......   " << index << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(10));
			return index * 1000;
		};
		std::packaged_task<int(int)> pt1(run);
		std::packaged_task<int(int)> pt2(run);
		std::thread t1([&]() {
			pt1(2);
		});
		std::thread t2([&]() {
			pt2(3);
		});

		std::this_thread::sleep_for(std::chrono::milliseconds(10 * 1000));
		int f1 = pt1.get_future().get();
		int f2 = pt2.get_future().get();
		std::cout << "task result 1:   " << f1 << std::endl;
		std::cout << "task result 2:  " << f2 << std::endl;
		t1.join();
		t2.join();
	}





	std::mutex mutex_m;
	long do_sum(std::vector<long> *arr, std::size_t start, std::size_t count) {
		long sum = 0;
		for (std::size_t i = 0; i != count; ++i) {
			sum += (*arr)[start + i];
		}
		{
			std::lock_guard<std::mutex> _m(mutex_m);
			std::cout << "thread:   " << std::this_thread::get_id() << " , count=  " << count << " , " << sum << std::endl;
		}
		return sum;
	}

	long mrandsss() {
		static std::default_random_engine engines(100);
		static std::uniform_int_distribution<long> u(0, 10000000000);
		return u(engines);
	}

	void threadA() {
		const int count = 10000;
		std::vector<long> data;
		for (std::size_t i = 0; i != count; ++i) {
			//std::cout << "mrandsss() & 0xff.. " << (mrandsss() & 0xff) << std::endl;
			data.push_back(mrandsss() & 0xff);
		}
		std::vector<std::future<long>> result;
		std::size_t ptc = std::thread::hardware_concurrency() * 2;
		for (std::size_t batch = 0; batch < ptc; ++batch) {
			std::size_t batch_each = count / ptc;
			if (batch == ptc - 1) {
				batch_each = count - (count / ptc*batch);
			}
			result.push_back(std::async(do_sum, &data, batch*batch_each, batch_each));
		}
		long total = 0;
		for (std::size_t batch = 0; batch != ptc; ++batch) {
			total += result[batch].get();
		}
		std::cout << "total:   " << total << std::endl;
	}





	double square_root(double dValue) {
		if (dValue < 0)
			throw std::out_of_range("dValue<0");
		if(dValue==0)
			throw std::logic_error("logic erro...");
		return sqrt(dValue);
	}
	std::promise<double> some_promise;
	void moveFuture() {
		some_promise.set_value(22);
		std::future<double> f(some_promise.get_future());
		assert(f.valid());

		//std::shared_future<double> sf(std::move(f));
		//assert(sf.valid());

		std::shared_future<double> ssf(f.share());
		assert(ssf.valid());

		std::shared_future<double> sssf(ssf);
		assert(sssf.valid());

		//assert(f.valid());
		
	}
	void sqThrow() {
		std::future<double> f = std::async(square_root, -1);
		std::thread t([]()->void {
			try {
				some_promise.set_value(square_root(0));
			}
			catch (const std::logic_error &erro) {
				some_promise.set_exception(std::current_exception());
			}
		});
		std::thread t2([]()->void {
			std::future<double> f = some_promise.get_future();
			try {
				f.get();
			}
			catch (const std::logic_error &erro) {
				std::cout << "future erro:   " << erro.what() << std::endl;;
			}
		});


		try {
			f.get();
		}
		catch (const std::out_of_range &erro) {
			std::cout << "erro....  " << erro.what() << std::endl;
		}

		t2.join();
		t.join();

	}






	void getCurTimeByChrono() {
		auto result=std::chrono::system_clock::now();
		//std::cout << "chrono rsult :  " << result << std::endl;
		std::chrono::time_point<std::chrono::system_clock> cres = std::chrono::system_clock::now();
		std::ratio<1, 25>;
		std::chrono::system_clock::period per;
		//assert(std::chrono::system_clock::is_steady);
		std::chrono::steady_clock slock;
		std::chrono::high_resolution_clock hlock;
		std::chrono::duration<short, std::ratio<60, 1>> sd;
		std::chrono::duration<double, std::ratio<1, 1000>> dd;
		std::chrono::nanoseconds nnlock;// 1/十亿 seconds
		std::chrono::microseconds mmlock;// 1/一百万 
		std::chrono::milliseconds milllock(54802);//1/1000 
		std::chrono::seconds s=std::chrono::duration_cast<std::chrono::seconds>(milllock);
		assert(5 * std::chrono::seconds(1) == std::chrono::seconds(5));
		std::cout << "distance time:  " << ((std::chrono::minutes(1) - std::chrono::seconds(55)).count()) << std::endl;

		std::future<int> f = std::async([]()->int {
			return 10;
		});
		assert(f.wait_for(std::chrono::milliseconds(35)) == std::future_status::ready);


		std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> aaa;
		std::cout << "since time:  "<<aaa.time_since_epoch().count()<<std::endl;
		auto start = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(std::chrono::milliseconds(5 * 1000));
		auto end = std::chrono::high_resolution_clock::now();
		std::cout <<"end - start:  "<< (std::chrono::duration<double, std::ratio<1,1000>>(end - start).count()) << std::endl;
	}

	std::mutex chrmut;
	std::condition_variable cv;
	void wait_loop() {
		const auto timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(5000);
		std::unique_lock<std::mutex> lk(chrmut);
		while (true) {
			if (cv.wait_until(lk, timeout) == std::cv_status::timeout)
				break;
		}
	}
	void mratio() {
		typedef std::ratio<2, 4> a;
		typedef std::ratio<1, 3> b;
		typedef std::ratio_add<a, b> c;
		typedef std::ratio_less<b, a> d;
		std::cout << "a num: " << a::num << "  a den:  " << a::den << std::endl;
		std::cout << "b num:  " << b::num << "  b den:  " << b::den << std::endl;
		std::cout << "c num:  " << c::num << " c den:  " << c::den << std::endl;
		std::cout << "d num:  " << d::value << std::endl;
	}










	int ifun(int ii) {
		return int(ii);
	}
	typedef int(&fn_ref)(int);
	typedef int(*fn_ptr)(int);
	struct fn_class {
		int operator()(int i) {
			return i;
		}
	};

	void spawn_tasktestss() {
		int i = 3;
		spawn_task(ifun, std::move(i));
	}

	decltype(ifun) &reRef(int) {
		return ifun;
	}

	int (&copyreRef(int))(int) {
		return ifun;
	}

	decltype(ifun) &ss = copyreRef(3);

	void result_ofsss() {

		typedef decltype(ifun) &ss(int);
		//int (&sss(int)) (int);

		typedef std::result_of<ss>::type A;
		//std::cout << typeid(decltype(reRef)).name() << std::endl;
		//typedef std::result_of<decltype(reRef)>::type A;
		std::cout << typeid(A).name() << std::endl;
		std::cout << "A:  " <<std::boolalpha<< std::is_same<int, A>::value << std::endl;

		typedef std::result_of<fn_ref(int)>::type B;
		std::cout << "B:  " << std::boolalpha << std::is_same<int, B>::value << std::endl;

		typedef std::result_of<fn_ptr(int)>::type C;
		std::cout << "C:  " << std::boolalpha << std::is_same<int, C>::value << std::endl;

		typedef std::result_of<fn_class(int)>::type D;
		std::cout << "D:  " << std::boolalpha << std::is_same<int, D>::value << std::endl;

	}




	std::chrono::high_resolution_clock::time_point points;
	void sleep1() {
		std::chrono::duration<double, std::ratio<1, 1000>> duration(2000);
		std::this_thread::sleep_for(duration);
		std::cout << "duration:   " << duration.count() << std::endl;
	}
	void sleep2() {
		//std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::milliseconds> points;
		
		std::this_thread::sleep_until(points + (std::chrono::milliseconds(10 * 1000)));
		std::cout << points.time_since_epoch().count() << std::endl;
	}

	void sleepTime() {
		points = std::chrono::high_resolution_clock::now();
		std::thread t1(sleep1);
		std::thread t2(sleep2);
		t1.detach();
		t2.detach();
		system("pause");
	}





	std::condition_variable mcd;
	std::mutex mcd_lock;
	void mdcres() {
		while (true) {

		}
		mcd.notify_all();
	}
	void recmcdcopy();
	void recmcd() {
		//recmcdcopy();
		std::unique_lock<std::mutex> lk(mcd_lock);
		if (mcd.wait_for(lk, std::chrono::duration<double, std::ratio<1, 1000>>(2000)) == std::cv_status::timeout) {
			std::cout << "time out  " << std::endl;
		}
	}
	void recmcdcopy() {
		std::unique_lock<std::mutex> lk(mcd_lock);
		bool bl= mcd.wait_until(lk, std::chrono::steady_clock::now() + std::chrono::milliseconds(5 * 1000), []()->bool {
			int i = 3;
			++i;
			return i = 0;
		});
		if (!bl) {
			std::cout << "time out  11111" << std::endl;
		}
	}
	void mcdnotiy() {
		std::thread t1(mdcres);
		std::thread t2(recmcd);
		std::thread t3(recmcdcopy);
		t1.detach();
		t2.detach();
		t3.detach();
		system("pause");
	}






	std::timed_mutex time_lock;
	void upLokc() {
		std::lock_guard<std::timed_mutex> lk(time_lock);
		while (1) {

		}
	}
	void timelock() {
		if (!time_lock.try_lock_for(std::chrono::milliseconds(5 * 1000)))
			std::cout << "not get lock  ... " << std::endl;
	}
	void timethread() {
		std::thread t1(upLokc);
		std::thread t2(timelock);
		t1.detach();
		t2.detach();
		system("pause");
	}




	void futureTime() {
		std::future<void> f = std::async(upLokc);
		if (f.wait_for(std::chrono::milliseconds(5 * 1000)) == std::future_status::timeout) {
			std::cout << "future time out " << std::endl;
			f.get();
		}
		system("pause");
	}




}
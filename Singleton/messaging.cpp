#include"messaging.h"

namespace messaging {
	void func() {

	}
	void test1() {
		queue *qe = new queue();
		dispatcher dp(qe);
		//dp.handle(func);

	}
}

namespace messaging {
	std::shared_ptr<message_bace> queue::wait_and_pop() {
		std::unique_lock<std::mutex> lk(m);
		c.wait(lk, [&] {return !q.empty(); });
		auto res = q.front();
		q.pop();
		return res;
	}
}

namespace messaging {
	receiver::operator sender() {
		return sender(&q);
	}
	dispatcher receiver::wait() {
		std::cout << "wait wait ...  " << std::endl;
		return dispatcher(&q);
	}
}


namespace messaging {

	void dispatcher::wait_and_dispatch() {
		for (;;) {
			auto msg = q->wait_and_pop();
			dispatch(msg);
		}
	}

	bool dispatcher::dispatch(const std::shared_ptr<message_bace> &msg) {
		if(dynamic_cast<wrapped_message<close_queue>*>(msg.get()))
			throw close_queue();
		return false;
	}

	dispatcher::dispatcher(dispatcher &&other) :q(other.q), chained(other.chained) {
		other.chained = true;
	}
	
	dispatcher::dispatcher(queue *q_) : q(q_), chained(false) {

	}

	dispatcher::~dispatcher() /*noexcept(false)*/ {
		if (!chained) {
			wait_and_dispatch();
		}
	}



}





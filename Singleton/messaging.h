#ifndef MESSAGING_H
#define MESSAGING_H

#include<mutex>
#include<condition_variable>
#include<queue>
#include<memory>
#include<iostream>
#include<utility>

namespace messaging {
	void test1();
}

namespace messaging {

	struct message_bace {
		virtual ~message_bace(){}
	};

	template<typename Msg> struct wrapped_message :public message_bace {
		Msg contents;
		explicit wrapped_message(const Msg &contents_) :contents(contents_) {}
	};

	class queue {
	private:
		std::mutex m;
		std::condition_variable c;
		std::queue<std::shared_ptr<message_bace>> q;
	public:
		template<typename T> void push(const T &msg);
		std::shared_ptr<message_bace> wait_and_pop();
	};


	class sender {
	private:
		queue *q;
	public:
		sender():q(nullptr){}
		explicit sender(queue *q_):q(q_){}
		template<typename Message> void send(const Message &msg);
	};

	class dispatcher;
	class receiver {
	private:
		queue q;
	public:
		operator sender();
		dispatcher wait();
	};


	class close_queue{};

	class dispatcher {
	private:
		queue *q;
		bool chained;
		dispatcher(const dispatcher&) = delete;
		dispatcher &operator=(const dispatcher&) = delete;

		template<typename Dispatcher,typename Msg,typename Func>
		friend class TemplateDispatcher;

		void wait_and_dispatch();
		bool dispatch(const std::shared_ptr<message_bace> &msg);
	public:
		dispatcher(dispatcher &&other);
		explicit dispatcher(queue *q_);

		template<typename Message,typename Func> 
		TemplateDispatcher<dispatcher, Message, Func> handle(Func &&f);

		~dispatcher() /*noexcept(false)*/;
	};


	template<typename PreviousDispatcher,typename Msg,typename Func>
	class TemplateDispatcher {
	private:
		queue *q;
		PreviousDispatcher *prev;
		Func f;
		bool chained;
		TemplateDispatcher(const TemplateDispatcher&) = delete;
		TemplateDispatcher &operator=(const TemplateDispatcher&) = delete;

		template<typename Dispatcher,typename OtherMsg,typename OtherFunc>
		friend class TemplateDispatcher;

		void wait_and_dispatch();
		bool dispatch(const std::shared_ptr<message_bace> &msg);
	public:
		TemplateDispatcher(TemplateDispatcher &&other);
		TemplateDispatcher(queue *q_, PreviousDispatcher *prev_, Func &&f_);
		template<typename OtherMsg, typename OtherFunc> TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc> handle(OtherFunc &&of) {
			return TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>(q, this, std::forward<OtherFunc>(of));
		}
		~TemplateDispatcher() /*noexcept(false)*/;
	};



}
#endif

namespace messaging {
	template<typename T> inline void queue::push(const T & msg) {
		std::lock_guard<std::mutex> lk(m);
		q.push(std::make_shared<wrapped_message<T>>(msg));
		c.notify_all();
	}
}

namespace messaging {
	template<typename Message> inline void sender::send(const Message & msg) {
		if (q)
			q->push(msg);
	}
}

namespace messaging {
	template<typename Message, typename Func>
	inline TemplateDispatcher<dispatcher, Message, Func> dispatcher::handle(Func && f) {
		return TemplateDispatcher<dispatcher, Message, Func>(q, this, std::forward<Func>(f));
	}
}

namespace messaging {

	template<typename PreviousDispatcher, typename Msg, typename Func>
	inline void TemplateDispatcher<PreviousDispatcher, Msg, Func>::wait_and_dispatch() {
		for (;;) {
			auto msg = q->wait_and_pop();
			if (dispatch(msg))
				break;
		}
	}

	template<typename PreviousDispatcher, typename Msg, typename Func>
	inline bool TemplateDispatcher<PreviousDispatcher, Msg, Func>::dispatch(const std::shared_ptr<message_bace>& msg) {
		if (wrapped_message<Msg> *wrapper = dynamic_cast<wrapped_message<Msg>*>(msg.get())) {
			f(wrapper->contents);
			return true;
		}
		else {
			return prev->dispatch(msg);
		}
	}

	template<typename PreviousDispatcher, typename Msg, typename Func>
	inline TemplateDispatcher<PreviousDispatcher, Msg, Func>::TemplateDispatcher(TemplateDispatcher && other)
		:q(other.q), prev(other.prev), f(std::move(other.f)), chained(other.chained) {
		other.chained = true;
	}

	template<typename PreviousDispatcher, typename Msg, typename Func>
	inline TemplateDispatcher<PreviousDispatcher, Msg, Func>::TemplateDispatcher(queue * q_, PreviousDispatcher * prev_, Func && f_)
		: q(q_), prev(prev_), f(std::forward<Func>(f_)), chained(false) {
		prev_->chained = true;
	}


	template<typename PreviousDispatcher, typename Msg, typename Func>
	inline TemplateDispatcher<PreviousDispatcher, Msg, Func>::~TemplateDispatcher() /*noexcept(false)*/ {
		if (!chained) {
			std::cout << "destory TemplateDispatcher " << std::endl;
			wait_and_dispatch();
		}
	}


}
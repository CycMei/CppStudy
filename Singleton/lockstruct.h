#ifndef LOCKSTRUCT_H
#define LOCKSTRUCT_H

#include<exception>
#include<stack>
#include<mutex>
#include<iostream>
#include<memory>
#include<thread>
#include<queue>
#include<vector>
#include<functional>
#include<map>
#include<list>
#include<algorithm>
#include<set>
#include<utility>
#include<string>
#include<boost\thread\shared_mutex.hpp>
#include<boost\thread\shared_lock_guard.hpp>
#include<condition_variable>

namespace LOCKSTRUCT {

	void threadsafe();

	struct empty_stack :std::exception {
		const char *what() const throw();
	};

	template<typename T> class threadsafe_stack {
	private:
		mutable std::mutex m;
		std::stack<T> data;
	public:
		threadsafe_stack();
		threadsafe_stack(const threadsafe_stack&);
		threadsafe_stack &operator=(const threadsafe_stack&) = delete;
		void push(T);
		std::shared_ptr<T> pop1();
		void pop2(T&);
		bool empty() const;
	};


	void threadsafequeue();
	template<typename T> class threadsafe_queue {
	private:
		mutable std::mutex mut;
		std::queue<std::shared_ptr<T>> data_queue;
		std::condition_variable data_cond;
	public:
		threadsafe_queue();
		void push(T);
		void wait_and_pop1(T&);
		std::shared_ptr<T> wait_and_pop2();
		bool try_pop1(T&);
		std::shared_ptr<T> try_pop2();
		bool empty() const;
	};


	void mqueuesss();
	template<typename T> class mqueue {
	private:
		struct node {
			T data;
			std::unique_ptr<node> next;
			explicit node(T);
		};
		std::unique_ptr<node> head;
		node *tail;
	public:
		mqueue(){}
		mqueue(const mqueue&) = delete;
		mqueue &operator=(const mqueue&) = delete;
		std::shared_ptr<T> try_pop();
		void push(T);
	};


	void lqueuess();
	template<typename T> class lqueue {
	private:
		struct node {
			std::shared_ptr<T> data;
			std::unique_ptr<node> next;
		};
		std::unique_ptr<node> head;
		node *tail;
	public:
		lqueue();
		lqueue(const lqueue&) = delete;
		lqueue operator=(const lqueue&) = delete;
		std::shared_ptr<T> try_pop();
		void push(T);
	};


	void nnthreadsafe_lock();
	template<typename T> class threadsafe_lock {
	private:
		struct node {
			std::shared_ptr<T> data;
			std::unique_ptr<node> next;
		};
		std::mutex head_mutex;
		std::unique_ptr<node> head;
		std::mutex tail_mutex;
		node *tail;
		std::condition_variable data_cond;
		node *get_tail() {
			std::lock_guard<std::mutex> tail_lock(tail_mutex);
			return tail;
		}
		std::unique_ptr<node> pop_head() {
			std::unique_ptr<node> old_head = std::move(head);
			head = std::move(old_head->next);
			return old_head;
		}
		std::unique_lock<std::mutex> wait_for_data() {
			std::unique_lock<std::mutex> head_lock(head_mutex);
			data_cond.wait(head_lock, [&]()->bool {
				return head.get() != get_tail();
			});
			return std::move(head_lock);
		}
		std::unique_ptr<node> wait_pop_head() {
			std::unique_lock<std::mutex> head_lock(wait_for_data());
			return pop_head();
		}
		std::unique_ptr<node> wait_pop_head(T &value) {
			std::unique_lock<std::mutex> head_lock(wait_for_data());
			value = std::move(*head->data);
			return pop_head();
		}
		std::unique_ptr<node> try_pop_head() {
			std::lock_guard<std::mutex> head_lock(head_mutex);
			if (head.get() == get_tail())
				return std::unique_ptr<node>();
			return pop_head();
		}
		std::unique_ptr<node> try_pop_head(T &value) {
			std::lock_guard<std::mutex> head_lock(head_mutex);
			if (head.get() == get_tail())
				return std::unique_ptr<node>();
			value = std::move(*head->data);
			return pop_head();
		}


	public:
		threadsafe_lock();
		threadsafe_lock(const threadsafe_lock&) = delete;
		threadsafe_lock &operator=(const threadsafe_lock&) = delete;
		void push(T);
		std::shared_ptr<T> wait_and_pop1();
		void wait_and_pop2(T&);
		std::shared_ptr<T> try_pop1();
		bool try_pop2(T&);
		bool empty() const;

	};


	void threadsafelookuptable();
	class MTLT{};
	template<typename Key, typename Value, typename Hash = std::hash<Key>> class threadsafe_lookup_table {
	private:
		class bucket_type {
		private:
			typedef std::pair<Key, Value> bucket_value;
			typedef std::list<bucket_value> bucket_data;
			typedef typename bucket_data::iterator bucket_iterator;
			bucket_data data;
			mutable boost::shared_mutex mutex;
			bucket_iterator find_entry_for(const Key &key) {
				return std::find_if(data.begin(), data.end(), [&](const bucket_value &item)->bool {
					return item.first == key;
				});
			}
		public:
			Value value_for(const Key &key, const Value &default_value) const {
				boost::shared_lock_guard<boost::shared_mutex> lock(mutex);
				const bucket_iterator found_entry = find_entry_for(key);
				return (found_entry == data.end()) ? default_value : found_entry->second;
			}
			void add_or_update_mapping(const Key &key, const Value &value) {
				std::unique_lock<boost::shared_mutex> lock(mutex);
				const bucket_iterator found_entry = find_entry_for(key);
				if (found_entry == data.end())
					data.push_back(bucket_value(key, value));
				else
					found_entry->second = value;
			}
			void remove_mapping(const Key &key) {
				std::unique_lock<boost::shared_mutex> lock(mutex);
				const bucket_iterator found_entry = find_entry_for(key);
				if (found_entry != data.end())
					data.erase(found_entry);
			}
		};
		std::vector<std::unique_ptr<bucket_type>> buckets;
		Hash hasher;
		bucket_type &get_bucket(const Key &key) const {
			const std::size_t bucket_index = hasher(key) % buckets.size();
			return *buckets[bucket_index];
		}
	public:
		typedef Key key_type;
		typedef Value mapped_type;
		typedef Hash hash_type;
		threadsafe_lookup_table(const threadsafe_lookup_table&) = delete;
		threadsafe_lookup_table &operator=(const threadsafe_lookup_table&) = delete;
		threadsafe_lookup_table(unsigned int num_buckets = 19, const Hash &hasher_ = Hash()) :buckets(num_buckets), hasher(hasher_) {
			for (unsigned int i = 0; i != num_buckets; ++i) {
				buckets[i].reset(new bucket_type);
			}
		}
		Value value_for(const Key &key, const Value &default_value = Value()) const {
			return get_bucket(key).value_for(key, default_value);
		}
		void add_or_update_mapping(const Key &key, const Value &value) {
			get_bucket(key).add_or_update_mapping(key, value);
		}
		void remove_mapping(const Key &key) {
			get_bucket(key).remove_mapping(key);
		}
		std::map<Key, Value> get_map() const {
			std::vector<std::unique_lock<boost::shared_mutex>> locks;
			for (unsigned int i = 0; i != buckets.size(); ++i) {
				locks.push_back(std::unique_lock<boost::shared_mutex>(buckets[i].mutex));
			}
			std::map<Key, Value> res;
			for (unsigned int i = 0; i != buckets.size(); ++i) {
				for (bucket_iterator it = buckets[i].data.begin(); it != buckets[i].data.end(); ++it) {
					res.insert(*it);
				}
			}
			return res;
		}

	};



	void threadsafelist();
	template<typename T> class threadsafe_list {
	private:
		struct node {
			std::mutex m;
			std::shared_ptr<T> data;
			std::unique_ptr<node> next;
			node():next(){}
			node(const T &value):data(std::make_shared<T>(value)){}
		};
		node head;
	public:
		threadsafe_list(){}
		~threadsafe_list() {
			remove_if([](node const &) {
				return true;
			});
		}
		threadsafe_list(const threadsafe_list&) = delete;
		threadsafe_list &operator=(const threadsafe_list&) = delete;
		void push_front(const T &value) {
			std::unique_ptr<node> new_node(new node(value));
			std::lock_guard<std::mutex> lk(head.m);
			new_node->next = std::move(head.next);
			head.next = std::move(new_node);
		}
		template<typename Function> void for_each(Function f) {
			node *current = &head;
			std::unique_lock<std::mutex> lk(head.m);
			while (node *const next = current->next.get()) {
				std::unique_lock<std::mutex> next_lk(next->m);
				lk.unlock();
				f(*next->data);
				current = next;
				lk = std::move(next_lk);
			}
		}
		template<typename Predicate> std::shared_ptr<T> find_first_if(Predicate p) {
			node *current = &head;
			std::unique_lock<std::mutex> lk(head.m);
			while (node *const next = current->next.get()) {
				std::unique_lock<std::mutex> next_lk(next->m);
				lk.unlock();
				if (p(*next->data)) {
					std::cout << "sldfk:   " << (*next->data) << std::endl;
					return next->data;
				}
				current = next;
				lk = std::move(next_lk);
			}
			return std::shared_ptr<T>();
		}
		template<typename Predicate> void remove_if(Predicate p) {
			node *current = &head;
			std::unique_lock<std::mutex> lk(head.m);
			while (node *const next = current->next.get()) {
				std::unique_lock<std::mutex> next_lk(next->m);
				if (p(*next->data)) {
					std::unique_ptr<node> old_next = std::move(current->next);
					current->next = std::move(next->next);
					next_lk.unlock();
				}
				else {
					lk.unlock();
					current = next;
					lk = std::move(next_lk);
				}
			}
		}
	};
}


#endif

namespace LOCKSTRUCT {

	template<typename T> threadsafe_stack<T>::threadsafe_stack() {

	}

	template<typename T> threadsafe_stack<T>::threadsafe_stack(const threadsafe_stack &other) {
		std::lock_guard<std::mutex> lock(other.m);
		data = other.data;
	}

	template<typename T> void threadsafe_stack<T>::push(T new_value) {
		std::lock_guard<std::mutex> lock(m);
		data.push(std::move(new_value));
	}

	template<typename T> std::shared_ptr<T> threadsafe_stack<T>::pop1() {
		std::lock_guard<std::mutex> lock(m);
		if (data.empty())
			throw empty_stack();
		const std::shared_ptr<T> res(std::make_shared<T>(std::move(data.top())));
		data.pop();
		return res;
	}

	template<typename T> void threadsafe_stack<T>::pop2(T &value) {
		std::lock_guard<std::mutex> lock(m);
		if (data.empty())
			throw empty_stack();
		value = std::move(data.top());
		data.pop();
	}

	template<typename T> bool threadsafe_stack<T>::empty() const {
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}

}

namespace LOCKSTRUCT {

	template<typename T> threadsafe_queue<T>::threadsafe_queue() {

	}

	template<typename T> void threadsafe_queue<T>::push(T new_value) {
		std::shared_ptr<T> data(std::make_shared<T>(std::move(new_value)));
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(data);
		data_cond.notify_one();
	}

	template<typename T> void threadsafe_queue<T>::wait_and_pop1(T &value) {
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this]()->bool {
			return !data_queue.empty();
		});
		value = std::move(*data_queue.front());
		data_queue.pop();
	}

	template<typename T> std::shared_ptr<T> threadsafe_queue<T>::wait_and_pop2() {
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this]()->bool {
			return !data_queue.empty();
		});
		const std::shared_ptr<T> res = data_queue.front();
		data_queue.pop();
		return res;
	}

	template<typename T> bool threadsafe_queue<T>::try_pop1(T &value) {
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return false;
		value = std::move(*data_queue.front());
		data_queue.pop();
		return true;
	}

	template<typename T> std::shared_ptr<T> threadsafe_queue<T>::try_pop2() {
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return std::shared_ptr<T>();
		const std::shared_ptr<T> res = data_queue.front();
		data_queue.pop();
		return res;
	}

	template<typename T> bool threadsafe_queue<T>::empty() const {
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.empty();

	}

}


namespace LOCKSTRUCT {

	template<typename T> mqueue<T>::node::node(T data_) :data(data_) {

	}

	template<typename T> std::shared_ptr<T> mqueue<T>::try_pop() {
		if (!head)
			return std::shared_ptr<T>();
		const std::shared_ptr<T> res(std::make_shared<T>(std::move(head->data)));
		const std::unique_ptr<node> old_head = std::move(head);
		head = std::move(old_head->next);
		return res;
	}

	template<typename T> void mqueue<T>::push(T new_value) {
		std::unique_ptr<node> p(new node(std::move(new_value)));
		node *const new_tail = p.get();
		if (tail)
			tail->next = std::move(p);
		else
			head = std::move(p);
		tail = new_tail;
	}

}

namespace LOCKSTRUCT {

	template<typename T> lqueue<T>::lqueue() :head(new node), tail(head.get()) {

	}

	template<typename T> std::shared_ptr<T> lqueue<T>::try_pop() {
		if (head.get() == tail)
			return std::shared_ptr<T>();
		const std::shared_ptr<T> res(head->data);
		std::unique_ptr<node> old_head = std::move(head);
		head = std::move(old_head->next);
		return res;
	}

	template<typename T> void lqueue<T>::push(T new_value) {
		std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
		std::unique_ptr<node> p(new node);
		tail->data = new_data;
		node *const new_tail = p.get();
		tail->next = std::move(p);
		tail = new_tail;
	}

}

namespace LOCKSTRUCT {

	template<typename T> threadsafe_lock<T>::threadsafe_lock() :head(new node), tail(head.get()) {

	}

	template<typename T> std::shared_ptr<T> threadsafe_lock<T>::try_pop1() {
		std::unique_ptr<node> old_head = try_pop_head();
		return old_head ? old_head->data : std::shared_ptr<T>();
	}

	template<typename T> bool threadsafe_lock<T>::try_pop2(T &value) {
		std::unique_ptr<node> const old_head = try_pop_head(value);
		return old_head;
	}

	template<typename T> void threadsafe_lock<T>::push(T new_value) {
		std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
		std::unique_ptr<node> p(new node);
		{
			std::lock_guard<std::mutex> tail_lock(tail_mutex);
			tail->data = new_data;
			node *const new_tail = p.get();
			tail->next = std::move(p);
			tail = new_tail;
		}
		data_cond.notify_one();
	}
	template<typename T> std::shared_ptr<T> threadsafe_lock<T>::wait_and_pop1() {
		std::unique_ptr<node> const old_head = wait_pop_head();
		return old_head->data;
	}
	template<typename T> void threadsafe_lock<T>::wait_and_pop2(T &value) {
		std::unique_ptr<node> const old_head = wait_pop_head(value);
	}

	template<typename T> bool threadsafe_lock<T>::empty() const {
		std::lock_guard<std::mutex> head_lock(head_mutex);
		return (head.get() == get_tail());
	}
}
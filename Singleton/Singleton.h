#ifndef SINGLETON_H
#define SINGLETON_H

#include<mutex>
#include<cstdlib>
#include<functional>

namespace SINGLETON {

	template<typename T> class Singleton {
	private:
		friend T;
		static std::mutex ComMutex;
		Singleton(const Singleton&) = delete;
		Singleton &operator=(const Singleton&) = delete;
		void Destroy() {
			if (m_pInstance != nullptr)
				delete m_pInstance;
			m_pInstance = nullptr;
		}
		static T *volatile m_pInstance;
	protected:
		Singleton() {}
		~Singleton() {}

	public:
		
		static T &Instance() {
			if (m_pInstance == nullptr) {
				std::lock_guard<std::mutex> guard(ComMutex);
				if (m_pInstance == nullptr) {
					m_pInstance = new T();
					atexit([]()->void {
						if (m_pInstance != nullptr)
							delete m_pInstance;
						m_pInstance = nullptr;
					});
				}
				return *m_pInstance;
			}
			return *m_pInstance;
		}
	};


	template<typename T> bool ope() { return false; }

	class Cyc :public Singleton<Cyc> {
	private:
		//friend class Singleton<Cyc>;
		//template<typename T> friend class Singleton;
		//template<typename T> friend T &Singleton<T>::Instance();
		//friend bool ope<Cyc>();
		//template<typename T> friend bool ope<T>();
		friend Cyc &Singleton<Cyc>::Instance();
		Cyc(){}
	public:
		~Cyc(){}
	};

}


#endif

template<typename T> T *volatile SINGLETON::Singleton<T>::m_pInstance = nullptr;

template<typename T> std::mutex SINGLETON::Singleton<T>::ComMutex;
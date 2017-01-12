#ifndef SINGLETON_H
#define SINGLETON_H

namespace SINGLETON {
	template<typename T> class Singleton {
	private:
		Singleton(const Singleton&) = delete;
		Singleton &operator=(const Singleton&) = delete;
		void Destroy() {
			if (m_pInstance != nullptr)
				delete m_pInstance;
			m_pInstance = nullptr;
		}
		

	protected:
		Singleton() {}
		~Singleton() {}

	public:
		static T *volatile m_pInstance;
		static T &Instance() {
			if (m_pInstance == nullptr) {
				//Lock lock;
				if (m_pInstance == nullptr) {
					m_pInstance = new T();
					//atexit(Destroy);
				}
				return *m_pInstance;
			}
			return *m_pInstance;
		}
	};


	class Cyc :public Singleton<Cyc> {
	public:
		Cyc(){
		}
		~Cyc(){}
	};

}


#endif

template<typename T> T *volatile SINGLETON::Singleton<T>::m_pInstance = nullptr;
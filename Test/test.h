#ifndef TEST_H
#define TEST_H

namespace TEST{

	template<typename T> class SupTest{

	public:
		static T Instance(){
			return T();
		}

	};

	class Test:public SupTest<Test>{


	private:
		Test(){}
		friend Test SupTest<Test>::Instance();
		friend Test SupTest::Instance();
		friend class SupTest;
		friend class SupTest<Test>;
		
	public:
		~Test(){}
	};


}

#endif
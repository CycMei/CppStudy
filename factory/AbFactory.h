#ifndef ABFACTORY_H
#define ABFACTORY_H

#include<string>

namespace ABFACTORY {

	class AbFactory {
	public:
		virtual void prepare() = 0;
	};

	class AbFactory1 :public AbFactory {
	public:
		virtual void prepare() override{}
	};

	class AbFactory2 :public AbFactory {
	public:
		virtual void prepare() override {}
	};





	class Pizza {

	};

	class Pizza1 :public Pizza {
	public:
		explicit Pizza1(AbFactory1 &af){
			af.prepare();
		}
	};


	class Pizza2 :public Pizza {
	public:
		explicit Pizza2(AbFactory2 &af) {
			af.prepare();
		}
	};







	class PizzaStore {
	public:
		Pizza orderPizza(const std::string &type) {
			return createPizza(type);
		}
	protected:
		virtual Pizza createPizza(const std::string &type) = 0;
	};

	class PizzaStore1 :public PizzaStore {
	protected:
		virtual Pizza createPizza(const std::string &type) override {
			AbFactory1 af1 = AbFactory1();
			return Pizza1(af1);
		}
	};

	class PizzaStore2 :public PizzaStore {
	protected:
		virtual Pizza createPizza(const std::string &type) override {
			AbFactory2 af2 = AbFactory2();
			return Pizza2(af2);
		}
	};



}

#endif
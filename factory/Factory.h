#ifndef FACTORY_H
#define FACTORY_H

#include<string>

namespace FACTORY {

	class Pizza {

	};

	class Pizza1 :public Pizza {

	};


	class Pizza2 :public Pizza {

	};







	class PizzaStore {
	public:
		Pizza orderPizza(const std::string &type) {
			return createPizza(type);
		}
	protected:
		virtual Pizza createPizza(const std::string &type) = 0;
	};

	class PizzaStore1:public PizzaStore {
	protected:
		virtual Pizza createPizza(const std::string &type) override{
			return Pizza1();
		}
	};

	class PizzaStore2:public PizzaStore {
	protected:
		virtual Pizza createPizza(const std::string &type) override {
			return Pizza2();
		}
	};

}

#endif
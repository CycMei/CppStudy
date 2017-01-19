#ifndef SIMPLEFACTORY_H
#define SIMPLEFACTORY_H

#include<string>

namespace SIMPLEFACTORY {

	class Pizza {

	};

	class SimpleFactory {
	public:
		Pizza createPizza(const std::string &type){
			return Pizza();
		}
	};

	class PizzaStore {
	private:
		SimpleFactory sf;
	public:
		explicit PizzaStore(const SimpleFactory &sf):sf(sf){
		
		}
		Pizza orderPizza(const std::string &type) {
			return sf.createPizza(type);
		}
	};



}


#endif
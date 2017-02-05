#include "atm.h"

namespace ATM {

	withdraw::withdraw(const std::string & account_, unsigned amount_, messaging::sender atm_queue_) :account(account_), amount(amount_), atm_queue(atm_queue_) {

	}

}

namespace ATM {

	cancel_withdrawal::cancel_withdrawal(const std::string & account_, unsigned amount_) :account(account_), amount(amount_) {

	}

}

namespace ATM {

	withdrawal_processed::withdrawal_processed(const std::string & account_, unsigned amount_) :account(account_), amount(amount_) {

	}

}

namespace ATM {

	card_inserted::card_inserted(const std::string & account_) :account(account_) {

	}

}

namespace ATM {

	digit_pressed::digit_pressed(char digit_) :digit(digit_) {

	}

}

namespace ATM {

	withdraw_pressed::withdraw_pressed(unsigned int amount_) :amount(amount_) {

	}

}

namespace ATM {

	issue_money::issue_money(unsigned int amount_) :amount(amount_) {

	}

}

namespace ATM {

	verify_pin::verify_pin(const std::string &account_, const std::string & pin_, messaging::sender atm_queue_) :account(account_), pin(pin_), atm_queue(atm_queue_) {

	}

}

namespace ATM {

	get_balance::get_balance(const std::string & account_, messaging::sender atm_queue_) :account(account_), atm_queue(atm_queue_) {

	}

}

namespace ATM {

	balance::balance(unsigned int amount_) :amount(amount_) {

	}

}

namespace ATM {

	display_balance::display_balance(unsigned int amount_) :amount(amount_) {

	}

}





namespace ATM {

	void atm::process_withdrawal() {
		incoming.wait()
		.handle<widthdraw_ok>([&](widthdraw_ok const &msg) {
			interface_hardware.send(issue_money(withdrawal_amount));
			bank.send(withdrawal_processed(account, withdrawal_amount));
			state = &atm::done_processing;
		})
		.handle<withdraw_denied>([&](withdraw_denied const &msg) {
			interface_hardware.send(display_insufficient_funds());
			state = &atm::done_processing;
		})
		.handle<cancel_pressed>([&](cancel_pressed const &msg) {
			bank.send(cancel_withdrawal(account, withdrawal_amount));
			interface_hardware.send(display_withdrawal_cancelled());
			state = &atm::done_processing;
		});
	}

	void atm::process_balance() {
		incoming.wait()
		.handle<balance>([&](balance const &msg) {
			interface_hardware.send(display_balance(msg.amount));
			state = &atm::wait_for_action;
		})
		.handle<cancel_pressed>([&](cancel_pressed const &msg) {
			state = &atm::done_processing;
		});
	}

	void atm::wait_for_action() {
		interface_hardware.send(display_withdrawal_options());
		incoming.wait()
		.handle<withdraw_pressed>([&](withdraw_pressed const &msg) {
			withdrawal_amount = msg.amount;
			bank.send(withdraw(account, msg.amount, incoming));
			state = &atm::process_withdrawal;
		})
		.handle<balance_pressed>([&](balance_pressed const &msg) {
			bank.send(get_balance(account, incoming));
			state = &atm::process_balance;
		})
		.handle<cancel_pressed>([&](cancel_pressed const &msg) {
			state = &atm::done_processing;
		});
	}

	void atm::verifying_pin() {
		incoming.wait()
		.handle<pin_verified>([&](pin_verified const &msg) {
			state = &atm::wait_for_action;
		})
		.handle<pin_incorrect>([&](pin_incorrect const &msg) {
			interface_hardware.send(display_pin_incorrect_message());
			state = &atm::done_processing;
		})
		.handle<cancel_pressed>([&](cancel_pressed const &msg) {
			state = &atm::done_processing;
		});
	}

	void atm::getting_pin() {
		incoming.wait()
		.handle<digit_pressed>([&](digit_pressed const &msg) {
			const unsigned int pin_length = 4;
			pin += msg.digit;
			if (pin.length() == pin_length) {
				bank.send(verify_pin(account, pin, incoming));
				state = &atm::verifying_pin;
			}
		})
		.handle<clear_last_pressed>([&](clear_last_pressed const &msg) {
			if (!pin.empty()) {
				pin.pop_back();
			}
		})
		.handle<cancel_pressed>([&](cancel_pressed const &msg) {
			state = &atm::done_processing;
		});
	}

	void atm::waiting_for_card() {
		interface_hardware.send(display_enter_card());
		incoming.wait()
		.handle<card_inserted>([&](card_inserted const &msg) {
			account = msg.account;
			pin = "";
			interface_hardware.send(display_enter_pin());
			state = &atm::getting_pin;
		});
	}

	void atm::done_processing() {
		interface_hardware.send(eject_card());
		state = &atm::waiting_for_card;
	}

	atm::atm(sender bank_, sender interface_handware_) :bank(bank_), interface_hardware(interface_handware_), withdrawal_amount(0), state(nullptr){

	}

	void atm::done() {
		get_sender().send(close_queue());
	}

	void atm::run() {
		state = &atm::waiting_for_card;
		try {
			for (;;)
				(this->*state)();
		}
		catch (close_queue const &) {

		}
	}

	sender atm::get_sender() {
		return incoming;
	}

}






namespace ATM {

	bank_machine::bank_machine() :balance(199) {

	}

	void bank_machine::done() {
		get_sender().send(close_queue());
	}

	void bank_machine::run() {
		try {
			for (;;) {
				incoming.wait()
				.handle<verify_pin>([&](verify_pin const &msg) {
					std::cout << "one one one " << std::endl;
					if (msg.pin == "1937") {
						msg.atm_queue.send(pin_verified());
					}
					else {
						msg.atm_queue.send(pin_incorrect());
					}
				})
				.handle<withdraw >([&](withdraw const &msg) {
					std::cout << "two two two " << std::endl;
					if (this->balance >=msg.amount ) {
						msg.atm_queue.send(widthdraw_ok());
						this->balance -= (msg.amount);
					}
					else {
						msg.atm_queue.send(withdraw_denied());
					}
				})
				.handle<get_balance>([&](get_balance const &msg) {
					std::cout << "three three three " << std::endl;
					msg.atm_queue.send(ATM::balance(balance));
				})
				.handle<withdrawal_processed>([&](withdrawal_processed const &msg) {
					std::cout << "four four four four" << std::endl;
				})
				.handle<cancel_withdrawal>([&](cancel_withdrawal const &msg) {
					std::cout << "fiva five five five " << std::endl;
				});
			}

		}
		catch (close_queue const &) {

		}
	}

	sender bank_machine::get_sender() {
		return incoming;
	}

}



namespace ATM {

	void interface_machine::done() {
		get_sender().send(close_queue());
	}

	void interface_machine::run() {
		try {
			for (;;) {
				incoming.wait()
				.handle<issue_money>([&](issue_money const &msg) {
					{
						std::lock_guard<std::mutex> lk(iom);
						std::cout << "Issuing " << msg.amount << std::endl;
					}
				})
				.handle<display_insufficient_funds>([&](display_insufficient_funds const &msg) {
					{
						std::lock_guard<std::mutex> lk(iom);
						std::cout << "insufficient funds  " << std::endl;
					}
				})
				.handle<display_enter_pin>([&](display_enter_pin const &msg) {
					{
						std::lock_guard<std::mutex> lk(iom);
						std::cout << "please enter you pin   " << std::endl;
					}
				})
				.handle<display_enter_card>([&](display_enter_card const &msg) {
					{
						std::lock_guard<std::mutex> lk(iom);
						std::cout << " plear you cards..  " << std::endl;
					}
				})
				.handle<display_balance>([&](display_balance const &msg) {
					{
						std::lock_guard<std::mutex> lk(iom);
						std::cout << "the balance of your account is " << msg.amount << std::endl;
					}
				})
				.handle<display_withdrawal_options>([&](display_withdrawal_options const &msg) {
					{
						std::lock_guard<std::mutex> lk(iom);
						std::cout << "withdraw 50ds  " << std::endl;
					}
				})
				.handle<display_withdrawal_cancelled>([&](display_withdrawal_cancelled const &msg) {
					{
						std::lock_guard<std::mutex> lk(iom);
						std::cout << "wiflksldf   " << std::endl;
					}
				})
				.handle<display_pin_incorrect_message>([&](display_pin_incorrect_message const &msg) {
					{
						std::lock_guard<std::mutex> lk(iom);
						std::cout << "dkfjl pin   " << std::endl;
					}
				})
				.handle<eject_card>([&](eject_card const &msg) {
						{
							std::lock_guard<std::mutex> lk(iom);
							std::cout << "ejecti ng dlsfcardld " << std::endl;
						}
				});
			}
		}
		catch (close_queue &) {

		}
	}

	sender interface_machine::get_sender() {
		return incoming;
	}
}



namespace ATM {
	void atmMain() {
		bank_machine bank;
		interface_machine interface_hardware;
		atm machine(bank.get_sender(), interface_hardware.get_sender());

		std::thread bank_thread(&bank_machine::run, &bank);

		std::thread if_thread(&interface_machine::run, &interface_hardware);

		std::thread atm_thread(&atm::run, &machine);

		messaging::sender atmqueue(machine.get_sender());

		bank_thread.join();
		if_thread.join();
		atm_thread.join();
	}
}
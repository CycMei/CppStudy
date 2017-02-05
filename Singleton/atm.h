#ifndef ATM_H
#define ATM_H

#include<string>
#include<iostream>

#include"messaging.h"

namespace ATM {
	struct withdraw {
		std::string account;
		unsigned int amount;
		mutable messaging::sender atm_queue;
		withdraw(const std::string &account_, unsigned amount_, messaging::sender atm_queue_);
	};

	struct widthdraw_ok {

	};

	struct withdraw_denied {

	};

	struct cancel_withdrawal {
		std::string account;
		unsigned amount;
		cancel_withdrawal(const std::string &account_, unsigned amount_);
	};

	struct withdrawal_processed {
		std::string account;
		unsigned amount;
		withdrawal_processed(const std::string &account_, unsigned amount_);
	};

	struct card_inserted {
		std::string account;
		explicit card_inserted(const std::string &account_);
	};

	struct digit_pressed {
		char digit;
		explicit digit_pressed(char digit_);
	};

	struct clear_last_pressed {

	};

	struct eject_card {

	};

	struct withdraw_pressed {
		unsigned int amount;
		explicit withdraw_pressed(unsigned int amount_);
	};

	struct cancel_pressed {

	};

	struct issue_money {
		unsigned int amount;
		explicit issue_money(unsigned int amount_);
	};


	struct verify_pin {
		std::string account;
		std::string pin;
		mutable messaging::sender atm_queue;
		verify_pin(const std::string &account_, const std::string &pin_, messaging::sender atm_queue_);
	};
	
	struct pin_verified {

	};

	struct pin_incorrect {

	};

	struct display_enter_pin {

	};

	struct display_enter_card {

	};

	struct display_insufficient_funds {

	};

	struct display_withdrawal_cancelled {

	};

	struct display_pin_incorrect_message {

	};

	struct display_withdrawal_options {

	};

	struct get_balance {
		std::string account;
		mutable messaging::sender atm_queue;
		get_balance(const std::string &account_, messaging::sender atm_queue_);
	};

	struct balance {
		unsigned int amount;
		explicit balance(unsigned int amount_);
	};

	struct display_balance {
		unsigned int amount;
		explicit display_balance(unsigned int amount_);
	};

	struct balance_pressed {

	};

}


namespace ATM {
	using namespace messaging;
	class atm {
	private:
		receiver incoming;
		sender bank;
		sender interface_hardware;
		void (atm::*state)();
		std::string account;
		unsigned withdrawal_amount;
		std::string pin;
		void process_withdrawal();
		void process_balance();
		void wait_for_action();
		void verifying_pin();
		void getting_pin();
		void waiting_for_card();
		void done_processing();
		atm(const atm&) = delete;
		atm &operator=(const atm&) = delete;
	public:
		atm(sender bank_, sender interface_handware_);
		void done();
		void run();
		sender get_sender();
	};
}

namespace ATM {
	class bank_machine {
	private:
		receiver incoming;
		unsigned int balance;
	public:
		bank_machine();
		void done();
		void run();
		sender get_sender();
	};
}


namespace ATM {

	class interface_machine {
	private:
		std::mutex iom;
		receiver incoming;
	public:
		void done();
		void run();
		sender get_sender();
	};

}

namespace ATM {
	void atmMain();
}

#endif
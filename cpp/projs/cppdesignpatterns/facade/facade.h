#include "hdr.h"
enum class ItemState {
	INIT,
	INVENTORY_SEARCH,
	INVENTORY_CHECK_VALID_ITEM,
	INVENTORY_CHECK_ITEM_EXPARY,
	INVENTORY_CHECK_ITEM_COVER,
	INVENTORY_ITEM_SELECTED,

	VERIFICATION_RECIEVED,
	VERIFICATION_CHECKING,
	VERIFICATION_SENT_LAB,
	VERIFICATION_RECVED_LAB,
	VERIFICATION_VERIFIED,

	PREPARE_RECIEVED,
	PREPARE_PREPARED,
	PREPARE_WRAPPED,

	DISPATCH_RECIEVED,
	DISPATCH_CONTACT_DELEVERY_AGENT,
	DISPATCH_WAITING_PICKUP,
	DISPATCH_CONTACT_HAND_OVER,
	DISPATCH_DISPATCHED,

	COMPLETE
};
enum class operation {
	InProgress, Done
};

class itemDeleverySystem {
public:
	void moveStageNext(int &state) {
		state++;
	}
private:
	int _state;
};

class inventoryCheck : public itemDeleverySystem {
public:
	void submit() {
		_state = static_cast<int>(ItemState::INIT);
		exec();
	}
	bool exec() {
		std::future<bool> ft = std::async (&inventoryCheck::execStage, this);
		//bool rs = ft.get();
		return true;
	}
	bool execStage() {
		while (stateList[_state] != ItemState::COMPLETE) {
			std::cout << __PRETTY_FUNCTION__ << "State : " << _state << std::endl;
			_op = operation::InProgress;
			moveStageNext(_state);
			_op = operation::Done;
		}
		return true;
	}

	bool checkStatusDone() {
		if (stateList[_state] == ItemState::COMPLETE)
			return true;
		return false;
	}

private:
	std::vector<ItemState> stateList = {
			ItemState::INIT,
			ItemState::INVENTORY_SEARCH,
			ItemState::INVENTORY_CHECK_VALID_ITEM,
			ItemState::INVENTORY_CHECK_ITEM_EXPARY,
			ItemState::INVENTORY_CHECK_ITEM_COVER,
			ItemState::INVENTORY_ITEM_SELECTED,
			ItemState::COMPLETE
		};
	int _state;
	operation _op;
};

class itemVerification : public itemDeleverySystem {
public:
	void submit() {
		_state = static_cast<int>(ItemState::INIT);
		exec();
    }
	bool exec() {
		std::future<bool> ft = std::async (&itemVerification::execStage, this);
		return true;
	}
	bool execStage() {
		while (stateList[_state] != ItemState::COMPLETE) {
			std::cout << __PRETTY_FUNCTION__ << "State : " << _state << std::endl;
			_op = operation::InProgress;
			moveStageNext(_state);
			_op = operation::Done;
		}
		return true;
	}
	bool checkStatusDone() {
		if (stateList[_state] == ItemState::COMPLETE)
			return true;
		return false;
	}
private:
	std::vector<ItemState> stateList = {
		ItemState::INIT,
		ItemState::VERIFICATION_RECIEVED,
		ItemState::VERIFICATION_CHECKING,
		ItemState::VERIFICATION_SENT_LAB,
		ItemState::VERIFICATION_RECVED_LAB,
		ItemState::VERIFICATION_VERIFIED,
		ItemState::COMPLETE
	};
	int _state;
	operation _op;
};

class itemPrepare : public itemDeleverySystem {
public:
    void submit() {
		_state = static_cast<int>(ItemState::INIT);
		exec();
    }
	bool exec() {
		std::future<bool> ft = std::async (&itemPrepare::execStage, this);
		return true;
	}
	bool execStage() {
		while (stateList[_state] != ItemState::COMPLETE) {
			std::cout << __PRETTY_FUNCTION__ << "State : " << _state << std::endl;
			_op = operation::InProgress;
			moveStageNext(_state);
			_op = operation::Done;
		}
		return true;
	}
	bool checkStatusDone() {
		if (stateList[_state] == ItemState::COMPLETE)
			return true;
		return false;
	}
private:
	std::vector<ItemState> stateList = {
		ItemState::INIT,
		ItemState::PREPARE_RECIEVED,
		ItemState::PREPARE_PREPARED,
		ItemState::PREPARE_WRAPPED,
		ItemState::COMPLETE
	};
	int _state;
	operation _op;
};

class itemDispatch : public itemDeleverySystem {
public:
    void submit() {
		_state = static_cast<int>(ItemState::INIT);
		exec();
    }
	bool exec() {
		std::future<bool> ft = std::async (&itemDispatch::execStage, this);
		return true;
	}
	bool execStage() {
		while (stateList[_state] != ItemState::COMPLETE) {
			std::cout << __PRETTY_FUNCTION__ << "State : " << _state << std::endl;
			_op = operation::InProgress;
			moveStageNext(_state);
			_op = operation::Done;
		}
		return true;
	}
	bool checkStatusDone() {

		if (stateList[_state] == ItemState::COMPLETE)
			return true;
		return false;
	}
private:
	std::vector<ItemState> stateList = {
		ItemState::INIT,
		ItemState::DISPATCH_RECIEVED,
		ItemState::DISPATCH_CONTACT_DELEVERY_AGENT,
		ItemState::DISPATCH_WAITING_PICKUP,
		ItemState::DISPATCH_CONTACT_HAND_OVER,
		ItemState::DISPATCH_DISPATCHED,
		ItemState::COMPLETE
	};
	int _state;
	operation _op;
};

class ItemFacade {
public:
	ItemFacade() {
		_state = Init;
	}
	void submit() {
        _state = Init;
    }
    bool checkStatusDone() {
		std::cout << __PRETTY_FUNCTION__ << "Facade Current State : " << _state << std::endl;
		if (_state == Init) {
			int s = static_cast<int>(_state);
			s++;
			_state = static_cast<enum state>(s);
			inv.submit();
		} else if (_state == Inventory){
			if (inv.checkStatusDone()) {
				int s = static_cast<int>(_state);
				s++;
				_state = static_cast<enum state>(s);
				verify.submit();
			}
		} else if (_state == ItemVerify){
			if (verify.checkStatusDone()) {
				int s = static_cast<int>(_state);
                s++;
                _state = static_cast<enum state>(s);
				prepare.submit();
			}
		} else if (_state == ItemPrepare){
			if (prepare.checkStatusDone()) {
				int s = static_cast<int>(_state);
                s++;
                _state = static_cast<enum state>(s);
				dispatch.submit();
			}
		} else if (_state == itemDispatched){
			if (dispatch.checkStatusDone()) {
				int s = static_cast<int>(_state);
                s++;
                _state = static_cast<enum state>(s);
			}
		} else if (_state == Complete){
			std::cout << "Complete" << std::endl;
			return true;
		} else {
			std::cout << "Unknown State " << _state << std::endl;
			return true;
		}
        return false;
    }
private:
enum state {
        Init, Inventory, ItemVerify, ItemPrepare, itemDispatched, Complete
    };
    enum state _state;
	inventoryCheck inv;
	itemVerification verify;
	itemPrepare prepare;
	itemDispatch dispatch;
};

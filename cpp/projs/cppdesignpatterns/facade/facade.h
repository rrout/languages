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
		std::cout << __PRETTY_FUNCTION__ << "----------------------------------------------------------------------------------------------------" << std::endl;
		while (stateList[_state] != ItemState::COMPLETE) {
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
	enum state {
		Init, SearchInv, CheckValidItem, CheckExpary, CheckPackageCover,Complete
	};
	enum operation {
		InProgress, Done
	};
	std::vector<ItemState> stateList = 	{
											ItemState::INIT, ItemState::INVENTORY_SEARCH, ItemState::INVENTORY_CHECK_VALID_ITEM,
											ItemState::INVENTORY_CHECK_ITEM_EXPARY, ItemState::INVENTORY_CHECK_ITEM_COVER, ItemState::INVENTORY_ITEM_SELECTED,
											ItemState::COMPLETE
										};
	int _state;
	operation _op;
};

class itemVerification {
public:
	void submit() {
        _state = Init;
    }
    bool checkStatusDone() {
		int s = static_cast<int>(_state);
        s++;
        _state = static_cast<enum state>(s);
		std::cout << __PRETTY_FUNCTION__ << "State : " << _state << std::endl;
        if (_state == Complete)
            return true;
        return false;
	}
private:
	enum state {
        Init, ItemRecieved, InitiateVerify, SendLab, ReciveFromLab, Complete
    };
	enum state _state;
};

class itemPrepare {
public:
    void submit() {
        _state = Init;
    }
    bool checkStatusDone() {
		int s = static_cast<int>(_state);
        s++;
        _state = static_cast<enum state>(s);
		 std::cout << __PRETTY_FUNCTION__ << "State : " << _state << std::endl;
        if (_state == Complete)
            return true;
        return false;
	}
private:
enum state {
        Init, ItemRecieved, InitPrepare, WrapComplete, Complete
    };
    enum state _state;
};

class itemDispatch {
public:
    void submit() {
        _state = Init;
    }
    bool checkStatusDone() {
		 int s = static_cast<int>(_state);
        s++;
        _state = static_cast<enum state>(s);
		 std::cout << __PRETTY_FUNCTION__ << "State : " << _state << std::endl;
        if (_state == Complete)
            return true;
        return false;
	}
private:
enum state {
        Init, ItemRecieved, ContactingCouriorAgency, WaitingPickUp, HandedOver, Complete
    };
    enum state _state;
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
		std::cout << __PRETTY_FUNCTION__ << "State : " << _state << std::endl;
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

#include "hdr.h"
class inventoryCheck {
public:
	void submit() {
		_state = Init;
	}
	bool checkStatusDone() {
		int s = static_cast<int>(_state);
		s++;
		_state = static_cast<enum state>(s);
		if (_state == Complete)
			return true;
		return false;
	}
private:
	enum state {
		Init, SearchInv, CheckValidItem, CheckExpary, CheckPackageCover,Complete
	};
	enum state _state;
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

#include "hdr.h"
#include "sm.h"

fsm::fsm() {
	//stable[0][0].entryFunc = std::bind(&fsm::actS1E2entry, this, std::placeholders::_1, std::placeholders::_2);
   _state = S_1;
   _event = E_MAX;
   std::cout << "Constructor executed" << std::endl;
}

enum state fsm::getCurrentState() {
  return _state;	
}

enum event fsm::getCurrentEvent() {
  return _event;	
}

bool fsm::setCurrentStateEvent(enum state s, enum event ev) {
  _state = s;
  _event = ev;
  return true;
}
void fsm::printStateTable() {
	for(int i = 0; i < S_MAX; i++) {
		for(int j = 0; j < E_MAX; j++) {
			std::cout <<  "[ " << i << " ][ " << j << " ] = " <<
				stable[i][j].currState << " -> " <<
				stable[i][j].exitState << " (entry:): " <<
				stable[i][j].entryFunc.target_type().name() << " (exit:): " <<
				stable[i][j].exitFunc.target_type().name() << " (Valid:): " <<
				stable[i][j].valid << std::endl;
		}
	}
}

bool fsm::dispatchEvent(enum event ev) {
	enum state s = getCurrentState();
	enum event e = getCurrentEvent();
	std::cout << "Processing event " << ev << " for state " << s << std::endl << 
		"--------------------------------------------------" << std::endl;
	if (ev < E_1 || ev >= E_MAX) {
		std::cout << "invalid event" << std::endl;
		return false;
	}
	if (stable[s][ev].valid == false) {
		std::cout << "invalid event for the state" << std::endl;
		return false;
	}
	if (stable[s][ev].exitState >= S_MAX) {
		//if there is no exit state, dont honor the event
		std::cout << "Unable to derive next state for the event" << std::endl;
		return false;
	}
	// exec exit event for earlier state event
	if (s != S_MAX && e != E_MAX && stable[s][e].valid) {
		std::cout << "Entering exit event " << e << " for state " << e << std::endl;
		if (stable[s][e].exitFunc != nullptr) {
			std::cout << "Executing exit event " << std::endl;
			stable[s][e].exitFunc(s, e);
		}
	}
	if(stable[s][ev].entryFunc) {
		std::cout << "Executing entry event " << e << " for state " << ev << std::endl;
		if(false == stable[s][ev].entryFunc(s, ev)) {
			//Dont change state
			if (stable[s][e].valid && s != S_MAX && e != E_MAX) {
				if (stable[s][e].entryFunc != nullptr) {
					std::cout << "re-Executing current state entry event " << std::endl;
					stable[s][e].entryFunc(s, e);
				}
			}
		} else {
			//Change state
			s = stable[s][ev].exitState;
			setCurrentStateEvent(s, ev);
		}
	} else {
		//Change state
		s = stable[s][ev].exitState;
		setCurrentStateEvent(s, ev);
	}
	return true;
}

bool fsm::actS1E1entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}

bool fsm::actS1E2entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS1E3entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS1E4entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS1E5entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS2E1entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS2E2entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS2E3entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS2E4entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS2E5entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS3E1entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS3E2entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS3E3entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS3E4entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS3E5entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS4E1entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS4E2entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS4E3entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS4E4entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS4E5entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS5E1entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS5E2entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS5E3entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS5E4entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS5E5entry(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}



bool fsm::actS1E1exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS1E2exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS1E3exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS1E4exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS1E5exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS2E1exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS2E2exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS2E3exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS2E4exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS2E5exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS3E1exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS3E2exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS3E3exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS3E4exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS3E5exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS4E1exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS4E2exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS4E3exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS4E4exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS4E5exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS5E1exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS5E2exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS5E3exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS5E4exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}
bool fsm::actS5E5exit(enum state s, enum event ev) {
  std::cout <<  "State: " << s << " Event " << ev << std::endl;
  return true;
}

void fsm::fsmTest() {
  int a = 50;
  std::cout << "Starting Test" << std::endl;
  std::srand(static_cast<unsigned int>(std::time(nullptr)));
  while(a--) {
    int event = std::rand();
    //std::cout << "Random number : " << event << std::endl;
    event = event%5;
    dispatchEvent(static_cast<enum event>(event));
  }
}

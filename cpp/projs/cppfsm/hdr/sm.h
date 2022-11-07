#include "hdr.h"
#ifndef __SM_H__
#define __SM_H__
using namespace std::placeholders;
enum state {
	S_1,
	S_2,
	S_3,
	S_4,
	S_5,
	S_MAX
};

enum event {
	E_1,
        E_2,
        E_3,
        E_4,
        E_5,
        E_MAX
};

typedef struct state_table_s{
  bool valid;
	enum state currState;
	std::function<bool (enum state s, enum event ev)> entryFunc;
	std::function<bool(enum state s, enum event ev)> exitFunc;
	enum state exitState;
}state_table_t;

class fsm {
private:
	enum state _state;
  enum event _event;
	std::string _info;
public:
  fsm();
	state_table_t stable [S_MAX][E_MAX] = {
   {
      //{
      //{S_1,std::bind(&fsm::actS1E2entry, this, _1, std::placeholders::_2), nullptr, S_2}
      //}
      {true, S_1, std::bind(&fsm::actS1E2entry, this, _1, _2), std::bind(&fsm::actS1E2exit, this, _1, _2), S_2},
      {true, S_1, std::bind(&fsm::actS1E2entry, this, _1, _2), std::bind(&fsm::actS1E2exit, this, _1, _2), S_3},
      {true, S_1, std::bind(&fsm::actS1E2entry, this, _1, _2), std::bind(&fsm::actS1E2exit, this, _1, _2), S_4},
      {true, S_1, std::bind(&fsm::actS1E2entry, this, _1, _2), std::bind(&fsm::actS1E2exit, this, _1, _2), S_5},
      {true, S_1, std::bind(&fsm::actS1E2entry, this, _1, _2), std::bind(&fsm::actS1E2exit, this, _1, _2), S_1}
    },
    {
      {true, S_2,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_3},
      {true, S_2,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_4},
      {true, S_2,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_5},
      {true, S_2,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_1},
      {true, S_2,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_2}
    },
    {
      {true, S_3,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_4},
      {true, S_3,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_5},
      {true, S_3,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_1},
      {true, S_3,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_2},
      {true, S_3,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_3}
    },
    {
      {true, S_4,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_5},
      {true, S_4,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_1},
      {true, S_4,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_2},
      {true, S_4,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_3},
      {true, S_4,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_4}
    },
    {
      {true, S_5,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_1},
      {true, S_5,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_2},
      {true, S_5,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_3},
      {true, S_5,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_4},
      {true, S_5,std::bind(&fsm::actS1E2entry, this, _1, _2), nullptr, S_5}
    }
  };
	enum state getCurrentState();
  enum event getCurrentEvent();
  bool setCurrentStateEvent(enum state s, enum event ev);
  void printStateTable();
  void fsmTest();
	bool dispatchEvent(enum event ev);
	bool action(enum state s, enum event ev);
	bool actS1E1entry(enum state s, enum event ev);
	bool actS1E1exit(enum state s, enum event ev);
 	bool actS1E2entry(enum state s, enum event ev);
	bool actS1E2exit(enum state s, enum event ev);
 	bool actS1E3entry(enum state s, enum event ev);
	bool actS1E3exit(enum state s, enum event evt);
	bool actS1E4entry(enum state s, enum event ev);
	bool actS1E4exit(enum state s, enum event ev);
 	bool actS1E5entry(enum state s, enum event ev);
	bool actS1E5exit(enum state s, enum event evt);
	bool actS2E1entry(enum state s, enum event ev);
	bool actS2E1exit(enum state s, enum event ev);
 	bool actS2E2entry(enum state s, enum event ev);
	bool actS2E2exit(enum state s, enum event ev);
 	bool actS2E3entry(enum state s, enum event ev);
	bool actS2E3exit(enum state s, enum event ev);
	bool actS2E4entry(enum state s, enum event ev);
	bool actS2E4exit(enum state s, enum event ev);
 	bool actS2E5entry(enum state s, enum event ev);
	bool actS2E5exit(enum state s, enum event ev);
 	bool actS3E1entry(enum state s, enum event ev);
	bool actS3E1exit(enum state s, enum event ev);
 	bool actS3E2entry(enum state s, enum event ev);
	bool actS3E2exit(enum state s, enum event ev);
 	bool actS3E3entry(enum state s, enum event ev);
	bool actS3E3exit(enum state s, enum event ev);
	bool actS3E4entry(enum state s, enum event ev);
	bool actS3E4exit(enum state s, enum event ev);
 	bool actS3E5entry(enum state s, enum event ev);
	bool actS3E5exit(enum state s, enum event ev);
 	bool actS4E1entry(enum state s, enum event ev);
	bool actS4E1exit(enum state s, enum event ev);
 	bool actS4E2entry(enum state s, enum event ev);
	bool actS4E2exit(enum state s, enum event ev);
 	bool actS4E3entry(enum state s, enum event ev);
	bool actS4E3exit(enum state s, enum event ev);
	bool actS4E4entry(enum state s, enum event ev);
	bool actS4E4exit(enum state s, enum event ev);
 	bool actS4E5entry(enum state s, enum event ev);
	bool actS4E5exit(enum state s, enum event ev);
 	bool actS5E1entry(enum state s, enum event ev);
	bool actS5E1exit(enum state s, enum event ev);
 	bool actS5E2entry(enum state s, enum event ev);
	bool actS5E2exit(enum state s, enum event ev);
 	bool actS5E3entry(enum state s, enum event ev);
	bool actS5E3exit(enum state s, enum event ev);
	bool actS5E4entry(enum state s, enum event ev);
	bool actS5E4exit(enum state s, enum event ev);
 	bool actS5E5entry(enum state s, enum event ev);
	bool actS5E5exit(enum state s, enum event ev);
};

class verify {
public:
  int i = 10;
};
#endif //__SM_H__

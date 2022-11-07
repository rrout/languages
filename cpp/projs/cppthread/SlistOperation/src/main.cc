#include "hdr.h"
#include "slist.h"
#include "sthread.h"


int main() {
  slist<int> list;
  list.print();
  list.insert(1);
  list.insert(2);
  list.insert(3);
  list.insert(4);
  list.insert(5);
  list.print();
  list.pop_back();
#if 0
  //dequeue(list);
  list.print();
  //std::thread E(f1, a);
  std::thread E(enqueue<int>, std::ref(list));
  std::thread d(dequeue<int>, std::ref(list));
  auto lmda = [&](){
		  for (int i = 0; i < MAX_ACT; i++) {
                list.remove(i);
        }};
  //std::thread d(lmda);
  E.join();
  d.join();
#endif
  list.print();
  enqueue_async<int>(std::ref(list));
  list.print();
}

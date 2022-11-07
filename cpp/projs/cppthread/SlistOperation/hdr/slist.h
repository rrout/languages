#ifndef __SLIST_H__
#define __SLIST_H__
#include "hdr.h"

template <class T>
class slist_node {
public:
	T data;
	slist_node *next;
  slist_node(T data);
};

template <class T>
class slist {
public:
  std::mutex lck;
	int count;
	int insCount;
	int remCount;
  slist();
	void insert(T data);
  void push_back(T data);
  void push_front(T data);
  void pop_back();
  void pop_front();
	void remove(T data);
	void print();
private:
	slist_node<T> *root;
};

// Explicit template instantiation
#endif //__SLIST_H__

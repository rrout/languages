#include "hdr.h"
#include "slist.h"

template <class T>
slist_node<T>::slist_node(T dat) {
  data = dat;
  next = nullptr;
}

template <class T>
slist<T>::slist() {
  count = 0;
	insCount = 0;
	remCount = 0;
  root = nullptr;
  std::cout << " List Initialized.." << count << ":" << insCount << ":" << remCount << ":" << root << std::endl;
}

template <class T>
void slist<T>::insert(T data) {
  const std::lock_guard<std::mutex> lock(lck);
  auto *tmp = new slist_node<T>(data);
  if (root == nullptr) {
    root = tmp;
  } else {
    tmp->next = root;
    root = tmp;
  }
  count++;
  insCount++;
}

template <class T>
void slist<T>::push_back(T data) {
  const std::lock_guard<std::mutex> lock(lck);
  auto *tmp = new slist_node<T>(data);
  if (root == nullptr) {
    root = tmp;
  } else {
    auto *iter = root, *prev = root;;
    while (iter) {
      prev = iter;
      iter = iter->next;
    }
    prev->next = tmp;
  }
  count++;
  insCount++;
}

template <class T>
void slist<T>::push_front(T data) {
  const std::lock_guard<std::mutex> lock(lck);
  auto *tmp = new slist_node<T>(data);
  if (root == nullptr) {
    root = tmp;
  } else {
    tmp->next = root;
    root = tmp;
  }
  count++;
  insCount++;
}

template <class T>
void slist<T>::remove(T data) {
  const std::lock_guard<std::mutex> lock(lck);
  if (!root) {
    std::cout << data << " List Empty.." << std::endl;
    return;
  }
  auto *tmp = root;
  if (data == root->data) {
    root = root->next;
    delete tmp;
    count--;
    remCount++;
    return;
  }
  slist_node<T> *prev = nullptr;
  while(tmp) {
    if (tmp->data == data) {
      prev->next = tmp->next;
      delete tmp;
      count--;
      remCount++;
      return;
    }
    prev = tmp;
    tmp = tmp->next;
  }
  std::cout << data << " Not found.." << std::endl;
  return;
}

template <class T>
void slist<T>::pop_back() {
  const std::lock_guard<std::mutex> lock(lck);
  if (root == nullptr) {
    return;
  } else if (root->next == nullptr) {
    delete root;
    root = nullptr;
  }else {
    auto *iter = root, *prev = root;;
    while (iter->next) {
      prev = iter;
      iter = iter->next;
    }
    prev->next = nullptr;
    delete iter;
  }
  count--;
  remCount++;
}

template <class T>
void slist<T>::pop_front() {
  const std::lock_guard<std::mutex> lock(lck);
  if (root == nullptr) {
    return;
  } else {
    auto *tmp = root;
    root = root->next;
    delete tmp;
  }
  count--;
  remCount++;
}

template <class T>
void slist<T>::print() {
  const std::lock_guard<std::mutex> lock(lck);
  int counts = 0;
  std::stringstream ss;
  auto *tmp = root;
  std::cout << std::endl << "List [" << count << "] : [" << insCount << "] : [" << remCount << "] : [" << root << "]" << std::endl;
  std::cout << "data : " << std::endl;
  while(tmp) {
    ss << " " << tmp->data;
    tmp = tmp->next;
    if (counts%20 == 0)
	ss << std::endl;
    counts++;
  }
  std::cout << "data : " << ss.str() << std::endl;
  std::cout << "\nNumber of items : " << counts << std::endl;
}

// Explicit template instantiation
// https://bytefreaks.net/programming-2/c/c-undefined-reference-to-templated-class-function
template class slist_node<int>;
template class slist<int>;

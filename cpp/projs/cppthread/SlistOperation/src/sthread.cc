#include "hdr.h"
#include "slist.h"
#include "sthread.h"
template <typename T> void enqueue(class slist<T> &l) {
	for (int i = 0; i < MAX_ACT; i++) {
        std::cout << "Inserting " << i <<std::endl;
		//l.insert(i);
        l.push_back(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(34));
	}	
}

template <typename T> void dequeue(class slist<T> &l) {
	for (int i = 0; i < MAX_ACT; i++) {
                std::cout << "Removing " << i << std::endl;
                //l.remove(i);
                l.pop_back();
                std::this_thread::sleep_for(std::chrono::milliseconds(34));
        }
}


//template void enqueue<slist<slist_node>>(class slist<slist_node> &l); //Not working
//template void dequeue<int>(class slist<slist_node<int>> &l); //Not working
template void enqueue<int>(slist<int>&);
template void dequeue<int>(slist<int>&);

template <typename T> void enqueue_async(class slist<T> &l) {
    //enqueue(l);
    std::vector<std::future<void>> asynclist;
    for (int i = 0; i < MAX_ACT; i++) {
        std::future<void> f = std::async(std::launch::async, &enqueue<int>, std::ref(l));
        asynclist.push_back(std::move(f));
    }

    for (int i = 0; i < MAX_ACT; i++) {
        std::future<void> f = std::async(std::launch::async, &dequeue<int>, std::ref(l));
        asynclist.push_back(std::move(f));
    }

    int count = 0;
    for (auto &x : asynclist) {
        std::cout << "Iterating over Async task : " << count << std::endl;
	count++;
        x.get();
    }
    /*for (int i = 0; i < asynclist.size(); i++) {
        std::cout << "Iterating " << i << std::endl;
        asynclist[i].get();
    }*/
}
template void enqueue_async<int>(slist<int>&);

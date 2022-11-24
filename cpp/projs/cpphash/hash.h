#include "hdr.h"

template<typename T1, typename T2>
class Node {
	private:
	protected:
	public:
		T1 key;
		T2 value;
		Node() = delete;
		Node(T1 k, T2 v) {
			key = k;
			value = v;
		}
};

template<typename T1, typename T2>
class Hash {
	private:
		static const int arrsize = 500;
		std::array<std::list<Node<T1, T2> *>, arrsize> bucket;
		T1 key;
		T2 value;
	protected:
		int hashfunc(T1 ky) {
			int k = ((ky*2)%2069)%arrsize;
			return k;
		}
	public:
		Hash() = default;
		Hash(T1 k, T2 v) {

		}
		T2 * get(T1 k) {
			int index = hashfunc(k);
			if (bucket[index].size() != 0) {
				for (auto &a : bucket[index]) {
					if (a->key == k)
						return &a.value;
				}
			}
		}
		void print() {
			for( auto &x : bucket ) {
				for (auto &y : x ) {
					std::cout << "Key : " << y->key << " Value : " << y->value << std::endl;
				}
			}
		}
		bool set(T1 k, T2 v) {
			int index = hashfunc(k);
			bucket[index].push_back(new Node<T1, T2>(k,v));
			return true;
		}
};

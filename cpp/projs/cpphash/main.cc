#include "hash.h"

int main() {
	Hash<int, std::string> h;
	h.set(1, "xxxxxxxxxx");
	h.print();

	Hash<int, std::string> h2;
	h2.set(5, "xxxxxxxxxx");
	h2.print();

}

#include "hdr.h"

enum VehicleType { 
    MOPED, SEDAN, HATCHBACK, SUV 
}; 

class Veichle {
	private:

	public:
		Veichle() = default;
		virtual ~Veichle() = default;
		virtual void printVichle() {};
		static Veichle * CreateVeichle(enum VehicleType type);
};

class moped : public Veichle {
    private:

    public:
		moped() {
			std::cout << __PRETTY_FUNCTION__ << "Constructor" << std::endl;
		}
		~moped() {
            std::cout << __PRETTY_FUNCTION__ << "Destructor" << std::endl;
        }
		void printVichle() {
			std::cout << __PRETTY_FUNCTION__ << " Printed" << std::endl;
		}

};

class hatchback : public Veichle {
	private:

    public:
        hatchback() {
            std::cout << __PRETTY_FUNCTION__ << "Constructor" << std::endl;
        }
        ~hatchback() {
            std::cout << __PRETTY_FUNCTION__ << "Destructor" << std::endl;
        }
        void printVichle() {
            std::cout << __PRETTY_FUNCTION__ << " Printed" << std::endl;
        }
};

class sedan : public Veichle {
private:

    public:
        sedan() {
            std::cout << __PRETTY_FUNCTION__ << "Constructor" << std::endl;
        }
        ~sedan() {
            std::cout << __PRETTY_FUNCTION__ << "Destructor" << std::endl;
        }
        void printVichle() {
            std::cout << __PRETTY_FUNCTION__ << " Printed" << std::endl;
        }
};

class suv : public Veichle {
private:

    public:
        suv() {
            std::cout << __PRETTY_FUNCTION__ << "Constructor" << std::endl;
        }
        ~suv() {
            std::cout << __PRETTY_FUNCTION__ << "Destructor" << std::endl;
        }
        void printVichle() {
            std::cout << __PRETTY_FUNCTION__ << " Printed" << std::endl;
        }
};


Veichle * Veichle::CreateVeichle(enum VehicleType type) {
	if (type == MOPED) {
		return new moped();
	} else if (type == HATCHBACK) {
		return new hatchback();
	} else if (type == SEDAN) {
		return new sedan();
	} else if (type == SUV) {
		return new suv();
	} else {
		return nullptr;
	}
}

class VeichleClient {
	private:
		std::unique_ptr<Veichle> p;
	public:
		VeichleClient() = delete;
		VeichleClient(enum VehicleType type) {
			p = std::unique_ptr<Veichle>(Veichle::CreateVeichle(type));
		}
		void print() {
			p->printVichle();
		}
};

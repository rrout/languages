#include "hdr.h"

enum veicheletype {
	CAR,
	TRUCK,
	BUS,
	CRANE,
	BULDOZOR
};
class veicheleImpl {
	private:
	protected:
		enum veicheletype type;
		std::string color;
		int numWheels;
	public:
		veicheleImpl(enum veicheletype typ, std::string col) {
			std::cout << __PRETTY_FUNCTION__ << " Constructor" << std::endl;
			numWheels = 0;
		}
		virtual ~veicheleImpl() {
			std::cout << __PRETTY_FUNCTION__ << " Destructor" << std::endl;
		}
		virtual void display() {};
		virtual void buildVeichle() { };
};

class carImpl : public veicheleImpl {
	private:
    protected:
		//enum veicheletype type = CAR;
		//int numWheels;
		std::string model;
		//std::string color;
	public:
		carImpl(std::string mod, std::string col) : veicheleImpl(CAR, col) {
			std::cout << __PRETTY_FUNCTION__ << " Constructor" << std::endl;
			type = TRUCK;
			model = mod;
			color = col;
		}
		~carImpl() {
			std::cout << __PRETTY_FUNCTION__ << " Destructor" << std::endl;
		}
		virtual void display() {
			std::cout << type << " DISPLAY" << std::endl;
			std::cout << "Model : " << model <<  std::endl;
			std::cout << "Color : " << color << std::endl;
			std::cout << "wheels: " << numWheels << std::endl;
		}
		virtual void buildVeichle() {
			if (model == "SEDAN") {
				std::cout << "Baking " << model << std::endl;
				std::cout << "making " << model << std::endl;
				numWheels = 5;
			} else if (model == "SUV") {
				std::cout << "Baking " << model << std::endl;
                std::cout << "making " << model << std::endl;
				numWheels = 6;
			} else if (model == "HATCHBACKK") {
				std::cout << "Baking " << model << std::endl;
                std::cout << "making " << model << std::endl;
				numWheels = 4;
			} else {
				std::cout << __PRETTY_FUNCTION__ << " Unknown Model" << std::endl;
			}

		}
};

class truckImpl : public veicheleImpl {
	private:
    protected:
		std::string model;
	public:
		truckImpl(std::string mod, std::string col) : veicheleImpl(TRUCK, col) {
			std::cout << __PRETTY_FUNCTION__ << " Constructor" << std::endl;
			type = TRUCK;
			model = mod;
			color = col;
		}
		~truckImpl() {
			std::cout << __PRETTY_FUNCTION__ << " Destructor" << std::endl;
		}
		virtual void display() {
			std::cout << type << " DISPLAY" << std::endl;
			std::cout << "Model : " << model <<  std::endl;
			std::cout << "Color :" << color << std::endl;
			std::cout << "wheels: " << numWheels << std::endl;
		}
		virtual void buildVeichle() {
			if (model == "SEDAN") {
				std::cout << "Baking " << model << std::endl;
				std::cout << "making " << model << std::endl;
				numWheels = 5;
			} else if (model == "SUV") {
				std::cout << "Baking " << model << std::endl;
                std::cout << "making " << model << std::endl;
				numWheels = 6;
			} else if (model == "HATCHBACKK") {
				std::cout << "Baking " << model << std::endl;
                std::cout << "making " << model << std::endl;
				numWheels = 4;
			} else {
				std::cout << __PRETTY_FUNCTION__ << " Unknown Model" << std::endl;
			}

		}
};

// BRIDGE

class Veichle {
	private:
	protected:
		std::unique_ptr<veicheleImpl> _impl;
	public:
		Veichle() = default;
		Veichle(enum veicheletype typ, std::string col) {
			std::cout << __PRETTY_FUNCTION__ << " Constructor" << std::endl;
			_impl = std::make_unique<veicheleImpl>(typ, col);
		}
		virtual ~Veichle() {
			std::cout << __PRETTY_FUNCTION__ << " Destructor" << std::endl;
		}
		virtual void buildVeichle() {
			_impl->buildVeichle();
		}
		virtual void display() {
			_impl->display();
		}
};

class car : public Veichle {
	private:
	protected:
	public:
		car(std::string mod, std::string col) {
			std::cout << __PRETTY_FUNCTION__ << " Constructor" << std::endl;
			_impl = std::make_unique<carImpl>(mod, col);
		}
		virtual ~car() {
			std::cout << __PRETTY_FUNCTION__ << " Destructor" << std::endl;
		}
		virtual void buildVeichle() {
            _impl->buildVeichle();
        }
        virtual void display() {
            _impl->display();
        }
};

class truck : public Veichle {
    private:
    protected:
    public:
        truck(std::string mod, std::string col) {
			std::cout << __PRETTY_FUNCTION__ << " Constructor" << std::endl;
            _impl = std::make_unique<truckImpl>(mod, col);
        }
		~truck() {
			std::cout << __PRETTY_FUNCTION__ << " Destructor" << std::endl;
		}
        virtual void buildVeichle() {
            _impl->buildVeichle();
        }
        virtual void display() {
            _impl->display();
        }
};

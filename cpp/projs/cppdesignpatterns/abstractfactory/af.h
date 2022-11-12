#include "hdr.h"

class product {
	private:

	public:
		product() = default;
		virtual ~product() = default;
		virtual void engieneDetails() {};
		virtual void fuelDetails() {};
		virtual void milageDetails() {};
};

class car : public product {
	private:

    public:
        car() {
			std::cout << __PRETTY_FUNCTION__ << "Constructor" << std::endl;
		}
        ~car() {
            std::cout << __PRETTY_FUNCTION__ << "Destructor" << std::endl;
        }
        void engieneDetails() {
			std::cout << __PRETTY_FUNCTION__ << "Engiene" << std::endl;
		}
        void fuelDetails() {
			std::cout << __PRETTY_FUNCTION__ << "Fuel" << std::endl;
		}
        void milageDetails() {
			std::cout << __PRETTY_FUNCTION__ << "Milage" << std::endl;
		}
};

class truck : public product {
    private:

    public:
        truck() {
            std::cout << __PRETTY_FUNCTION__ << "Constructor" << std::endl;
        }
        ~truck() {
            std::cout << __PRETTY_FUNCTION__ << "Destructor" << std::endl;
        }
        void engieneDetails() {
            std::cout << __PRETTY_FUNCTION__ << "Engiene" << std::endl;
        }
        void fuelDetails() {
            std::cout << __PRETTY_FUNCTION__ << "Fuel" << std::endl;
        }
        void milageDetails() {
            std::cout << __PRETTY_FUNCTION__ << "Milage" << std::endl;
        }
};

class VeichleFactory {
	private:
	public:
		VeichleFactory() = default;
		virtual ~VeichleFactory() = default;
		virtual std::unique_ptr<product> crate4Wheeler() {
			std::cout << __PRETTY_FUNCTION__ << "4Wheeler" << std::endl;
			return nullptr;
		};
		virtual std::unique_ptr<product> crate6Wheeler() {
			std::cout << __PRETTY_FUNCTION__ << "6Wheeler" << std::endl;
			return nullptr;
		};
};

class HondaFactory : public	VeichleFactory {
    private:
    public:
		HondaFactory() {
            std::cout << __PRETTY_FUNCTION__ << " Constructor" << std::endl;
        }
        ~HondaFactory() {
            std::cout << __PRETTY_FUNCTION__ << " destructor" << std::endl;
        }
		std::unique_ptr<product> crate4Wheeler() {
			std::cout << __PRETTY_FUNCTION__ << "4Wheeler" << std::endl;
			return std::make_unique<car>();
		}
		std::unique_ptr<product> crate6Wheeler() {
			std::cout << __PRETTY_FUNCTION__ << "6Wheeler" << std::endl;
			return std::make_unique<truck>();
        }
};

class TataFactory : public VeichleFactory {
    private:
    public:
		TataFactory() {
			std::cout << __PRETTY_FUNCTION__ << " Constructor" << std::endl;
		}
		~TataFactory() {
			std::cout << __PRETTY_FUNCTION__ << " destructor" << std::endl;
		}
        std::unique_ptr<product> crate4Wheeler() {
			std::cout << __PRETTY_FUNCTION__ << "6Wheeler" << std::endl;
            return std::make_unique<car>();
        }
        std::unique_ptr<product> crate6Wheeler() {
			std::cout << __PRETTY_FUNCTION__ << "6Wheeler" << std::endl;
            return std::make_unique<truck>();
        }
};

class EicherFactory : public VeichleFactory {
    private:
    public:
        EicherFactory() {
            std::cout << __PRETTY_FUNCTION__ << " Constructor" << std::endl;
        }
        ~EicherFactory() {
            std::cout << __PRETTY_FUNCTION__ << " destructor" << std::endl;
        }
        std::unique_ptr<product> crate4Wheeler() {
            std::cout << __PRETTY_FUNCTION__ << "6Wheeler" << std::endl;
            return std::make_unique<car>();
        }
        std::unique_ptr<product> crate6Wheeler() {
            std::cout << __PRETTY_FUNCTION__ << "6Wheeler" << std::endl;
            return std::make_unique<truck>();
        }
};

class VeichleFactoryClient {
	private:
		std::unique_ptr<VeichleFactory> p;
		std::vector<std::unique_ptr<product>> _product;
	public:
		VeichleFactoryClient(std::unique_ptr<VeichleFactory> ptr) {
			std::cout << __PRETTY_FUNCTION__ << " Constructor Client" << std::endl;
			if(ptr)
				p = std::move(ptr);
		}
		~VeichleFactoryClient() {
			std::cout << __PRETTY_FUNCTION__ << " desstructor Client" << std::endl;
		}

		void add4Wheeler() {
			std::cout << __PRETTY_FUNCTION__ << " ADD A 4 WHEELER" << std::endl;
			if (p) {
				std::unique_ptr<product> p4whiler(p->crate4Wheeler());
				_product.push_back(std::move(p4whiler));
			}
		}
		void add6Wheeler() {
			std::cout << __PRETTY_FUNCTION__ << " ADD A 6 WHEELER" << std::endl;
			if (p) {
				std::unique_ptr<product> p6whiler(p->crate6Wheeler());
				_product.push_back(std::move(p6whiler));
			}
		}

		void printProductDetails() {
			for (auto &a : _product) {
				a->engieneDetails();
				a->fuelDetails();
				a->milageDetails();
			}
			std::cout << __PRETTY_FUNCTION__ << " PrintDetails" << std::endl;
			if (p) {
				std::cout << __PRETTY_FUNCTION__ << "Pointer PrintDetails" << std::endl;
				std::unique_ptr<product> pPtr(p->crate4Wheeler());
				pPtr->engieneDetails();
				pPtr->fuelDetails();
				pPtr->milageDetails();
			}

		}
};


#include "af.h"

int main() {
	std::unique_ptr<VeichleFactory> v = std::make_unique<HondaFactory>();
	std::unique_ptr<VeichleFactoryClient> hclient = std::make_unique<VeichleFactoryClient>(std::move(v));
	hclient->add4Wheeler();
	hclient->add6Wheeler();
	hclient->printProductDetails();

	std::unique_ptr<VeichleFactory> vv = std::make_unique<TataFactory>();
    std::unique_ptr<VeichleFactoryClient> tclient = std::make_unique<VeichleFactoryClient>(std::move(vv));
    tclient->add4Wheeler();
    tclient->add6Wheeler();
    tclient->printProductDetails();

	v = std::move(std::make_unique<EicherFactory>());
    std::unique_ptr<VeichleFactoryClient> eclient = std::make_unique<VeichleFactoryClient>(std::move(v));
    eclient->add4Wheeler();
    eclient->add6Wheeler();
    eclient->printProductDetails();

}

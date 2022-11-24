#include "hdr.h"
#include "pktdispatchconfig.h"
#include "pktpublisher.h"
#include "pktsubscriber.h"

int main() {
	std::unique_ptr<pktdispatchconfig> pktDispatch( pktdispatchconfig::createInstance());
	pktDispatch->cratePublisher(PD_TOPIC_0_TOPIC_A);
	pktDispatch->cratePublisher(PD_TOPIC_1_TOPIC_B);
	pktDispatch->cratePublisher(PD_TOPIC_2_TOPIC_C);
	pktDispatch->cratePublisher(PD_TOPIC_3_TOPIC_D);
	pktDispatch->cratePublisher(PD_TOPIC_0_TOPIC_A);

	pktDispatch->prntPublisherList();

	pktDispatch->deletePublisher(PD_TOPIC_0_TOPIC_A);
	pktDispatch->prntPublisherList();

	pktDispatch->deletePublisher(PD_TOPIC_0_TOPIC_A);
	pktDispatch->cratePublisher(PD_TOPIC_1_TOPIC_B);
	pktDispatch->cratePublisher(PD_TOPIC_0_TOPIC_A);
    pktDispatch->prntPublisherList();

	pktDispatch->addSubscriber("AAAAA", PD_TOPIC_0_TOPIC_A);
	pktDispatch->addSubscriber("AAAAA", PD_TOPIC_0_TOPIC_A);
	pktDispatch->addSubscriber("AAAAA", PD_TOPIC_1_TOPIC_B);
	pktDispatch->addSubscriber("AAAAA", PD_TOPIC_2_TOPIC_C);
	pktDispatch->delSubscriber("AAAAA", PD_TOPIC_2_TOPIC_C);
	pktDispatch->delSubscriber("AAAAA", PD_TOPIC_2_TOPIC_C);
	pktDispatch->addSubscriber("BBBBB", PD_TOPIC_0_TOPIC_A);
	pktDispatch->printSubscriberlist();

	pktDispatch->start();
	std::cout << __PRETTY_FUNCTION__ << ":" << "Exit" << std::endl;
}

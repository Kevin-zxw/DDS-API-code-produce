#include "SOAP_Subscriber.h"
#include "MessageDataReaderListenerImpl.h"
#include <dds/DCPS/Marked_Default_Qos.h>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// USING SECTION                                                             //
///////////////////////////////////////////////////////////////////////////////

using std::cerr;
using std::cout;
using std::endl;

///////////////////////////////////////////////////////////////////////////////
// CONSTANTS SECTION                                                         //
///////////////////////////////////////////////////////////////////////////////

DDS::DomainId_t SubDomainID{ 1066 };


///////////////////////////////////////////////////////////////////////////////
// PUBLIC SECTION                                                            //
///////////////////////////////////////////////////////////////////////////////

SOAP_Subscriber::SOAP_Subscriber(int argc, char* argv[]) {
  createParticipant(argc, argv);
  createSubscriber();
  registerTopic();
  createDataReader();
}

SOAP_Subscriber::~SOAP_Subscriber() {
  try {
    if (!CORBA::is_nil (m_participant.in ())) {
      m_participant->delete_contained_entities();
    }
    if (!CORBA::is_nil (m_participantFactory.in ())) {
      m_participantFactory->delete_participant(m_participant.in ());
    }
  } catch (CORBA::Exception& e) {
    cerr << "Exception caught in cleanup." << endl << e << endl;
  }
  TheServiceParticipant->shutdown();
}

void SOAP_Subscriber::setReceivedTopicFunction(std::function<void (const SoapMsg::SoapMsgerResult&)> tf) {
  auto listenerServant = dynamic_cast<MessageDataReaderListenerImpl*>(m_listener.in());
  listenerServant->setReceivedTopicFunction(tf);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE SECTION                                                           //
///////////////////////////////////////////////////////////////////////////////

void SOAP_Subscriber::createParticipant(int argc, char* argv[]) {
  auto m_participantFactory = TheParticipantFactoryWithArgs(argc, argv);

  m_participant = m_participantFactory->create_participant(
    SubDomainID,
    PARTICIPANT_QOS_DEFAULT,
    DDS::DomainParticipantListener::_nil(),
    ::OpenDDS::DCPS::DEFAULT_STATUS_MASK
  );

  if (CORBA::is_nil (m_participant.in ())) {
      cerr << "Publisher: Failed to create participant..." << endl;
  } else {
    cout << "Publisher: participant created successfully" << endl;
  }
}

void SOAP_Subscriber::createSubscriber() {
  m_subscriber = m_participant->create_subscriber(
    SUBSCRIBER_QOS_DEFAULT,
    DDS::SubscriberListener::_nil(),
    ::OpenDDS::DCPS::DEFAULT_STATUS_MASK
  );
  if (CORBA::is_nil (m_subscriber.in ())) {
    cerr << "createSubscriber failed." << endl;
  } else {
    cout << "createSubscriber ok." << endl;
  }
}

void SOAP_Subscriber::registerTopic() {
  m_topicSupport = new SoapMsg::SoapMsgerResultTypeSupportImpl();

    if (DDS::RETCODE_OK != m_topicSupport->register_type(
      m_participant.in(),
      "")
    ) {
      cerr << "Cration of register_type for " << "MessageType" << " failed." << endl;
    }
    DDS::TopicQos defaultQoS;
    CORBA::String_var type_name = m_topicSupport->get_type_name();
    m_participant->get_default_topic_qos(defaultQoS);
    m_messageTopic = m_participant->create_topic (
      "Result",
      type_name,
      defaultQoS,
      DDS::TopicListener::_nil(),
      ::OpenDDS::DCPS::DEFAULT_STATUS_MASK
    );
    if (CORBA::is_nil (m_messageTopic.in ())) {
      cerr << "Creation of create_topic for " << "MessageTopic" << " failed." << endl;
    }
}

void SOAP_Subscriber::createDataReader() {
 m_listener = new MessageDataReaderListenerImpl;
 auto listenerServant = dynamic_cast<MessageDataReaderListenerImpl*>(m_listener.in());

  if (CORBA::is_nil (m_listener.in ())) {
    cerr << "ExchangeEvent listener is nil." << endl;
  }
  DDS::DataReaderQos defaultQoS;
  m_subscriber->get_default_datareader_qos (defaultQoS);

  DDS::DataReader_var quote_dr = m_subscriber->create_datareader(
    m_messageTopic.in (),
    defaultQoS,
    m_listener.in (),
    ::OpenDDS::DCPS::DEFAULT_STATUS_MASK
  );
}

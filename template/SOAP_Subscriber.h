#ifndef SOAP_SUBSCRIBER_H_
#define SOAP_SUBSCRIBER_H_

#include "idl/SoapMsgTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <functional>

class SOAP_Subscriber {
public:

  SOAP_Subscriber(int argc, char* argv[]);
  ~SOAP_Subscriber();
  void setReceivedTopicFunction(std::function<void (const SoapMsg::SoapMsgerResult&)> tf);

private:

  void createParticipant(int argc, char* argv[]);
  void createSubscriber();
  void registerTopic();
  void createDataReader();

private:

  DDS::DomainParticipantFactory_var m_participantFactory;
  DDS::DomainParticipant_var m_participant;
  DDS::Subscriber_var m_subscriber;
  SoapMsg::SoapMsgerResultTypeSupport_var m_topicSupport;
  DDS::Topic_var m_messageTopic;
  DDS::DataReaderListener_var m_listener;
  DDS::TopicQos m_defaultQoS;
};

#endif // !SOAP_SUBSCRIBER_H_

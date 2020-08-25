#ifndef SOAP_PUBLISHER_H_
#define SOAP_PUBLISHER_H_

#include "idl/SoapMsgTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <string>

class SOAP_Publisher {
public:

  SOAP_Publisher(int argc, char* argv[]);
  ~SOAP_Publisher();

  void sendMessage(SoapMsg::SoapMsgerRequest& topic);

private:

  void createParticipant(int argc, char* argv[]);
  void createPublisher();
  void registerTopic();
  void createDataWriter();

private:
// 客户度，发送的是 Request消息
  DDS::DomainParticipantFactory_var m_participantFactory;
  DDS::DomainParticipant_var m_participant;
  DDS::Publisher_var m_publisher;
  SoapMsg::SoapMsgerRequestTypeSupport_var m_topicSupport;  //  发送类型 Request
  DDS::Topic_var m_messageTopic;
  SoapMsg::SoapMsgerRequestDataWriter_var m_dataWriter;  // Request 写入者
  DDS::InstanceHandle_t m_topicHandler;
  int m_counter = 1;
};

#endif // !SOAP_PUBLISHER_H_

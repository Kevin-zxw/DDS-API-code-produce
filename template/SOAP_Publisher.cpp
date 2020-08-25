#include "SOAP_Publisher.h"
#include <dds/DCPS/Marked_Default_Qos.h>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// USING SECTION                                                             //
///////////////////////////////////////////////////////////////////////////////

using std::cerr;
using std::cout;
using std::endl;
using std::string;

///////////////////////////////////////////////////////////////////////////////
// CONSTANTS SECTION                                                         //
///////////////////////////////////////////////////////////////////////////////

DDS::DomainId_t DomainID{ 1066 };


///////////////////////////////////////////////////////////////////////////////
// PUBLIC SECTION                                                            //
///////////////////////////////////////////////////////////////////////////////

SOAP_Publisher::SOAP_Publisher(int argc, char* argv[]) {
  createParticipant(argc, argv);
  createPublisher();
  registerTopic();
  createDataWriter();
}

SOAP_Publisher::~SOAP_Publisher() {
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

void SOAP_Publisher::sendMessage(SoapMsg::SoapMsgerRequest& topic) {
  int success = m_dataWriter->write(topic, DDS::HANDLE_NIL);
  if (success != DDS::RETCODE_OK) {
    ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: SOAP_Publisher::sendMessage write returned %d.\n"), success));
  }
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE SECTION                                                           //
///////////////////////////////////////////////////////////////////////////////

void SOAP_Publisher::createParticipant(int argc, char* argv[]) {
// 1. Initialize DomainParticipantFactory
  auto m_participantFactory = TheParticipantFactoryWithArgs(argc, argv);
// 2. Create DomainParticipant
  m_participant = m_participantFactory->create_participant(
      DomainID,
      PARTICIPANT_QOS_DEFAULT,
      DDS::DomainParticipantListener::_nil(),
      ::OpenDDS::DCPS::DEFAULT_STATUS_MASK);

  if (CORBA::is_nil (m_participant.in ())) {
      cerr << "SOAP_Publisher: Failed to create participant..." << endl;
  } else {
    cout << "SOAP_Publisher: participant created successfully" << endl;
  }
}
//????????????????PublisherListener需要改吗?????????????????????????
void SOAP_Publisher::createPublisher() {
// Create SOAP_Publisher
  m_publisher = m_participant->create_publisher(
    PUBLISHER_QOS_DEFAULT,
    DDS::PublisherListener::_nil(),
    ::OpenDDS::DCPS::DEFAULT_STATUS_MASK
  );
  if (CORBA::is_nil(m_publisher.in())) {
    cerr << "create_publisher failed." << endl;
  } else {
    cout << "SOAP_Publisher: SOAP_Publisher created correctly" << endl;
  }
}

void SOAP_Publisher::registerTopic() { // 客户端发送 Request消息
  // 3. Register TypeSupport (Messenger::Message)
  m_topicSupport = new SoapMsg::SoapMsgerRequestTypeSupportImpl();
  if (DDS::RETCODE_OK != m_topicSupport->register_type(
    m_participant.in(),
    "")
  ) {
    cerr << "register_type for " << "MessageType" << " failed." << endl;
  }
  // 4. Create Topic (Movie Discussion List)
  DDS::TopicQos defaultQoS;
  CORBA::String_var type_name = m_topicSupport->get_type_name();  
  m_participant->get_default_topic_qos(defaultQoS);
  m_messageTopic = m_participant->create_topic (
    "Request",
    type_name,
    defaultQoS,
    DDS::TopicListener::_nil(),
    ::OpenDDS::DCPS::DEFAULT_STATUS_MASK);
  if (CORBA::is_nil (m_messageTopic.in ())) {
    cerr << "create_topic for " << "MessageTopic" << " failed." << endl;
  }
}

void SOAP_Publisher::createDataWriter() {
  DDS::DataWriterQos defaultQoS;
  m_publisher->get_default_datawriter_qos (defaultQoS);
  DDS::DataWriter_var dataWriter =
  m_publisher->create_datawriter(
    m_messageTopic.in (),
    defaultQoS,
    DDS::DataWriterListener::_nil(),
    ::OpenDDS::DCPS::DEFAULT_STATUS_MASK
  );
  if (CORBA::is_nil (dataWriter.in ())) {
    cerr << "create_datawriter for " << "MessageTopic" << " failed." << endl;
  }

  m_dataWriter = SoapMsg::SoapMsgerRequestDataWriter::_narrow(dataWriter.in());
  if (CORBA::is_nil (m_dataWriter.in ())) {
    cerr << "MessageDataWriter could not be narrowed"<< endl;
  }
  // SoapMsg::Message spy;
  // spy.counter = 0;
  // spy.m = CORBA::string_dup("SPY");
  // m_topicHandler = m_dataWriter->register_instance(spy);
}

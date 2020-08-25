#ifndef MESSAGEDATAREADERLISTENERIMPL_H_
#define MESSAGEDATAREADERLISTENERIMPL_H_

#include "idl/SoapMsgC.h"
#include <dds/DdsDcpsSubscriptionC.h>
#include <dds/DCPS/LocalObject.h>
#include <functional>

class MessageDataReaderListenerImpl
  : public virtual OpenDDS::DCPS::LocalObject<DDS::DataReaderListener>
{
public:
// 客户端 获取Result消息
  MessageDataReaderListenerImpl() = default;
  virtual ~MessageDataReaderListenerImpl() = default;
  void setReceivedTopicFunction(std::function<void (const SoapMsg::SoapMsgerResult&)> tf);
  virtual void on_requested_deadline_missed(DDS::DataReader_ptr reader, const DDS::RequestedDeadlineMissedStatus& status);
  virtual void on_requested_incompatible_qos(DDS::DataReader_ptr reader, const DDS::RequestedIncompatibleQosStatus& status);
  virtual void on_liveliness_changed(DDS::DataReader_ptr reader, const DDS::LivelinessChangedStatus& status);
  virtual void on_subscription_matched (DDS::DataReader_ptr reader, const DDS::SubscriptionMatchedStatus& status);
  virtual void on_sample_rejected(DDS::DataReader_ptr reader, const DDS::SampleRejectedStatus& status);
  virtual void on_data_available(DDS::DataReader_ptr reader);
  virtual void on_sample_lost(DDS::DataReader_ptr reader, const DDS::SampleLostStatus& status);

private:

  std::function<void (const SoapMsg::SoapMsgerResult&)> m_function;
};

#endif // !MESSAGEDATAREADERLISTENERIMPL_H_

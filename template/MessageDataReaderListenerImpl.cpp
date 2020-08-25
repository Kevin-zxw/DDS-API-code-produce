#include "MessageDataReaderListenerImpl.h"
#include "idl/SoapMsgTypeSupportS.h"
#include "idl/SoapMsgTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <ace/streams.h>

///////////////////////////////////////////////////////////////////////////////
// PUBLIC SECTION                                                            //
///////////////////////////////////////////////////////////////////////////////

void MessageDataReaderListenerImpl::setReceivedTopicFunction(std::function<void (const SoapMsg::SoapMsgerResult&)> tf) {
  m_function = tf;
}

void MessageDataReaderListenerImpl::on_data_available(DDS::DataReader_ptr reader) {
  try {
    SoapMsg::SoapMsgerResultDataReader_var messageDataReader = SoapMsg::SoapMsgerResultDataReader::_narrow(reader);
    if (CORBA::is_nil(messageDataReader.in ())) {
      cerr << "MessageDataReaderListenerImpl::on_data_available: _narrow failed." << endl;
    }
    int count = 0;
    while(true) {
      SoapMsg::SoapMsgerResult SoapMsger;
      DDS::SampleInfo si;
      DDS::ReturnCode_t status = messageDataReader->take_next_sample(SoapMsger, si);
      if (status == DDS::RETCODE_OK && si.valid_data) {
        m_function(SoapMsger);
      } else if (status == DDS::RETCODE_NO_DATA) {
        break;
      } else {
        cerr << "ERROR: read SoapMsger: Error: " <<  status << endl;
      }
    }
  } catch (CORBA::Exception& e) {
    cerr << "Exception caught in read:" << endl << e << endl;
  }
}

void MessageDataReaderListenerImpl::on_requested_deadline_missed(DDS::DataReader_ptr,const DDS::RequestedDeadlineMissedStatus&) {
  cerr << "MessageDataReaderListenerImpl::on_requested_deadline_missed" << endl;
}

void MessageDataReaderListenerImpl::on_requested_incompatible_qos(DDS::DataReader_ptr, const DDS::RequestedIncompatibleQosStatus&) {
  cerr << "MessageDataReaderListenerImpl::on_requested_incompatible_qos" << endl;
}

void MessageDataReaderListenerImpl::on_liveliness_changed(DDS::DataReader_ptr, const DDS::LivelinessChangedStatus&) {
  cerr << "MessageDataReaderListenerImpl::on_liveliness_changed" << endl;
}

void MessageDataReaderListenerImpl::on_subscription_matched(DDS::DataReader_ptr, const DDS::SubscriptionMatchedStatus&) {
  cerr << "MessageDataReaderListenerImpl::on_subscription_matched" << endl;
}

void MessageDataReaderListenerImpl::on_sample_rejected(DDS::DataReader_ptr, const DDS::SampleRejectedStatus&) {
  cerr << "MessageDataReaderListenerImpl::on_sample_rejected" << endl;
}

void MessageDataReaderListenerImpl::on_sample_lost(DDS::DataReader_ptr, const DDS::SampleLostStatus&) {
  cerr << "MessageDataReaderListenerImpl::on_sample_lost" << endl;
}

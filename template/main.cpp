#include "SOAP_Subscriber.h"
#include "SOAP_Publisher.h"
#include "idl/SoapMsgC.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <time.h>
#include <string>


// Client部分
/****************************************************************
*                           程序逻辑
*   1. 分别开启订阅和发布DDDS
*   2. 订阅线程，接收到服务端发送回来的
*   3. 主线程中，休眠状态等待唤醒。唤醒后，开始解析客户端发送过来的主题
*   3.1 
*
***************************************************************/
int respond_count = 0;
SOAP_Publisher* publisher_ptr = NULL;
SOAP_Subscriber* subscriber_ptr = NULL;
int client_id = 0;


// 当前时间
static unsigned long get_tick()
{
	struct timespec tv;
	clock_gettime(CLOCK_MONOTONIC, &tv);
	unsigned long time = tv.tv_sec * 1000000 + tv.tv_nsec / (1000);
	return time; // 返回微秒值
}
long all_cost_time = 0;
int count = 0;
long cost_time = 0;
// 这里回调是Result消息
void printTopic(const SoapMsg::SoapMsgerResult message) {
  // 返回  服务端返回的Result消息
  unsigned long request_time  =  message.request_time;
  unsigned long anser_time = message.result_time;
  unsigned long current_time = get_tick() ;
  cost_time = current_time - request_time;
  all_cost_time += cost_time;
  count++;
  printf("===============================\n");
  printf("SesionId [%d] Comes back [%s]\n",message.session_id, message.soap_content);
  printf("Client Post Time [%u]\n",request_time);
  printf("Server Post Time [%u] [Not local time]\n",anser_time);
  printf("Current    Time [%u]\n",current_time);
  printf("Client to Server Trans Time [%u]\n",anser_time - request_time);
  printf("Server to Client Trans Time [%u]\n", current_time - anser_time);
  printf("[%dth]Post and Get Cost  Time[%u]\n", cost_time);
  printf("Post and Get Cost Average Time[%u]\n",all_cost_time/count);
  printf("===============================\n\n");
}
int request_count = 0;
char out_string[1025];
std::string send_content;

int main(int argv, char* argc[]) {

  using namespace std::chrono_literals;
  using namespace std;
  for(int i=0;i<1024;i++)
  {
	out_string[i] = 'z';
  }
  out_string[1024] = '\0';
  send_content = out_string;
  printf("Sent length %d\n",send_content.length());
// A: 初始化订阅模块，订阅主题为SoapMsgerRequest的消息
  std::function<void (const SoapMsg::SoapMsgerResult&)> topicFunction = printTopic;
  const auto sleepTime { 1s };
  cout << "=== SUBSCRIBER ===" << endl;

  subscriber_ptr = new SOAP_Subscriber(argv, argc);
  subscriber_ptr->setReceivedTopicFunction(topicFunction);
  cout << "SOAP_Subscriber: waiting for events" << endl;
// B: 发布模块
  publisher_ptr = new SOAP_Publisher(argv,argc);
  SoapMsg::SoapMsgerRequest req_topic;
  req_topic.client_id = client_id;

  while (true) {
    this_thread::sleep_for(sleepTime);
	req_topic.count = request_count;
  	req_topic.request_time = get_tick();
  	req_topic.session_id = request_count++;
	req_topic.soap_content = "Hello World";
	publisher_ptr->sendMessage(req_topic); // 1s 发送一次
	printf("Cline have post msg [%u]\n",req_topic.request_time);
  }

  return 0;
}

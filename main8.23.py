# -*- coding: utf-8 -*-
# @Time    : 2020/8/21 15:25
# @Author  : Kevin-zxw

import json
import easygui as g  # 导入EasyGui模块


def findall_struct(strings, x, words, list):  # 遍历获取某个元素索引
    num = 0
    while (x != -1):
        x = strings.find(words, num)
        if (not x == -1):
            num = x + 1
            list.append(x)
        else:
            break


# 定义区
Messagetype_beginlist = []  # 查找时开始的索引
Messagetype_endlist = []  # 查找时结尾的索引（两个索引之间就是数据类型名字）
struct_name = []  # 数据类型结构名
x = 0
k = 0
j = 0
struct_num = 0  # 数据类型结构个数

# with open("F:\机器人中间件\DDS\测试python\config.json", 'r', encoding='utf-8') as f:
#     temp = json.loads(f.read())
#     print(temp)
#     f.close()


# fileopenbox()函数的返回值是你选择的那个文件的具体路径
file_position = g.fileopenbox('open file', 'C:/User/Administrator/Desktop/__pycache__')
# msgbox()是测试用的，可以不用写
# g.msgbox(str1)

with open(file_position, 'r', encoding='utf-8') as a:
    temp = a.read()
    temp1 = str(temp)
    # print(temp1)

    index1 = temp1.find(' ')
    index2 = temp1.find('{')  # topic前后的位置索引
    Topic_Name = temp1[index1:index2]  # 得到topic名

    findall_struct(temp1, x, "struct", Messagetype_beginlist)
    Messagetype_beginlist = [i + 7 for i in Messagetype_beginlist]

    findall_struct(temp1, x, "{", Messagetype_endlist)
    # Messagetype_endlist = [i - 1 for i in Messagetype_endlist]

    while k <= (len(Messagetype_beginlist) - 1):
        print(k)
        struct_name.append(temp1[Messagetype_beginlist[k]:Messagetype_endlist[j + 1]])  # 得到数据类型结构名
        # print(struct_name[j])
        # print(struct_num)
        struct_num += 1
        j += 1
        k += 1
    parameters = {"main_file": "main_temp.txt",
                  "main_name": "main.cpp",
                  "ImplC_file": "MessageDataReaderListenerImplC_temp.txt",
                  "ImplC_name": "MessageDataReaderListenerImpl.cpp",
                  "ImplH_file": "MessageDataReaderListenerImplH_temp.txt",
                  "ImplH_name": "MessageDataReaderListenerImpl.h",
                  "PublisherC_file": "PublisherC_temp.txt",
                  "PublisherC_name": str(Topic_Name) + "_Publisher.cpp",
                  "PublisherH_file": "PublisherH_temp.txt",
                  "PublisherH_name": str(Topic_Name) + "_Publisher.h",
                  "SubscriberC_file": "SubscriberC_temp.txt",
                  "SubscriberC_name": str(Topic_Name) + "_Subscriber.cpp",
                  "SubscriberH_file": "SubscriberH_temp.txt",
                  "SubscriberH_name": str(Topic_Name) + "_Subscriber.h",
                  "Publisher_def": str(Topic_Name) + "_PUBLISHER",
                  "Subscriber_def": str(Topic_Name) + "_SUBSCRIBER",

                  "Topic_name": str(Topic_Name),
                  "Request_name": str(struct_name[0]),
                  "Result_name": str(struct_name[1]),

                  "XXXPublisher": str(Topic_Name) + "_Publisher",
                  "XXXSubscriber": str(Topic_Name) + "_Subscriber"
                  #"Struct_name": str(struct_name[0]),

                  #"Struct_name1": str(struct_name[1])
                  }
    json_str = json.dumps(parameters, indent=4, ensure_ascii=False)

    with open('config.json', 'w') as f:  # 创建一个params.json文件
        f.write(json_str)  # 将json_str写到文件中
    a.close()

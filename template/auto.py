# -*- coding: UTF-8 -*-
#!/usr/bin/env python

#-------------------------------------------------------------------------------
# Name:        UI自动生成工具
# Purpose:
#
# Author:      Yang
#
# Created:     13/08/2015
# Copyright:   (c) Yang 2015
# Licence:     <your licence>
#-------------------------------------------------------------------------------
import json
import codecs

def replace(tempfile, targetfile, conjson):
    # read template file
    s = ""
    template = conjson.get(tempfile, "main_temp.txt")
    with codecs.open(template, "rb", "UTF-8") as f:
        s = f.read()
    if not s:
        return
    s = s % conjson

    # save to file
    fn = conjson[targetfile]
    with codecs.open(fn, "wb", "UTF-8") as f:
        f.write(s)
        f.flush()
    


def main():
    # read config
    config = {}
    with codecs.open("config.json","rb","UTF-8") as f:
        config = json.loads(f.read())
    if not config:
        return
		
    replace("main_file", "main_name", config)
    replace("ImplC_file", "ImplC_name", config)
    replace("ImplH_file", "ImplH_name", config)
    replace("PublisherC_file", "PublisherC_name", config)
    replace("PublisherH_file", "PublisherH_name", config)
    replace("SubscriberC_file", "SubscriberC_name", config)
    replace("SubscriberH_file", "SubscriberH_name", config)


if __name__ == '__main__':
    try:
        main()
    except Exception as ex:
        print(ex)

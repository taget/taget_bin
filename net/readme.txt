file.txt 中记录启动方式 ，其中最后一项为xml模板

xml模板格式如下
line 1：这行为登录信息，eg ：`SF`1.00002803e8LGINID=2`EF`
line 2：这行为xml模板，使用 [5] 表示将[5]用5为使用时间生成的数字字符串代替
eg `SF`1.00085203e8INFO<xmlmessage><ticket><tichead><ssid>[4]</ssid><subsid>[10]</subsid><pid>1</pid><usertype>0</usertype><tictype>0</tictype><prior>0</prior><area>029</area><dn>81795003</dn><officecode>0</officecode><otime>2010-07-22 12:16:23</otime></tichead><ticbody><basesvcid>2</basesvcid><subsvcs><subsvcid>200092</subsvcid><oper>2</oper><svcpara><parament>200016</parament><value>96563</value></svcpara><svcpara><parament>100002</parament><value>81795003</value></svcpara><svcpara><parament>200015</parament><value>0</value></svcpara></subsvcs><subsvcs><subsvcid>200094</subsvcid><oper>1</oper><svcpara><parament>200016</parament><value>88888881</value></svcpara><svcpara><parament>100002</parament><value>81795003</value></svcpara><svcpara><parament>200015</parament><value>1</value></svcpara></subsvcs></ticbody></ticket></xmlmessage>`EF`
#!/bin/awk -f
BEGIN{
    ARGC+=1 ; 
    ARGV [ARGC -1] = "file.txt";
	print "INAS�����ű���ʼִ��"
	FS= "[ \t:]+" #�ı��ֶ��и�ķ�ʽ
	"date" | getline  # Shell ִ�� "date". getline ȡ�ý����$0��¼
	print " Today is " ,$2, $3 
	FS= ";" #�ı��ֶ��и�ķ�ʽ
	print "-------------------------"
	
	while ("pa" | getline info)
	{
		array[i++] = info
	}
}

{
#	painfo = "pa | grep " $1
	proname = $1 
	argv = $2
	print proname
	print argv
	cmd = "./"proname " " argv
	print cmd
	system(cmd)
	system("sleep 1")	
}

END{
	print "-------------------------"
	print "INAS�ű�ά���������"
}

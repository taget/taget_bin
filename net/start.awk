#!/bin/awk -f
BEGIN{
    ARGC+=1 ; 
    ARGV [ARGC -1] = "file.txt";
	print "INAS启动脚本开始执行"
	FS= "[ \t:]+" #改变字段切割的方式
	"date" | getline  # Shell 执行 "date". getline 取得结果以$0记录
	print " Today is " ,$2, $3 
	FS= ";" #改变字段切割的方式
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
	print "INAS脚本维护服务完毕"
}

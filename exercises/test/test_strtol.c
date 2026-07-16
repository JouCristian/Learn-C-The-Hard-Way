#include<stdio.h>
#include<stdlib.h>	//strtol住在这个头文件里

int main(int argc, char *argv[]){	
	if(argc < 2){
		printf("用法错误，请提供一个数字参数！\n");
		printf("正确示例： ./test_strtol 1024\n");
		exit(1);
	}

	char *end;	//用来记录在哪里被非数字字符卡住了
	long number = strtol(argv[1],&end,10);	//对传入的第一个字符串argc[1]进行10进制转换
						//并将卡住的位置地址传给end指向的地址
	
	if(end == argv[1] || *end != '\0'){
		printf("发现 %s 不是一个单纯的数字！\n",end);
	}
	else{
		printf("成功转换成数字： %ld。\n",number);
	}

	return 0;
}

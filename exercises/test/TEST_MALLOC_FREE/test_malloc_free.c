#include<stdio.h>
#include<stdlib.h>	//malloc 和 free 住在这个头文件里

int main(){
	
	int *arr = malloc(5 * sizeof(int));

	if(arr == NULL){
		printf("ERROR: 没租到场地，内存不够用了。\n");
		return 1;
	}
	
	
	arr[0] = 100;
	arr[1] = 200;
	printf("第一个房间的数据是： %d\n",arr[0]);

	free(arr);

	arr = NULL;

	return 0;
}

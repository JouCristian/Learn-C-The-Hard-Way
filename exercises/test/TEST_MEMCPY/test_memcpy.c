#include<stdio.h>
#include<stdlib.h>
#include<string.h>	// memcpy 住在这里

int main(){
	int src_array[5] = {10,20,30,40,50};

	int *dest_array = calloc(5,sizeof(int));
	if(dest_array == NULL){
		fprintf(stderr, "内存空间不足了哦！\n");
		return 1;
	}

	memcpy(dest_array, src_array, 5 * sizeof(int));

	printf("搬运后的新数组第一个元素是: %d\n", dest_array[0]); // 输出 10
	
	free(dest_array);

	dest_array = NULL;
	return 0;
}


#include <stdio.h>
#include <string.h>

int main() {
    // 故意造一个极小的容器，只能装 4 个字母 + 1 个 \0
    char tiny_buffer[5]; 
    // 造一个正常大小的容器
    char normal_buffer[20];

    // ==========================================
    // 案发现场 1：fflush 的发车指令与 fgets 的截断
    // ==========================================
    printf("\n--- 现场 1：装满的容器与滞留的垃圾 ---\n");
    printf("请输入超过 4 个字母（比如 abcdefg），然后按回车：");
    
    // 踩下油门！强制把上面那句没有 \n 的提示语推送到屏幕上
    fflush(stdout); 

    // 容器只有 5 大小，所以 fgets 最多只会吸走前 4 个字母
    fgets(tiny_buffer, 5, stdin);
    printf("👉 tiny_buffer 拼命吸，只吸到了: [%s]\n", tiny_buffer);


    // ==========================================
    // 案发现场 2：被污染的底层输入管道
    // ==========================================
    printf("\n--- 现场 2：惨遭毒手的下一个函数 ---\n");
    printf("现在我们想让你输入正常的姓氏：\n");
    
    // 灾难发生！因为刚才的 efg 和回车符还卡在管道里
    // 这里的 fgets 会瞬间把它们吸进去，根本不给你敲键盘的机会！
    fgets(normal_buffer, 20, stdin);
    
    // 注意看打印出来的括号里面，不仅有字母，还带了一个隐形的换行符！
    printf("👉 normal_buffer 瞬间被垃圾填满，读到了: [%s]\n", normal_buffer);


    // ==========================================
    // 案发现场 3：终极冲马桶大法
    // ==========================================
    printf("\n--- 现场 3：打扫战场 ---\n");
    printf("请再次输入一串长字母（比如 12345678）：");
    fflush(stdout);
    
    fgets(tiny_buffer, 5, stdin);
    printf("👉 tiny_buffer 又吸了前 4 个字母: [%s]\n", tiny_buffer);

    // 重点来了！开始冲马桶！
    printf("⏳ 发现管道超载，开始启动马桶抽水程序...\n");
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        // 我们把抽出来的残渣打印在屏幕上，让你亲眼看看管道里刚才藏了什么
        printf("   >> 冲走管道残渣: %c\n", ch);
    }
    printf("✨ 管道已彻底清洗干净！\n");

    // 管道干净了，终于可以正常输入了
    printf("\n现在管道干净了，请输入你的真实名字：");
    fflush(stdout);
    fgets(normal_buffer, 20, stdin);
    
    // 顺手做个“抹除回车符”的经典操作
    char *newline = strchr(normal_buffer, '\n');
    if(newline != NULL) {
       *newline = '\0'; 
    }
    printf("👉 最终得到纯净的名字: [%s]\n\n", normal_buffer);

    return 0;
}

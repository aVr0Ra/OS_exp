#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LENGTH 100  // 定义最大输入长度
#define MAX_ARGS 100    // 定义参数数组的最大长度

int main() {
    char input[MAX_LENGTH]; // 用于存储输入的命令行
    char *args[MAX_ARGS]; // 存储分割后的命令和参数
    char *token; // 用于标记分割的字符串
    pid_t pid; // 用于存储进程ID
    int i = 0; // 循环变量

    while (1) { // 无限循环直到用户输入 "quit\n"
        printf("21071125YSM's Shell> ");

        // 读取一行输入
        if (fgets(input, MAX_LENGTH, stdin) == NULL) {
            break; // 如果读取失败，则跳出循环
        }

        // 如果输入是 "quit\n"，退出程序
        if (strcmp(input, "quit\n") == 0) {
            break;
        }

        // 使用 strtok 分割输入的命令和参数
        token = strtok(input, " \t\n");
        i = 0;
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " \t\n");
        }
        args[i] = NULL; // 确保参数数组的最后一个元素为NULL

        // 创建子进程
        pid = fork();
        if (pid < 0) {
            perror("Fork failed"); // 如果 fork 失败，打印错误并退出
            exit(1);
        }
        else if (pid == 0) { // 在子进程中
            // 执行输入的命令
            if (execvp(args[0], args) < 0) {
                perror("execvp failed"); // 如果 execvp 失败，打印错误并退出
                exit(1);
            }
        } 
        else { 
            wait(NULL); // 父进程等待子进程结束
            printf("Child exited successfully\n"); // 子进程结束后打印消息
        }
    }

    return 0;
}

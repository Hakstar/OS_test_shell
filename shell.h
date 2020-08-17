//
// Created by Alex on 2020/8/13.
//

#ifndef TEST_SHELL_SHELL_H
#define TEST_SHELL_SHELL_H

#include <list>
#include <queue>
#include <string>
#include "PCB.h"
#include "RCB.h"

class shell {
public:
    shell();// shell构造函数
    void init_Process();// 初始化进程
    PCB *get_process(int pid);// 根据pid获取一个PCB
    RCB *get_RCB(int rid);// 根据rid获取一个RCB
    void create_process(int pid, int priority);// 创建一个进程
    void destroy_process(int pid);// 销毁一个进程
    void request_resource(int rid, int n);// 请求资源
    void release_resource(int rid, int n);// 释放资源
    void schedule();// 进程调度
    void time_out();// 时间片轮转
    void list_ready_process();// 查看就绪队列
    void list_block_process();// 查看阻塞队列
    void list_available_res();// 查看可用资源
    void print_PCB(int pid);// 打印PCB信息
    ~shell();

private:
    int running;// 正在运行的进程的pid
    std::list<RCB *> all_resources;// 所有资源RCB链表
    std::list<PCB *> allPCBs;// 所有PCB链表
    std::list<int> p1_ready_process;// 优先级为1的就绪队列
    std::list<int> p1_block_process;// 优先级为1的阻塞队列
    std::list<int> p2_ready_process;// 优先级为2的就绪队列
    std::list<int> p2_block_process;// 优先级为2的阻塞队列

    void kill_tree(int pid);// 销毁进程
    void preemption(int p, int running_p);// 抢占进程
    bool search_posterity(int pid, int search_id);// 查找祖先
    void delete_child(int pid, int delete_pid);// 从children_list中删掉被销毁的进程（递归）
};


#endif //TEST_SHELL_SHELL_H

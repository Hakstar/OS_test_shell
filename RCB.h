//
// Created by Alex on 2020/8/13.
//

#ifndef TEST_SHELL_RCB_H
#define TEST_SHELL_RCB_H

#include <list>
#include "PCB.h"

class RCB {
public:
    RCB(int rid, int num);// 初始化RCB需要rid
    int get_available();// 获取剩余资源数量
    int get_rid();// 获取资源rid
    int get_total();// 获取资源总数
    void set_available(int n);// 设置可获得资源数量
    std::list<int> get_wait_list();// 获取等待该资源的进程pid链表
    void remove_wait_pid(int pid);// 删除等待进程
    void add_wait_pid(int pid);// 添加等待进程
    ~RCB();

private:
    int rid;// 资源rid
    int total;// 资源总数
    int available;// 当前可用资源
    std::list<int> wait_list;// 等待该资源的进程链表
};


#endif //TEST_SHELL_RCB_H

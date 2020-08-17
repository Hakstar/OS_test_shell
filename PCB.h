//
// Created by Alex on 2020/8/13.
//

#ifndef TEST_SHELL_PCB_H
#define TEST_SHELL_PCB_H

#include <list>
#include <map>

class PCB {
public:
    PCB(int pid, int fatherProcess, int priority);// 创建PCB需要 pid 父进程 优先级
    // 修改获取PCB数据接口
    int get_pid();// 获取进程pid
    int get_priority(); // 获取进程优先级
    std::map<int, int> get_resources(); // 获取进程已获得的资源
    int get_statue();// 获取进程当前状态
    int get_father_pid();// 获取父进程pid
    std::list<int> get_childrenPCBs();// 获取子进程pid
    int get_waitRid_num();// 获取等待资源数量
    int get_waitRid();// 获取等待资源的rid

    void set_priority(int new_priority); //　修改进程优先级
    void set_statue(int new_statue);// 修改进程状态
    void add_childProcess(int c_pid);// 增加子进程
    void set_resources(int rid, int num);// 修改拥有资源
    void delete_childProcess(int c_pid);// 删除子进程
    void set_waitRid(int rid);// 修改等待资源rid
    void set_waitRid_num(int n);// 修改等待资源数目
    ~PCB();

private:
    int pid;// 进程id
    int priority;// 进程优先级
    std::map<int, int> resources{{1, 0},
                                 {2, 0},
                                 {3, 0},
                                 {4, 0}};// 拥有资源
    int father_pid;// 父进程
    std::list<int> childrenPCBs;// 子进程链表
    int statue;// 进程状态(0 :就绪  -1 :阻塞 1：运行)

    int waitRid = 0;// 等待资源的rid
    int waitRid_num = 0;// 等待资源数目

};


#endif //TEST_SHELL_PCB_H

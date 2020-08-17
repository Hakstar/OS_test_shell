//
// Created by Alex on 2020/8/13.
//

#include <iostream>
#include <map>
#include "shell.h"

shell::shell() {
    // 初始设置进程为-1，表示没有正在运行的进程
    this->running = -1;

    // 创建RCB
    RCB *RCB_R1 = new RCB(1, 1);
    RCB *RCB_R2 = new RCB(2, 2);
    RCB *RCB_R3 = new RCB(3, 3);
    RCB *RCB_R4 = new RCB(4, 4);
    all_resources.push_back(RCB_R1);
    all_resources.push_back(RCB_R2);
    all_resources.push_back(RCB_R3);
    all_resources.push_back(RCB_R4);
}


void shell::init_Process() {
    // 创建init进程
    PCB *init_process = new PCB(0, -1, 0);
    init_process->set_statue(1);
    this->allPCBs.push_back(init_process);
    this->running = init_process->get_pid();
    std::cout << "Process init is running" << std::endl;
}

PCB *shell::get_process(int pid) {
    for (auto PCB : this->allPCBs) {
        if (PCB->get_pid() == pid) return PCB;
    }
    return nullptr;
}


void shell::create_process(int pid, int priority) {
    // 优先级输入检测
    if (priority != 1 && priority != 2) {
        std::cout << "error : bad priority" << std::endl;
        return;
    }
    // pid输入检测
    if (this->get_process(pid) != nullptr || pid <= 0) {
        std::cout << "error : pid wrong" << std::endl;
        return;
    }

    // 添加子进程
    int running_process = this->running;
    PCB *fatherPCB = this->get_process(running_process);
    PCB *new_process = new PCB(pid, running_process, priority);
    fatherPCB->add_childProcess(pid);
    allPCBs.push_back(new_process);

    // 放入对应优先级的就绪队列
    if (priority == 1) this->p1_ready_process.push_back(pid);
    else this->p2_ready_process.push_back(pid);

    this->schedule();

}

void shell::kill_tree(int pid) {
    // 获取PCB
    PCB *process = this->get_process(pid);
    if (process == nullptr) return;

    // 修改当前正在运行进程
    if (pid == this->running) this->running = -1;

    // 递归删除其子进程
    for (auto child :process->get_childrenPCBs()) {
        kill_tree(child);
    }

    // 将其从对应队列中移除
    if (process->get_statue() == 0) {
        if (process->get_priority() == 1) {
            this->p1_ready_process.remove(pid);
        } else {
            this->p2_ready_process.remove(pid);
        }
    } else {
        if (process->get_priority() == 1) {
            this->p1_block_process.remove(pid);
        } else {
            this->p2_block_process.remove(pid);
        }
    }


    // 清理等待队列中的进程
    if (process->get_statue() == -1) {
        for (auto resource : process->get_resources()) {
            RCB *r = get_RCB(resource.first);
            r->remove_wait_pid(pid);
        }
    }


    // 释放资源
    for (auto resource : process->get_resources()) {
        RCB *r = get_RCB(resource.first);
        r->set_available(r->get_available() + resource.second);// 修改可用资源
        if (resource.second > 0) { // 打印释放信息
            std::cout << "release " << resource.second << "  R" << resource.first << std::endl;
        }
        if (!r->get_wait_list().empty()) {// 查看等待该资源的进程能否唤醒
            PCB *r_p = this->get_process(r->get_wait_list().front());// 获取队列中的第一个进程
            if (r_p->get_waitRid_num() <= r->get_available()) { // 唤醒进程
                r->set_available(r->get_available() - r_p->get_waitRid_num());
                r->remove_wait_pid(r_p->get_pid());
                r_p->set_statue(0);
                r_p->set_resources(r->get_rid(),
                                   r_p->get_resources().find(r->get_rid())->second + r_p->get_waitRid_num());
                r_p->set_waitRid(0);
                r_p->set_waitRid_num(0);
                if (r_p->get_priority() == 1) {// 修改队列
                    this->p1_block_process.remove(r_p->get_pid());
                    this->p1_ready_process.push_back(r_p->get_pid());
                } else {
                    this->p2_block_process.remove(r_p->get_pid());
                    this->p2_ready_process.push_back(r_p->get_pid());
                }
                std::cout << "process " << r_p->get_pid() << " is awake" << std::endl;
            }
        }
    }


    // 从祖先中的子进程列表中删除
    delete_child(pid, pid);

    // 在PCB列表中删除该进程
    allPCBs.remove(process);
    delete process;
    process = nullptr;

}

void shell::destroy_process(int pid) {
    // 检测不能销毁祖先进程
    if (!search_posterity(this->running, pid)) {
        std::cout << "error : cant destroy the process " << std::endl;
        return;
    }
    kill_tree(pid);
    this->schedule();
}

RCB *shell::get_RCB(int rid) {
    for (auto RCB : this->all_resources) {
        if (rid == RCB->get_rid()) return RCB;
    }
    return nullptr;
}


void shell::request_resource(int rid, int n) {
    // init进程或没有运行进程是无法申请资源
    if (this->running <= 0) {
        std::cout << "error : init process cant request resource" << std::endl;
        return;
    }

    RCB *r = this->get_RCB(rid);
    PCB *p = this->get_process(this->running);
    // 资源总量小于申请资源量
    if (r->get_total() < n) {
        std::cout << "error : request resource is more than available resource" << std::endl;
        return;
    } else {
        if (r->get_available() >= n) {// 能够完成申请
            int a = p->get_resources().find(rid)->second;
            p->set_resources(rid, a + n);
            r->set_available(r->get_available() - n);
            std::cout << "process " << p->get_pid() << " requests " << n << " R" << rid << std::endl;
        } else {// 不能申请，被阻塞
            p->set_statue(-1);
            p->set_waitRid(rid);
            p->set_waitRid_num(n);
            if (p->get_priority() == 1) {
                this->p1_ready_process.remove(p->get_pid());
                this->p1_block_process.push_back(p->get_pid());
            } else {
                this->p2_ready_process.remove(p->get_pid());
                this->p2_block_process.push_back(p->get_pid());
            }
            r->add_wait_pid(p->get_pid());
            this->schedule();
        }
    }
}

void shell::release_resource(int rid, int n) {
    // init进程无法释放资源
    if (this->running <= 0) {
        std::cout << "error : init process cant release resource" << std::endl;
        return;
    }
    RCB *r = this->get_RCB(rid);
    PCB *p = this->get_process(this->running);
    if (n > p->get_resources().find(rid)->second) {// 释放资源错误
        std::cout << "error : release resource is more than got" << std::endl;
        return;
    } else { // 释放资源
        r->set_available(r->get_available() + n);
        p->set_resources(rid, p->get_resources().find(rid)->second - n);
        std::cout << "release resource " << n << "  R" << rid << std::endl;
        if (!r->get_wait_list().empty()) { // 查看能否唤醒进程
            PCB *r_p = this->get_process(r->get_wait_list().front());
            if (r_p->get_waitRid_num() <= r->get_available()) {
                r->set_available(r->get_available() - r_p->get_waitRid_num());
                r->remove_wait_pid(r_p->get_pid());
                r_p->set_statue(0);
                r_p->set_resources(rid, r_p->get_resources().find(rid)->second + r_p->get_waitRid_num());
                r_p->set_waitRid(0);
                r_p->set_waitRid_num(0);
                if (r_p->get_priority() == 1) {
                    this->p1_block_process.remove(r_p->get_pid());
                    this->p1_ready_process.push_back(r_p->get_pid());
                } else {
                    this->p2_block_process.remove(r_p->get_pid());
                    this->p2_ready_process.push_back(r_p->get_pid());
                }
                std::cout << "process " << r_p->get_pid() << " is awake" << std::endl;

                this->schedule();
            }
        }
    }
}

void shell::schedule() {

    // 查找当前优先级最高进程
    int new_p = -1;
    if (!p2_ready_process.empty()) {
        new_p = this->p2_ready_process.front();
    } else if (!p1_ready_process.empty()) {
        new_p = this->p1_ready_process.front();
    } else {
        new_p = 0;
    }

    // 如果正在运行的进程被销毁，直接抢占
    if (this->running == -1) {
        preemption(new_p, this->running);
    } else {
        PCB *running_process = get_process(this->running);
        PCB *highest_process = get_process(new_p);

        if (running_process->get_priority() < highest_process->get_priority() || running_process->get_statue() != 1) {
            preemption(new_p, this->running);
        } else {
            std::cout << "process " << this->running << " is running." << std::endl;
        }
    }
}

void shell::time_out() {
    PCB *p = get_process(this->running);
    if (p->get_priority() == 2) {
        p2_ready_process.remove(p->get_pid());
        p->set_statue(0);
        p2_ready_process.push_back(p->get_pid());
    } else if (p->get_priority() == 1) {
        p1_ready_process.remove(p->get_pid());
        p->set_statue(0);
        p1_ready_process.push_back(p->get_pid());
    }
    this->schedule();
}

void shell::preemption(int p_pid, int running_pid) {
    PCB *p = get_process(p_pid);
    p->set_statue(1);
    this->running = p_pid;
    std::cout << "process " << p_pid << " is running.";
    //todo 完善上下文信息
    if (this->get_process(running_pid)->get_statue() == -1) {
        std::cout << "process " << running_pid << " is blocked" << std::endl;
    } else {
        std::cout << "process " << running_pid << " is ready" << std::endl;
    }
}

void shell::list_ready_process() {
    std::cout << "priority 2 list : ";
    for (auto pid : p2_ready_process) {
        std::cout << pid << "-";
    }
    std::cout << std::endl << "priority 1 list : ";
    for (auto pid : p1_ready_process) {
        std::cout << pid << "-";
    }
    std::cout << std::endl << "priority 0 list : " << 0 << "-" << std::endl;
}

void shell::list_block_process() {
    for (int i = 1; i <= 4; i++) {
        RCB *r = get_RCB(i);
        std::cout << "R" << i << ": ";
        for (auto p : r->get_wait_list()) {
            std::cout << p << "-";
        }
        std::cout << std::endl;
    }
}

void shell::list_available_res() {
    for (int i = 1; i <= 4; ++i) {
        RCB *r = get_RCB(i);
        std::cout << "R" << i << ":" << r->get_available() << std::endl;
    }
}

void shell::print_PCB(int pid) {
    PCB *p = get_process(pid);
    std::cout << "pid : " << pid << std::endl;
    std::cout << "statue : " << p->get_statue() << std::endl;
    std::cout << "priority : " << p->get_priority() << std::endl;
    std::cout << "resources: " << std::endl;
    for (auto res : p->get_resources()) {
        std::cout << "R" << res.first << "  :" << res.second << std::endl;
    }
    std::cout << "father_pid : " << p->get_father_pid() << std::endl;
    std::cout << "children_pid : ";
    for (auto c_pid : p->get_childrenPCBs()) {
        std::cout << c_pid << "-";
    }
    std::cout << std::endl;
    if (p->get_statue() == -1) {
        std::cout << "wait for " << p->get_waitRid() << " : " << p->get_waitRid_num() << std::endl;
    }
}

//　用于检测是销毁的进程是否是当前运行进程的祖先　search_id:需要删除的id pid:当前运行的进程，并用于递归查找祖先
bool shell::search_posterity(int pid, int search_id) {
    PCB *p = this->get_process(pid);
    if (p->get_childrenPCBs().empty()) return false;
    bool flag = false;
    for (auto i : p->get_childrenPCBs()) {
        if (i == search_id) return true;
        flag = search_posterity(i, search_id);
    }
    return flag;
}

// 用于删除进程中子进程
void shell::delete_child(int pid, int delete_pid) {
    PCB *p = get_process(pid);
    if (p->get_father_pid() != -1) {
        PCB *f = get_process(p->get_father_pid());
        f->delete_childProcess(delete_pid);
        delete_child(p->get_father_pid(), delete_pid);
    }
}


shell::~shell() = default;

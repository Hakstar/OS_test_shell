//
// Created by Alex on 2020/8/13.
//

#include "PCB.h"

PCB::~PCB() = default;

PCB::PCB(int pid, int father_pid, int priority)
        : pid(pid),
          father_pid(father_pid),
          priority(priority),
          statue(0) {

}

int PCB::get_pid() {
    return this->pid;
}

int PCB::get_priority() {
    return this->priority;
}

std::map<int, int> PCB::get_resources() {
    return this->resources;
}

int PCB::get_statue() {
    return this->statue;
}


std::list<int> PCB::get_childrenPCBs() {
    return this->childrenPCBs;
}

void PCB::set_priority(int new_priority) {
    this->priority = new_priority;
}

void PCB::set_statue(int new_statue) {
    this->statue = new_statue;
}

void PCB::add_childProcess(int c_pid) {
    this->childrenPCBs.push_back(c_pid);
}

void PCB::set_resources(int rid, int num) {
    this->resources[rid] = num;
}

void PCB::delete_childProcess(int c_pid) {
    this->childrenPCBs.remove(c_pid);
}

int PCB::get_father_pid() {
    return this->father_pid;
}

void PCB::set_waitRid(int rid) {
    this->waitRid = rid;
}

void PCB::set_waitRid_num(int n) {
    this->waitRid_num = n;
}

int PCB::get_waitRid_num() {
    return this->waitRid_num;
}

int PCB::get_waitRid() {
    return this->waitRid;
}





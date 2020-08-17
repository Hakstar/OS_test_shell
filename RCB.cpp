//
// Created by Alex on 2020/8/13.
//

#include "RCB.h"
#include <list>

RCB::RCB(int rid, int num)
        : rid(rid),
          total(num),
          available(num) {
}

RCB::~RCB() = default;

int RCB::get_available() {
    return this->available;
}

int RCB::get_rid() {
    return this->rid;
}

std::list<int> RCB::get_wait_list() {
    return this->wait_list;
}

int RCB::get_total() {
    return this->total;
}

void RCB::set_available(int n) {
    this->available = n;
}

void RCB::remove_wait_pid(int pid) {
    this->wait_list.remove(pid);
}

void RCB::add_wait_pid(int pid) {
    this->wait_list.push_back(pid);
}

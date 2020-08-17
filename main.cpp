#include <iostream>
#include "shell.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>

constexpr size_t HASH_STRING_PIECE(const char *string_piece, size_t hashNum = 0) {
    return *string_piece ? HASH_STRING_PIECE(string_piece + 1, (hashNum * 131) + *string_piece) : hashNum;
}

size_t CALC_STRING_HASH(const std::string &str) {
    // 获取string对象得字符串值并传递给HAHS_STRING_PIECE计算，获取得返回值为该字符串HASH值
    return HASH_STRING_PIECE(str.c_str());
}

constexpr size_t operator "" _HASH(const char *string_pice, size_t) {
    return HASH_STRING_PIECE(string_pice);
}

// 根据指令选择执行函数
void run_command(std::vector<std::string> command, shell *shell);

int main() {
    std::string ins;
    shell os = shell();
    std::vector<std::string> res;
    std::cout << "please input the command" << std::endl;
    while (getline(std::cin, ins)) {
        if (ins == "\n")break;
        std::string tep;
        std::stringstream input(ins);
        while (input >> tep) res.push_back(tep);
        run_command(res, &os);
        res.clear();
    }
}

void run_command(std::vector<std::string> command, shell *shell) {
    if (command.empty()) {
        std::cout << "error : Invalid command, Please check input parameters" << std::endl;
    }
    switch (CALC_STRING_HASH(command[0])) {
        case "init"_HASH:
            if (command.size() != 1) {
                std::cout << "error : Invalid command, Please check input parameters" << std::endl;
            } else {
                shell->init_Process();
            }
            break;
        case "cr"_HASH:
            if (command.size() != 3) {
                std::cout << "error : Invalid command, Please check input parameters" << std::endl;
            } else {
                int pid = atoi(command[1].c_str());
                int priority = atoi(command[2].c_str());
                shell->create_process(pid, priority);
            }
            break;
        case "de"_HASH:
            if (command.size() != 2) {
                std::cout << "error : Invalid command, Please check input parameters" << std::endl;
            } else {
                int pid = atoi(command[1].c_str());
                shell->destroy_process(pid);
            }
            break;
        case "req"_HASH:
            if (command.size() != 3) {
                std::cout << "error : Invalid command, Please check input parameters" << std::endl;
            } else {
                int rid = atoi(command[1].c_str());
                int n = atoi(command[2].c_str());
                shell->request_resource(rid, n);
            }
            break;
        case "rel"_HASH:
            if (command.size() != 3) {
                std::cout << "error : Invalid command, Please check input parameters" << std::endl;
            } else {
                int rid = atoi(command[1].c_str());
                int n = atoi(command[2].c_str());
                shell->release_resource(rid, n);
            }
            break;
        case "to"_HASH:
            if (command.size() != 1) {
                std::cout << "error : Invalid command, Please check input parameters" << std::endl;
            } else {
                shell->time_out();
            }
            break;
        case "list"_HASH:
            if (command.size() != 2) {
                std::cout << "error : Invalid command, Please check input parameters" << std::endl;
            } else {
                if (command[1] == "ready") {
                    shell->list_ready_process();
                } else if (command[1] == "block") {
                    shell->list_block_process();
                } else if (command[1] == "res") {
                    shell->list_available_res();
                } else {
                    std::cout << "error : Invalid command, Please check input parameters" << std::endl;
                }
            }
            break;
        case "pr"_HASH:
            if (command.size() != 2) {
                std::cout << "error : Invalid command, Please check input parameters" << std::endl;
            } else {
                int pid = atoi(command[1].c_str());
                shell->print_PCB(pid);
            }
            break;
        default:
            std::cout << "error : Invalid command, Please check input parameters" << std::endl;
            break;
    }
}

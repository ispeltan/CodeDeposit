//
// Created by tanpe on 2023/4/9.
//

#pragma once
#include <string>
#include <chrono>

class Tools {
public:
    static std::string GetCurrentDirName(std::string file_path) {
        std::string str(file_path);
        auto find = str.find_last_of('/');
        if (find == std::string::npos) {
            return {};
        }
        str = str.substr(0, find + 1);
        return str;
    }

    static std::time_t GetTick() {
        auto cn_p = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now());
        auto cn_d = std::chrono::duration_cast<std::chrono::milliseconds>(cn_p.time_since_epoch());
        return cn_d.count();
    }
};
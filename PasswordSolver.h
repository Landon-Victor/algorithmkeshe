#pragma once
#include "Decode.h"
#include "json/json.h"
#include <vector>
#include <string>
#include <fstream>

class PasswordSolver {
public:
    // 线索推理，返回所有可能的三位密码（字符串形式）
    std::vector<std::string> solve() {
        auto passwords = findPasswords();
        return convertVectorIntToVectorString(passwords);
    }

    // 验证密码
    bool verifyPassword(const std::string& input) {
        return lock.verifyPassword(input, hash);
    }
    void load(const std::string& filename="lock.json")
    {
        clues.clear();
        Json::Value root;
        Json::CharReaderBuilder builder;
        std::ifstream ifs(filename);
        if (!ifs.is_open()) return;
        std::string errs;
        if (!Json::parseFromStream(builder, ifs, &root, &errs)) return;
        // 读取线索
        const Json::Value& arr = root["C"];
        for (const auto& clue : arr) {
            std::vector<int> c;
            for (const auto& v : clue) {
                c.push_back(v.asInt());
            }
            clues.push_back(c);
        }
        // 读取哈希值
        if (root.isMember("L") && root["L"].isString()) {
			hash = root["L"].asString();
        }
    }
private:
    PasswordLock lock;
    std::vector<std::vector<int>> clues;
    std::string hash;
private:
    // 密码推理算法
    std::vector<std::vector<int>> findPasswords() {
        bool primeClue = false;
        for (auto& clue : clues) {
            if (clue.size() == 2 && clue[0] == -1 && clue[1] == -1) {
                primeClue = true;
            }
        }
        std::vector<std::vector<int>> candidates(3);
        if (primeClue) {
            for (int i = 0; i < 3; i++) {
                candidates[i] = { 2, 3, 5, 7 };
            }
        } else {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j <= 9; j++) {
                    candidates[i].push_back(j);
                }
            }
        }
        std::vector<int> fixed(3, -1);
        for (auto& clue : clues) {
            if (clue.size() == 3) {
                if (clue[0] != -1 && clue[1] == -1 && clue[2] == -1) {
                    int b = clue[0];
                    if (fixed[0] != -1) {
                        if (fixed[0] != b) return {};
                    } else {
                        if (primeClue) {
                            if (b != 2 && b != 3 && b != 5 && b != 7) return {};
                        }
                        candidates[0] = { b };
                        fixed[0] = b;
                    }
                } else if (clue[1] != -1 && clue[0] == -1 && clue[2] == -1) {
                    int b = clue[1];
                    if (fixed[1] != -1) {
                        if (fixed[1] != b) return {};
                    } else {
                        if (primeClue) {
                            if (b != 2 && b != 3 && b != 5 && b != 7) return {};
                        }
                        candidates[1] = { b };
                        fixed[1] = b;
                    }
                } else if (clue[2] != -1 && clue[0] == -1 && clue[1] == -1) {
                    int b = clue[2];
                    if (fixed[2] != -1) {
                        if (fixed[2] != b) return {};
                    } else {
                        if (primeClue) {
                            if (b != 2 && b != 3 && b != 5 && b != 7) return {};
                        }
                        candidates[2] = { b };
                        fixed[2] = b;
                    }
                }
            }
        }
        for (auto& clue : clues) {
            if (clue.size() == 2 && !(clue[0] == -1 && clue[1] == -1)) {
                int pos_index = clue[0] - 1;
                int parity = clue[1];
                if (pos_index < 0 || pos_index > 2) continue;
                std::vector<int> newCandidates;
                for (int num : candidates[pos_index]) {
                    if (parity == 0 && num % 2 == 0) newCandidates.push_back(num);
                    else if (parity == 1 && num % 2 == 1) newCandidates.push_back(num);
                }
                candidates[pos_index] = newCandidates;
                if (newCandidates.empty()) return {};
            }
        }
        std::vector<std::vector<int>> result;
        for (int a : candidates[0]) {
            for (int b : candidates[1]) {
                for (int c : candidates[2]) {
                    if (primeClue) {
                        if (a == b || a == c || b == c) continue;
                    }
                    result.push_back({ a, b, c });
                }
            }
        }
        return result;
    }

    // int数组转字符串
    std::vector<std::string> convertVectorIntToVectorString(const std::vector<std::vector<int>>& input) {
        std::vector<std::string> result;
        for (const auto& subVec : input) {
            std::string s;
            for (int num : subVec) {
                s += std::to_string(num);
            }
            result.push_back(s);
        }
        return result;
    }
};

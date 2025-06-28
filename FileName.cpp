#include <iostream>
#include <vector>
#include "Decode.h"
using namespace std;

vector<vector<int>> findPasswords(vector<vector<int>>& clues) {
    // 检查素数线索
    bool primeClue = false;
    for (auto& clue : clues) {
        if (clue.size() == 2 && clue[0] == -1 && clue[1] == -1) {
            primeClue = true;
        }
    }

    // 初始化候选集合
    vector<vector<int>> candidates(3);
    if (primeClue) {
        // 素数集合：2,3,5,7
        for (int i = 0; i < 3; i++) {
            candidates[i] = { 2, 3, 5, 7 };
        }
    }
    else {
        // 0-9所有数字
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j <= 9; j++) {
                candidates[i].push_back(j);
            }
        }
    }

    // 处理固定数字线索
    vector<int> fixed(3, -1); // -1表示未固定
    for (auto& clue : clues) {
        if (clue.size() == 3) {
            if (clue[0] != -1 && clue[1] == -1 && clue[2] == -1) {
                int b = clue[0];
                if (fixed[0] != -1) {
                    if (fixed[0] != b) return {};
                }
                else {
                    if (primeClue) {
                        if (b != 2 && b != 3 && b != 5 && b != 7) return {};
                    }
                    candidates[0] = { b };
                    fixed[0] = b;
                }
            }
            else if (clue[1] != -1 && clue[0] == -1 && clue[2] == -1) {
                int b = clue[1];
                if (fixed[1] != -1) {
                    if (fixed[1] != b) return {};
                }
                else {
                    if (primeClue) {
                        if (b != 2 && b != 3 && b != 5 && b != 7) return {};
                    }
                    candidates[1] = { b };
                    fixed[1] = b;
                }
            }
            else if (clue[2] != -1 && clue[0] == -1 && clue[1] == -1) {
                int b = clue[2];
                if (fixed[2] != -1) {
                    if (fixed[2] != b) return {};
                }
                else {
                    if (primeClue) {
                        if (b != 2 && b != 3 && b != 5 && b != 7) return {};
                    }
                    candidates[2] = { b };
                    fixed[2] = b;
                }
            }
        }
    }

    // 处理奇偶性线索
    for (auto& clue : clues) {
        if (clue.size() == 2 && !(clue[0] == -1 && clue[1] == -1)) {
            int pos_index = clue[0] - 1;
            int parity = clue[1]; // 0:偶数, 1:奇数
            if (pos_index < 0 || pos_index > 2) continue; // 位置无效则跳过

            vector<int> newCandidates;
            for (int num : candidates[pos_index]) {
                if (parity == 0 && num % 2 == 0) newCandidates.push_back(num);
                else if (parity == 1 && num % 2 == 1) newCandidates.push_back(num);
            }
            candidates[pos_index] = newCandidates;
            if (newCandidates.empty()) return {};
        }
    }

    // 三重循环生成所有密码
    vector<vector<int>> result;
    for (int a : candidates[0]) {
        for (int b : candidates[1]) {
            for (int c : candidates[2]) {
                if (primeClue) {
                    // 素数线索要求互不相同
                    if (a == b || a == c || b == c) continue;
                }
                result.push_back({ a, b, c });
            }
        }
    }

    return result;
}
vector<string> convertVectorIntToVectorString(const vector<vector<int>>& input) {
    vector<string> result;
    for (const auto& subVec : input) {
        string s;
        // 直接连接所有数字，不添加任何分隔符或格式字符
        for (int num : subVec) {
            s += to_string(num);  // 将每个数字转为字符串后直接拼接
        }
        result.push_back(s);
    }
    return result;
}

int main() {
    // 示例输入: [[-1,-1],[2,0],[-1,-1,5]]
    vector<vector<int>> clues = { {-1,-1},{2,0},{-1,-1,5} };
    PasswordLock lock;
   vector<vector<int>> passwords = findPasswords(clues);
   
   vector<string>user = convertVectorIntToVectorString(passwords);
   for (int i = 0; i < user.size(); i++)
   {
       cout << user[i]  << endl;
       cout << "对应哈希值是：" << endl;
       cout<< lock.hashPassword(user[i])<<endl;
   }
  
    


    // 输出结果
    for (auto& pwd : passwords) {
        for (int digit : pwd) {
            cout << digit;
        }
        cout << endl;
    }

   

    // 用户设置密码
    std::string userPassword;
    std::cout << "请设置一个三位数密码: ";
    std::cin >> userPassword;

    // 加密密码
    std::string passwordHash = lock.hashPassword(userPassword);
    std::cout << "密码已加密，哈希值: " << passwordHash << std::endl;

    // 验证密码
    std::string testPassword;
    std::cout << "请输入密码进行验证: ";
    std::cin >> testPassword;

    if (lock.verifyPassword(testPassword, passwordHash)) {
        std::cout << "密码验证成功！" << std::endl;
    }
    else {
        std::cout << "密码验证失败！" << std::endl;
    }

    return 0;
}
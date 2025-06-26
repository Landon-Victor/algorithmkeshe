#include <iostream>
#include <vector>
#include "Decode.h"
using namespace std;

vector<vector<int>> findPasswords(vector<vector<int>>& clues) {
    // �����������
    bool primeClue = false;
    for (auto& clue : clues) {
        if (clue.size() == 2 && clue[0] == -1 && clue[1] == -1) {
            primeClue = true;
        }
    }

    // ��ʼ����ѡ����
    vector<vector<int>> candidates(3);
    if (primeClue) {
        // �������ϣ�2,3,5,7
        for (int i = 0; i < 3; i++) {
            candidates[i] = { 2, 3, 5, 7 };
        }
    }
    else {
        // 0-9��������
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j <= 9; j++) {
                candidates[i].push_back(j);
            }
        }
    }

    // ����̶���������
    vector<int> fixed(3, -1); // -1��ʾδ�̶�
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

    // ������ż������
    for (auto& clue : clues) {
        if (clue.size() == 2 && !(clue[0] == -1 && clue[1] == -1)) {
            int pos_index = clue[0] - 1;
            int parity = clue[1]; // 0:ż��, 1:����
            if (pos_index < 0 || pos_index > 2) continue; // λ����Ч������

            vector<int> newCandidates;
            for (int num : candidates[pos_index]) {
                if (parity == 0 && num % 2 == 0) newCandidates.push_back(num);
                else if (parity == 1 && num % 2 == 1) newCandidates.push_back(num);
            }
            candidates[pos_index] = newCandidates;
            if (newCandidates.empty()) return {};
        }
    }

    // ����ѭ��������������
    vector<vector<int>> result;
    for (int a : candidates[0]) {
        for (int b : candidates[1]) {
            for (int c : candidates[2]) {
                if (primeClue) {
                    // ��������Ҫ�󻥲���ͬ
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
        // ֱ�������������֣�������κηָ������ʽ�ַ�
        for (int num : subVec) {
            s += to_string(num);  // ��ÿ������תΪ�ַ�����ֱ��ƴ��
        }
        result.push_back(s);
    }
    return result;
}

int main() {
    // ʾ������: [[-1,-1],[2,0],[-1,-1,5]]
    vector<vector<int>> clues = { {-1,-1},{2,0},{-1,-1,5} };
    PasswordLock lock;
   vector<vector<int>> passwords = findPasswords(clues);
   
   vector<string>user = convertVectorIntToVectorString(passwords);
   for (int i = 0; i < user.size(); i++)
   {
       cout << user[i]  << endl;
       cout << "��Ӧ��ϣֵ�ǣ�" << endl;
       cout<< lock.hashPassword(user[i])<<endl;
   }
  
    


    // ������
    for (auto& pwd : passwords) {
        for (int digit : pwd) {
            cout << digit;
        }
        cout << endl;
    }

   

    // �û���������
    std::string userPassword;
    std::cout << "������һ����λ������: ";
    std::cin >> userPassword;

    // ��������
    std::string passwordHash = lock.hashPassword(userPassword);
    std::cout << "�����Ѽ��ܣ���ϣֵ: " << passwordHash << std::endl;

    // ��֤����
    std::string testPassword;
    std::cout << "���������������֤: ";
    std::cin >> testPassword;

    if (lock.verifyPassword(testPassword, passwordHash)) {
        std::cout << "������֤�ɹ���" << std::endl;
    }
    else {
        std::cout << "������֤ʧ�ܣ�" << std::endl;
    }

    return 0;
}
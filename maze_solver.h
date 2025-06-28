#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>
#include <fstream>
#include "json/json.h"
using namespace std;

class MazeSolver {
private:
    vector<vector<string>> maze;
    int rows, cols;
    int startx, starty, endx, endy;
    vector<vector<int>> resourceValue;
    vector<vector<int>> res_id;
    int K, total_mask;
    vector<pair<int, int>> path;
	int ans;
public:
    MazeSolver() = default;
    void load(const string& filename="map.json")
    {
        ifstream ifs(filename);
        if (!ifs.is_open()) {
            cerr << "�޷����ļ�: " << filename << endl;
            return;
        }

        // ��ȡ�����ļ����ݵ��ַ���
        string json_str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());

        // ʹ�� jsoncpp ����
        Json::Value root;
        Json::Reader reader;
        if (!reader.parse(json_str, root)) {
            cerr << "JSON����ʧ��" << endl;
            return;
        }

        // ��ȡmaze����
        const Json::Value& arr_maze = root["maze"];
        maze.clear();
        for (Json::Value::ArrayIndex i = 0; i < arr_maze.size(); ++i) {
            vector<string> row;
            const Json::Value& arr_row = arr_maze[i];
            for (Json::Value::ArrayIndex j = 0; j < arr_row.size(); ++j) {
                row.push_back(arr_row[j].asString());
            }
            maze.push_back(row);
        }
    }
    // ���캯���������Թ�����
    void init(const string filename="map.json") {
        load(filename);
        rows = maze.size();
        if (rows > 0) cols = maze[0].size();

        // ���������յ�
        findStartAndEnd();

        // ��ʼ����Դ����Ϣ
        initResources();
    }

private:
    // �������S���յ�E��λ��
    void findStartAndEnd() {
        startx = starty = endx = endy = -1;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (maze[i][j] == "S") {
                    startx = i;
                    starty = j;
                }
                if (maze[i][j] == "E") {
                    endx = i;
                    endy = j;
                }
            }
        }
    }

    // ��ʼ����Դ��ID�ͷ�ֵ
    void initResources() {
        resourceValue = vector<vector<int>>(rows, vector<int>(cols, 0));
        res_id = vector<vector<int>>(rows, vector<int>(cols, -1));
        K = 0;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (maze[i][j] == "G") {
                    resourceValue[i][j] = 5;
                    res_id[i][j] = K++;
                }
                else if (maze[i][j] == "T") {
                    resourceValue[i][j] = -3;
                    res_id[i][j] = K++;
                }
            }
        }
        total_mask = 1 << K;
    }

public:
    // ����⺯�������������Դֵ����ͨ�����ô���·��
    void solve() {
        // ��������յ��Ƿ�Ϸ�
        if (startx == -1 || endx == -1 ||
            maze[startx][starty] == "#" || maze[endx][endy] == "#") {
            return ;
        }

        // dp�����·����¼����
        vector<vector<vector<int>>> dp(rows, vector<vector<int>>(cols, vector<int>(total_mask, INT_MIN)));
        vector<vector<vector<tuple<int, int, int, int>>>> pre(rows,
            vector<vector<tuple<int, int, int, int>>>(cols,
                vector<tuple<int, int, int, int>>(total_mask, { -1, -1, -1, -1 })));
        queue<tuple<int, int, int>> q;

        // ��ʼ�����
        int mask0 = 0;
        if (res_id[startx][starty] != -1) {
            int id0 = res_id[startx][starty];
            mask0 = (1 << id0);
            dp[startx][starty][mask0] = resourceValue[startx][starty];
        }
        else {
            dp[startx][starty][mask0] = 0;
        }
        q.push({ startx, starty, mask0 });

        int dirs[4][2] = { {1,0}, {0,1}, {-1,0}, {0,-1} };

        // BFS����
        while (!q.empty()) {
            int i = get<0>(q.front());
            int j = get<1>(q.front());
            int mask = get<2>(q.front());
            q.pop();

            for (int d = 0; d < 4; d++) {
                int ni = i + dirs[d][0];
                int nj = j + dirs[d][1];
                if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && maze[ni][nj] != "#") {
                    int new_mask = mask;
                    int add = 0;
                    if (res_id[ni][nj] != -1) {
                        int k = res_id[ni][nj];
                        if (!(mask & (1 << k))) {
                            new_mask = mask | (1 << k);
                            add = resourceValue[ni][nj];
                        }
                    }
                    int new_value = dp[i][j][mask] + add;
                    if (new_value > dp[ni][nj][new_mask]) {
                        dp[ni][nj][new_mask] = new_value;
                        pre[ni][nj][new_mask] = { i, j, mask, d };
                        q.push({ ni, nj, new_mask });
                    }
                }
            }
        }

        // �����յ�������Դֵ
        ans = INT_MIN;
        int best_mask = 0;
        for (int mask = 0; mask < total_mask; mask++) {
            if (dp[endx][endy][mask] > ans) {
                ans = dp[endx][endy][mask];
                best_mask = mask;
            }
        }

        if (ans != INT_MIN) {
            path.clear();
            int i = endx, j = endy, mask = best_mask;
            path.push_back({ i, j });

            while (true) {
                int pi = get<0>(pre[i][j][mask]);
                int pj = get<1>(pre[i][j][mask]);
                int pmask = get<2>(pre[i][j][mask]);
                int d = get<3>(pre[i][j][mask]);
                if (pi == -1) break;

                path.push_back({ pi, pj });

                if (pi == startx && pj == starty && pmask == mask0) {
                    break;
                }

                i = pi;
                j = pj;
                mask = pmask;
            }

            reverse(path.begin(), path.end());
        }

    }

    // ��ӡ·���ĸ�������
    void printPath() {
        cout << "·��: ";
        for (size_t k = 0; k < path.size(); k++) {
            cout << "(" << path[k].first << "," << path[k].second << ")";
            if (k < path.size() - 1) cout << " -> ";
        }
        cout << endl;
		std::cout << "�����Դֵ: " << ans << endl;
        std::cout << ans << endl;
    }
    void save_path(const string filename="path.json")
    {
        Json::Value root;

        // ����maze
        Json::Value arr_maze;
        for (const auto& row : maze) {
            Json::Value arr_row;
            for (const auto& cell : row) {
                arr_row.append(cell);
            }
            arr_maze.append(arr_row);
        }
        root["maze"] = arr_maze;

        // ���������Դֵ
        root["max_resource"] = ans;

        // ��������·��
        Json::Value arr_path;
        for (const auto& p : path) {
            Json::Value point;
            point.append(p.first);
            point.append(p.second);
            arr_path.append(point);
        }
        root["optimal_path"] = arr_path;

        // д���ļ�
        ofstream ofs(filename);
        if (!ofs.is_open()) {
            cerr << "�޷�д���ļ�: " << filename << endl;
            return;
        }
        ofs << root.toStyledString();
        ofs.close();
    }
};

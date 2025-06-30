#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>
#include <fstream>
#include "json/json.h"
#include "vector2.h"
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
            cerr << "无法打开文件: " << filename << endl;
            return;
        }

        string json_str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
        Json::Value root;
        Json::Reader reader;
        if (!reader.parse(json_str, root)) {
            cerr << "JSON解析失败" << endl;
            return;
        }

        // 读取maze数组
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

        string result_filename = "result_" + filename;
        ifstream ifs_result(result_filename);
        path.clear();
        if (ifs_result.is_open()) {
            string json_str_result((istreambuf_iterator<char>(ifs_result)), istreambuf_iterator<char>());
            Json::Value root_result;
            Json::Reader reader_result;
            if (reader_result.parse(json_str_result, root_result)) {
                const Json::Value& arr_path = root_result["optimal_path"];
                for (Json::Value::ArrayIndex i = 0; i < arr_path.size(); ++i) {
                    if (arr_path[i].isArray() && arr_path[i].size() == 2) {
                        int x = arr_path[i][0].asInt();
                        int y = arr_path[i][1].asInt();
                        path.emplace_back(x, y);
                    }
                }
                if (root_result.isMember("max_resource")) {
                    ans = root_result["max_resource"].asInt()/10;
                }
            }
        }
    }
    // 构造函数，接收迷宫数据
    void init(const string filename="map.json") {
        load(filename);
        rows = maze.size();
        if (rows > 0) cols = maze[0].size();

        // 查找起点和终点
        findStartAndEnd();

        // 初始化资源点信息
        initResources();
    }
    vector<Vector2> getPath() const {
        vector<Vector2> vec_path;
        for (const auto& p : path) {
            vec_path.emplace_back(p.first, p.second);
        }
        return vec_path;
	}
private:
    // 查找起点S和终点E的位置
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

    // 初始化资源点ID和分值
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
    // 主求解函数，返回最大资源值，并通过引用传出路径
    void solve() {
        // 检查起点和终点是否合法
        if (startx == -1 || endx == -1 ||
            maze[startx][starty] == "#" || maze[endx][endy] == "#") {
            return ;
        }

        // dp数组和路径记录数组
        vector<vector<vector<int>>> dp(rows, vector<vector<int>>(cols, vector<int>(total_mask, INT_MIN)));
        vector<vector<vector<tuple<int, int, int, int>>>> pre(rows,
            vector<vector<tuple<int, int, int, int>>>(cols,
                vector<tuple<int, int, int, int>>(total_mask, { -1, -1, -1, -1 })));
        queue<tuple<int, int, int>> q;

        // 初始化起点
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

        // BFS搜索
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

        // 查找终点的最大资源值
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

    // 打印路径的辅助函数
    void printPath() {
        cout << "路径: ";
        for (size_t k = 0; k < path.size(); k++) {
            cout << "(" << path[k].first << "," << path[k].second << ")";
            if (k < path.size() - 1) cout << " -> ";
        }
        cout << endl;
		std::cout << "最大资源值: " << ans << endl;
		std::cout << "路径长度: " << path.size() << endl;
    }
    void save_path(const string& filename = "path.json")
    {
        // 先用JsonCpp生成max_resource和optimal_path部分
        Json::Value root;
        root["max_resource"] = ans;
        // optimal_path
        Json::Value arr_path(Json::arrayValue);
        for (const auto& p : path) {
            Json::Value point(Json::arrayValue);
            point.append(p.first);
            point.append(p.second);
            arr_path.append(point);
        }
        root["optimal_path"] = arr_path;

        // 手动写入文件，maze部分自定义格式
        std::ofstream ofs(filename);
        if (!ofs.is_open()) {
            std::cerr << "无法写入文件: " << filename << std::endl;
            return;
        }
        ofs << "{\n";
        // maze
        ofs << "  \"maze\": [\n";
        for (size_t i = 0; i < maze.size(); ++i) {
            ofs << "    [";
            for (size_t j = 0; j < maze[i].size(); ++j) {
                ofs << '"' << maze[i][j] << '"';
                if (j + 1 < maze[i].size()) ofs << ",";
            }
            ofs << "]";
            if (i + 1 < maze.size()) ofs << ",";
            ofs << "\n";
        }
        ofs << "  ],\n";
        // max_resource
        ofs << "  \"max_resource\": " << ans << ",\n";
        // optimal_path
        ofs << "  \"optimal_path\": [\n";
        for (Json::ArrayIndex i = 0; i < root["optimal_path"].size(); ++i) {
            ofs << "    [" << root["optimal_path"][i][0] << "," << root["optimal_path"][i][1] << "]";
            if (i + 1 < root["optimal_path"].size()) ofs << ",";
            ofs << "\n";
        }
        ofs << "  ]\n";
        ofs << "}\n";
        ofs.close();
    }
};

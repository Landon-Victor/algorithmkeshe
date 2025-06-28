#include <queue>
#include <fstream>
#include <locale>
#include <codecvt>
#include <algorithm> 
#include <iostream>
#include "json/json.h"
#include "map_generator.h"
#include "vector2.h"

typedef struct ArcNode
{
	int adjvex;
	struct ArcNode* nextarc;
}ArcNode;
typedef struct VexNode
{
	int data;
	ArcNode* firstarc;
}VexNode, AdjList[300];
typedef struct
{
	AdjList adjlist;
	int vexnum, arcnum;
}ALGraph;

void MapGenerator::divide_conquer_main(int size)
{ 
	map.clear();
	map.resize(2*size+1, std::vector<Content>(2*size+1, Content::none));
	//初始化,四周为墙
	for(int i = 0; i < 2 * size + 1; ++i)
	{
		map[0][i] = Content::wall;
		map[2 * size][i] = Content::wall;
		map[i][0] = Content::wall;
		map[i][2 * size] = Content::wall;
	}
	//把平面分成四块，分治生成迷宫
	//（奇数,奇数）位置为迷宫单元，其余位置描述联通性
	int row_start = 1, col_start = 1;
	int row_end = 2 * size - 1, col_end = 2 * size - 1;
	divide_conquer(row_start, row_end, col_start, col_end);
	//随机选取边缘位置作为入口
	Vector2 b;
	int a = rand() % 4; //0:上, 1:下, 2:左, 3:右
	if(a == 0) //上
	{
		int col = rand() % size*2 + 1;
		map[0][col] = Content::start;
		b = Vector2(0, col);
	}
	else if(a == 1) //下
	{
		int col = rand() %  size*2 + 1;
		map[2 * size][col] = Content::start;
		b = Vector2(2 * size, col);
	}
	else if(a == 2) //左
	{
		int row = rand() % size*2 + 1;
		map[row][0] = Content::start;
		b = Vector2(row, 0);
	}
	else //右
	{
		int row = rand() % size *2+ 1;
		map[row][2 * size] = Content::start;
		b = Vector2(row, 2 * size);
	}
	//广度优先遍历选取出口
	bfs_traverse(b);

	// 统计可用单元格
	std::vector<Vector2> available_cells;
	for (int i = 1; i < 2 * size; i += 2)
	{
		for (int j = 1; j < 2 * size; j += 2)
		{
			if (map[i][j] == Content::none)
				available_cells.push_back(Vector2(i, j));
		}
	}

	// 随机分配陷阱，保证至少3个
	std::vector<int> idxs(available_cells.size());
	for (int i = 0; i < idxs.size(); ++i) idxs[i] = i;
	std::random_shuffle(idxs.begin(), idxs.end());
	int trap_cnt = 0;
	for (int k = 0; k < idxs.size() && trap_cnt < 3; ++k) {
		Vector2& v = available_cells[idxs[k]];
		map[(int)v.x][(int)v.y] = Content::trap;
		trap_cnt++;
	}
	// 随机分配资源，保证至少10个
	int money_cnt = 0;
	for (int k = trap_cnt; k < idxs.size() && money_cnt < 10; ++k) {
		Vector2& v = available_cells[idxs[k]];
		if (map[(int)v.x][(int)v.y] == Content::none) {
			map[(int)v.x][(int)v.y] = Content::money;
			money_cnt++;
		}
	}
	int locker_cnt = 0;
	// 其余格子按原概率分配
	for (int k = trap_cnt + money_cnt; k < idxs.size(); ++k) {
		Vector2& v = available_cells[idxs[k]];
		if (map[(int)v.x][(int)v.y] == Content::none) {
			int r = rand() % 100;
			if (r < 3) {
				map[(int)v.x][(int)v.y] = Content::trap;
				trap_cnt++;
			}
			else if (r < 6) {
				map[(int)v.x][(int)v.y] = Content::locker;
				locker_cnt++;
			}
			else if (r < 10) {
				map[(int)v.x][(int)v.y] = Content::money;
				money_cnt++;
			}
		}
	}

	// path上随机一个格子放机关
	std::vector<Vector2> path_cells;
	for (auto& v : path) {
		int x = (int)v.x, y = (int)v.y;
		if (map[x][y] == Content::none)
			path_cells.push_back(v);
	}
	if (!path_cells.empty()) {
		int idx = rand() % path_cells.size();
		Vector2& v = path_cells[idx];
		map[(int)v.x][(int)v.y] = Content::locker;
		locker_cnt++;
	}
	std::cout << "生成的地图大小: " << map.size()<<std::endl;
	std::cout << "入口位置: " << b.x << ", " << b.y << std::endl;
	std::cout << "出口位置: " << path.back().x << ", " << path.back().y << std::endl;
	std::cout << "路径长度: " << path.size() << std::endl;
	std::cout << "陷阱数量: " << trap_cnt<< std::endl;
	std::cout << "金币数量: " <<money_cnt<< std::endl;
	std::cout << "机关数量: " << locker_cnt << std::endl;
	std::cout << "Boss位置: " << path[path.size() - 2].x << ", " << path[path.size() - 2].y << std::endl;
}

void MapGenerator::divide_conquer(int row_start, int row_end, int col_start, int col_end)
{
	if(row_end - row_start <= 1 || col_end - col_start <= 1)
		return;
	//随机选择横向和纵向的墙位置
	//墙位置必须是偶数行或偶数列
	int row_div = rand() % ((row_end - row_start)/2)*2 + row_start + 1;
	int col_div = rand() % ((col_end - col_start) / 2) * 2 + col_start + 1;
	for(int i = row_start; i <= row_end; ++i)
	{
		map[i][col_div] = Content::wall;
	}
	for(int i = col_start; i <= col_end; ++i)
	{
		map[row_div][i] = Content::wall; 
	}
	//在分割线上随机打通三个通道
	//通道位置必须是奇数行或奇数列
	int row_hole = rand() % ((row_end - row_start) / 2+1) * 2 + row_start;
	int col_hole = rand() % ((col_end - col_start) / 2 + 1) * 2 + col_start;
	int a = rand() % 2;
	if (a == 0)
	{
		if(row_hole< row_div)
		{
			int row_hole2 = rand() % ((row_end - row_div-1) / 2 + 1) * 2 + row_div + 1;
			map[row_hole][col_div] = Content::none;
			map[row_hole2][col_div] = Content::none;
			map[row_div][col_hole] = Content::none;
		}
		else
		{
			int row_hole2 = rand() % ((row_div - row_start - 1) / 2 + 1) * 2 + row_start;
			map[row_hole][col_div] = Content::none;
			map[row_hole2][col_div] = Content::none;
			map[row_div][col_hole] = Content::none;
		}
	}
	else
	{
		if(col_hole < col_div)
		{
			int col_hole2 = rand() % ((col_end - col_div - 1) / 2 + 1) * 2 + col_div + 1;
			map[row_div][col_hole] = Content::none;
			map[row_div][col_hole2] = Content::none;
			map[row_hole][col_div] = Content::none;
		}
		else
		{
			int col_hole2 = rand() % ((col_div - col_start - 1) / 2 + 1) * 2 + col_start;
			map[row_div][col_hole] = Content::none;
			map[row_div][col_hole2] = Content::none;
			map[row_hole][col_div] = Content::none;
		}
	}
	//递归处理四个子区域
	divide_conquer(row_start, row_div-1, col_start, col_div-1);
	divide_conquer(row_start, row_div-1, col_div+1, col_end);
	divide_conquer(row_div+1, row_end, col_start, col_div-1);
	divide_conquer(row_div+1, row_end, col_div+1, col_end);
}

void MapGenerator::bfs_traverse(const Vector2& start)
{
	int rows = static_cast<int>(map.size());
	int cols = static_cast<int>(map[0].size());
	//
	std::vector<std::vector<int>> dist(rows, std::vector<int>(cols, -1));
	std::vector<std::vector<Vector2>> prev(rows, std::vector<Vector2>(cols, Vector2(-1, -1)));

	std::queue<Vector2> q;
	int sx = static_cast<int>(start.x);
	int sy = static_cast<int>(start.y);
	dist[sx][sy] = 0;
	q.push(start);

	// 四个方向
	int dx[4] = { 1, -1, 0, 0 };
	int dy[4] = { 0, 0, 1, -1 };

	Vector2 farthest = start;
	int max_dist = 0;

	while (!q.empty()) {
		Vector2 cur = q.front();
		q.pop();
		int x = static_cast<int>(cur.x);
		int y = static_cast<int>(cur.y);

		for (int d = 0; d < 4; ++d) {
			int nx = x + dx[d];
			int ny = y + dy[d];
			if (nx >= 0 && nx < rows && ny >= 0 && ny < cols)
				if (map[nx][ny] == Content::none && dist[nx][ny] == -1) {
					dist[nx][ny] = dist[x][y] + 1;
					prev[nx][ny] = cur;
					q.push(Vector2(nx, ny));
					if (dist[nx][ny] > max_dist) {
						max_dist = dist[nx][ny];
						farthest = Vector2(nx, ny);
					}
				}
		}
	}
	// 回溯路径
	path.clear();
	Vector2 cur = farthest;
	while (!(cur == start)) {
		path.push_back(cur);
		Vector2 p = prev[static_cast<int>(cur.x)][static_cast<int>(cur.y)];
		if (p.x == -1 && p.y == -1) break;
		cur = p;
	}
	path.push_back(start);
	std::reverse(path.begin(), path.end());
	map[static_cast<int>(farthest.x)][static_cast<int>(farthest.y)] = Content::exit;
    Vector2 p = path[path.size() - 2];
	map[static_cast<int>(p.x)][static_cast<int>(p.y)] = Content::boss;
}

void MapGenerator::save_map(const std::wstring& filename)
{
	std::ofstream ofs;
#ifdef _WIN32
	std::string fn(filename.begin(), filename.end());
	ofs.open(fn, std::ios::out | std::ios::trunc);
#else
	std::ofstream ofs(filename);
#endif
	if (!ofs.is_open()) return;

	ofs << "{\n  \"maze\": [\n";
	for (size_t i = 0; i < map.size(); ++i) {
		ofs << "    [";
		for (size_t j = 0; j < map[i].size(); ++j) {
			std::string cell;
			switch (map[i][j]) {
			case Content::none:   cell = " "; break;
			case Content::wall:   cell = "#"; break;
			case Content::start:  cell = "S"; break;
			case Content::exit:   cell = "E"; break;
			case Content::trap:   cell = "T"; break;
			case Content::locker: cell = "L"; break;
			case Content::boss:   cell = "B"; break;
			case Content::money:  cell = "G"; break;
			default:              cell = " "; break;
			}
			ofs << "\"" << cell << "\"";
			if (j + 1 < map[i].size()) ofs << ",";
		}
		ofs << "]";
		if (i + 1 < map.size()) ofs << ",\n";
		else ofs << "\n";
	}
	ofs << "  ]\n}\n";
	ofs.close();
}
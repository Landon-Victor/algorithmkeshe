#pragma once
#include <vector>
#include <string>
#include "vector2.h"

enum class Content
{
	none,
	wall,
	start,
	exit,
	trap,
	locker,
	boss,
	money
};
typedef std::vector<std::vector<Content>> Map;
class MapGenerator
{
public:
	 Map get_a_map(int size)
	{
		divide_conquer_main(size);
		save_map(L"map.csv");
		return map;
	}
	void save_map(const std::wstring& filename);
	MapGenerator() = default;
	~MapGenerator() = default;
private:
	void divide_conquer_main(int size);
	void divide_conquer(int row_start,int row_end,int col_start, int col_end);
	void bfs_traverse(const Vector2 &start);
private:
	Map map;
	std::vector<Vector2> path;
};
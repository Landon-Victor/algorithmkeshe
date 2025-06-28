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
		for(int i = 0; i < map.size(); ++i)
		{
			for (int j = 0; j < map[i].size(); ++j)
			{
				switch (map[i][j])
				{
				case Content::none:
					std::cout << "_";
					break;
				case Content::wall:
					std::cout << "#";
					break;
				case Content::start:
					std::cout << "S";
					break;
				case Content::exit:
					std::cout << "E";
					break;
				case Content::trap:
					std::cout << "T";
					break;
				case Content::locker:
					std::cout << "L";
					break;
				case Content::boss:
					std::cout << "B";
					break;
				case Content::money:
					std::cout << "M";
					break;
				default:
					std::cout << "?";
					break;
				}
			}
			std::cout << std::endl;
		}
		save_map();
		return map;
	}
	void save_map(const std::wstring& filename=L"map.json");
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
#pragma once
#include <vector>
enum class Content
{
	none,
	wall,
	role,
	start,
	exit
};
typedef std::vector<std::vector<Content>> Map;
class MapGenerator
{
public:
	std::vector<std::vector<Content>> get_a_map()
	{
		Map map;
		map.resize(10, std::vector<Content>(10, Content::none));
		map[0][0] = Content::start;
		map[9][9] = Content::exit;
		for (int i = 0; i < 10; ++i)
		{
			map[i][0] = Content::wall;
		}
		for(int i = 0; i < 10; ++i)
		{
			map[i][9] = Content::wall;
		}
		for(int i = 0; i < 10; ++i)
		{
			map[0][i] = Content::wall;
		}
		for(int i = 0; i < 10; ++i)
		{
			map[9][i] = Content::wall;
		}
		map[5][5] = Content::role;
		return map;
	}
	MapGenerator() = default;
	~MapGenerator() = default;
};
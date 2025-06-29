#pragma once
#include <vector>
#include <string>
#include "vector2.h"
#include "camera.h"
#include "timer.h"

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
	MapGenerator()
	{
		timer.set_wait_time(100);
		timer.set_one_shot(false);
		timer.set_callback([this]()
		{
			if (index < generated_secquences.size()-1)
			{
				index++;
			}
			else
			{
				timer.pause();
				is_show = false;
				Sleep(1000);
			}
			});

	}
	~MapGenerator() = default;
	void on_render();
	void on_update(int delta)
	{
		if (!is_show)
			return;
		timer.on_update(delta);
	}
	bool get_is_show() const { return is_show; }
private:
	void divide_conquer_main(int size);
	void divide_conquer(int row_start,int row_end,int col_start, int col_end);
	void bfs_traverse(const Vector2 &start);
private:
	Map map;
	std::vector<Vector2> path;
	std::vector<std::vector<Vector2>> generated_secquences;//Â·¾¶Éú³ÉË³Ðò
	Timer timer;
	int index = 0;
	bool is_show = true;
};
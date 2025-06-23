#pragma once
#include "scene_manager.h"
#include "map_generator.h"
#include "static_image.h"
#include "object.h"
class GameScene :public Scene
{
public:
	GameScene() {}
	~GameScene() = default;
	void on_enter() {
		map = map_generator.get_a_map();
		for(int i=0;i<map.size();i++)
		{
			for (int j = 0;j < map[i].size();j++)
			{
				switch (map[i][j])
				{
				case Content::role:
				{
					player.set_position(Vector2(i, j));
					break;
				}
				case Content::wall:
				{
					Wall * wall = new Wall(i,j);
					objects.push_back(wall);
				}
				}
			}
		}
	}
	void on_exit() {}
	void on_render(const Camera& camera)override
	{
		player.on_render(camera);
		for (auto& it : objects)
		{
			it->on_render(camera);
		}
	}
	void on_input(const ExMessage& message)override
	{
		player.on_input(message);
	}
	void on_update(int delta)override
	{
		player.on_update(delta);
	}
private:
	MapGenerator map_generator;
private:
	int size=7;
	Map map;
	std::vector<Object*> objects;
	Player player;
};
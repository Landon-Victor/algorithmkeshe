#pragma once
#include "scene_manager.h"
#include "map_generator.h"
#include "static_image.h"
#include "object.h"

extern const int OBJECT_SIZE;
class GameScene :public Scene
{
public:
	GameScene() {}
	~GameScene() = default;
	void on_enter() {
		dilao.set_position(Vector2(0, 0));
		dilao.set_size(Vector2(640, 640));
		dilao.set_image("dilao");
		map = map_generator.get_a_map(10);
		for(int i=0;i<map.size();i++)
		{
			for (int j = 0;j < map[i].size();j++)
			{
				switch (map[i][j])
				{
				case Content::start:
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
		dilao.on_render();
		for (auto& it : objects)
		{
			it->on_render(camera);
		}
		player.on_render(camera);
	}
	void on_input(const ExMessage& message)override
	{
		player.on_input(message);
	}
	void on_update(int delta)override
	{
		player.on_update(delta);
		Camera* camera = ResourcesManager::instance()->get_camera();
		int CameraWidth = camera->get_camera_width();
		if (camera == nullptr) return;
		Vector2 pos = player.get_position()-Vector2(320,320);
		if (pos.x < 0) pos.x = 0;
		if (pos.y < 0) pos.y = 0;
		if (pos.x + CameraWidth > (2 * size + 1) * OBJECT_SIZE)pos.x = (2 * size + 1) * OBJECT_SIZE - CameraWidth;
		if (pos.y + CameraWidth > (2 * size + 1) * OBJECT_SIZE)pos.y = (2 * size + 1) * OBJECT_SIZE - CameraWidth;
		camera->set_position(pos);
	}
private:
	MapGenerator map_generator;
private:
	int size=10;
	Map map;
	std::vector<Object*> objects;
	Player player;
	StaticImage dilao;
};
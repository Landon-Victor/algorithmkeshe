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
		for (int i = 0;i < map.size();i++)
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
					Wall* wall = new Wall(i, j);
					objects.push_back(wall);
					break;
				}
				case Content::money:
				{
					Money* money = new Money(i, j);
					objects.push_back(money);
					break;
				}
				case Content::trap:
				{
					Trap* trap = new Trap(i, j);
					objects.push_back(trap);
					break;
				}
				case Content::boss:
				{
					Boss* boss = new Boss(i, j);
					objects.push_back(boss);
					break;
				}
				case Content::locker:
				{
					Locker* locker = new Locker(i, j);
					objects.push_back(locker);
					break;
				}
				case Content::none:
				{
					Path* path = new Path(i, j);
					objects.push_back(path);
					break;
				}
				case Content::exit:
				{
					Exit* exit = new Exit(i, j);
					objects.push_back(exit);
					break;
				}
				default:
					break;
				}
			}
		}
		cover.set_image("cover");
		cover.set_position(Vector2(640, 0));
		cover.set_size(Vector2(240, 640));
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
		cover.on_render();
		//±ß¿ò
		int x1 = 0, y1 = 0;
		int x2 = 640, y2 = 640;
		setlinecolor(RGB(255, 255, 255));
		line(x1, y1, x2, y1);
		line(x1, y2, x2, y2);
		line(x1, y1, x1, y2);
		line(x2, y1, x2, y2);
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
	StaticImage cover;
};
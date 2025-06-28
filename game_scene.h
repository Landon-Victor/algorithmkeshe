#pragma once
#include "scene_manager.h"
#include "map_generator.h"
#include "static_image.h"
#include "object.h"

extern const int OBJECT_SIZE;
class GameScene :public Scene
{
	friend class Wall;
	friend class Money;
	friend class Trap;
	friend class Boss;
	friend class Locker;
	friend class Path;
	friend class Exit;
	friend class Start;
	friend class Player;
public:
	GameScene() {}
	~GameScene() = default;
	void on_enter() {
		dilao.set_position(Vector2(0, 0));
		dilao.set_size(Vector2(640, 640));
		dilao.set_image("dilao");
		map = map_generator.get_a_map(size);
		for (int i = 0;i < map.size();i++)
		{
			for (int j = 0;j < map[i].size();j++)
			{
				switch (map[i][j])
				{
				case Content::start:
				{
					player.set_position(Vector2(i, j),this);
					break;
				}
				case Content::wall:
				{
					Wall* wall = new Wall(i, j,this);
					objects.push_back(wall);
					break;
				}
				case Content::money:
				{
					Money* money = new Money(i, j,this);
					objects.push_back(money);
					break;
				}
				case Content::trap:
				{
					Trap* trap = new Trap(i, j,this);
					objects.push_back(trap);
					break;
				}
				case Content::boss:
				{
					Boss* boss = new Boss(i, j,this);
					objects.push_back(boss);
					break;
				}
				case Content::locker:
				{
					Locker* locker = new Locker(i, j,this);
					objects.push_back(locker);
					break;
				}
				case Content::none:
				{
					Path* path = new Path(i, j,this);
					objects.push_back(path);
					break;
				}
				case Content::exit:
				{
					Exit* exit = new Exit(i, j,this);
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
		//边框
		int x1 = 0, y1 = 0;
		int x2 = 640, y2 = 640;
		setlinecolor(RGB(255, 255, 255));
		line(x1, y1, x2, y1);
		line(x1, y2, x2, y2);
		line(x1, y1, x1, y2);
		line(x2, y1, x2, y2);
		cover.on_render();
		little_on_render();
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
		Vector2 pos = player.get_position() - Vector2(320, 320);
		if (pos.x < 0) pos.x = 0;
		if (pos.y < 0) pos.y = 0;
		if (pos.x + CameraWidth > (2 * size + 1) * OBJECT_SIZE)pos.x = (2 * size + 1) * OBJECT_SIZE - CameraWidth;
		if (pos.y + CameraWidth > (2 * size + 1) * OBJECT_SIZE)pos.y = (2 * size + 1) * OBJECT_SIZE - CameraWidth;
		camera->set_position(pos);
	}
private:
	MapGenerator map_generator;
	void little_on_render()
	{
		// 小地图参数
		const int mini_map_size = 200; // 小地图区域大小
		const int offset_x = 640;      // 右上角起始x（cover区域内）
		const int offset_y = 20;       // 右上角起始y
		int rows = map.size();
		int cols = map[0].size();

		// 偶数行/列为墙，奇数为格子
		int cell_w = mini_map_size / size;
		int cell_h = mini_map_size / size;
		int wall_w = 2;
		int wall_h = 2;

		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < cols; ++j)
			{
				int x = offset_x, y = offset_y, w = cell_w, h = cell_h;
				if (j % 2 == 0) { 
					x += (j / 2) * cell_w + (j / 2) * wall_w;
					w = wall_w;
				}
				else {
					x += (j / 2) * cell_w + ((j + 1) / 2) * wall_w;
					w = cell_w;
				}
				if (i % 2 == 0) { 
					y += (i / 2) * cell_h + (i / 2) * wall_h;
					h = wall_h;
				}
				else {
					y += (i / 2) * cell_h + ((i + 1) / 2) * wall_h;
					h = cell_h;
				}

				if (i % 2 == 1 && j % 2 == 1) 
				{
					switch (map[i][j])
					{
					case Content::money:
						setfillcolor(RGB(255, 165, 0));
						solidrectangle(x + 1, y + 1, x + w - 1, y + h - 1);
						break;
					case Content::locker:
						setfillcolor(RGB(0, 128, 255));
						solidrectangle(x + 1, y + 1, x + w - 1, y + h - 1);
						break;
					case Content::trap:
						setfillcolor(RGB(255, 0, 0));
						solidrectangle(x + 1, y + 1, x + w - 1, y + h - 1);
						break;
					case Content::start:
					case Content::exit:
						setfillcolor(RGB(0, 255, 0));
						solidrectangle(x + 1, y + 1, x + w - 1, y + h - 1);
						break;
					default:
						break;
					}
				}
				else if (map[i][j] == Content::wall)
				{
					setfillcolor(WHITE);
					solidrectangle(x, y, x + w, y + h);
				}
			}
		}
	}
private:
	int size=13;
	Map map;
	std::vector<Object*> objects;
	Player player;
	StaticImage dilao;
	StaticImage cover;
};
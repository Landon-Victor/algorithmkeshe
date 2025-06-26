#pragma once
#include <string>
#include "collision_manager.h"
#include "collision_box.h"
#include "static_image.h"
#include "camera.h"
#include "animation.h"
const int OBJECT_SIZE = 90;
class Object
{
public:
	Object()
	{
		collision_box = CollisionManager::instance()->create_collision_box();
	}
	virtual ~Object()
	{
		if (collision_box != nullptr)
		{
			CollisionManager::instance()->destroy_collision_box(collision_box);
			collision_box = nullptr;
		}
	}
	virtual void on_render(const Camera&camera)=0;
protected:
	Vector2 logic_pos;
	Vector2 real_pos;
	CollisionBox* collision_box = nullptr;
	StaticImage image;
};

class Wall : public Object
{
public:
	Wall() = default;
	Wall(int x, int y)
	{
		logic_pos = Vector2(y, x);
		real_pos = Vector2(y * OBJECT_SIZE, x * OBJECT_SIZE);
		collision_box->set_enabled(true);
		collision_box->set_position(Vector2(real_pos.x+OBJECT_SIZE/2,real_pos.y+OBJECT_SIZE/2));
		collision_box->set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
		collision_box->set_layer_dst(CollisionLayer::Role);
		collision_box->set_layer_src(CollisionLayer::None);
		int a = rand() % 6+1;
		if (a > 3)
			a = 4;
		std::string image_name = "wall" + std::to_string(a);
		image.set_image(image_name);
		image.set_position(real_pos);
		image.set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
	}
	~Wall() {}
	void on_render(const Camera& camera) override
	{
		image.on_render(camera);
	}
};

class Money :public Object
{
public:
	Money() = default;
	Money(int x, int y)
	{
		logic_pos = Vector2(y, x);
		real_pos = Vector2(y * OBJECT_SIZE, x * OBJECT_SIZE);
		collision_box->set_enabled(true);
		collision_box->set_position(Vector2(real_pos.x + OBJECT_SIZE / 2, real_pos.y + OBJECT_SIZE / 2));
		collision_box->set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
		collision_box->set_layer_dst(CollisionLayer::None);
		collision_box->set_layer_src(CollisionLayer::OOO);
		collision_box->set_on_collide([&]() {
			collision_box->set_enabled(false);
			image.set_image("path");
			});
		image.set_image("money");
		image.set_position(real_pos);
		image.set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
	}
	void on_render(const Camera& camera) override
	{
		image.on_render(camera);
	}
};

class Start :public Object
{
public:
	Start() = default;
	Start(int x, int y)
	{
		logic_pos = Vector2(y, x);
		real_pos = Vector2(y * OBJECT_SIZE, x * OBJECT_SIZE);
		collision_box->set_enabled(true);
		collision_box->set_position(Vector2(real_pos.x + OBJECT_SIZE / 2, real_pos.y + OBJECT_SIZE / 2));
		collision_box->set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
		collision_box->set_layer_dst(CollisionLayer::Role);
		collision_box->set_layer_src(CollisionLayer::None);
		image.set_image("start");
		image.set_position(real_pos);
		image.set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
	}
	void on_render(const Camera& camera) override
	{
		image.on_render(camera);
	}
};

class Exit :public Object
{
	public:
	Exit() = default;
	Exit(int x, int y)
	{
		logic_pos = Vector2(y, x);
		real_pos = Vector2(y * OBJECT_SIZE, x * OBJECT_SIZE);
		collision_box->set_enabled(true);
		collision_box->set_position(Vector2(real_pos.x + OBJECT_SIZE / 2, real_pos.y + OBJECT_SIZE / 2));
		collision_box->set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
		collision_box->set_layer_dst(CollisionLayer::Role);
		collision_box->set_layer_src(CollisionLayer::None);
		image.set_image("exit");
		image.set_position(real_pos);
		image.set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
	}
	void on_render(const Camera& camera) override
	{
		image.on_render(camera);
	}
};

class Path :public Object
{
	public:
	Path() = default;
	Path(int x, int y)
	{
		logic_pos = Vector2(y, x);
		real_pos = Vector2(y * OBJECT_SIZE, x * OBJECT_SIZE);
		collision_box->set_enabled(false);
		image.set_image("path");
		image.set_position(real_pos);
		image.set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
	}
	void on_render(const Camera& camera) override
	{
		image.on_render(camera);
	}
};
class Locker :public Object
{
public:
	Locker() = default;
	Locker(int x, int y)
	{
		logic_pos = Vector2(y, x);
		real_pos = Vector2(y * OBJECT_SIZE, x * OBJECT_SIZE);
		collision_box->set_enabled(true);
		collision_box->set_position(Vector2(real_pos.x + OBJECT_SIZE / 2, real_pos.y + OBJECT_SIZE / 2));
		collision_box->set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
		collision_box->set_layer_dst(CollisionLayer::None);
		collision_box->set_layer_src(CollisionLayer::OOO);
		collision_box->set_on_collide([&]() {
			collision_box->set_enabled(false);
			image.set_image("path");
			});
		image.set_image("locker");
		image.set_position(real_pos);
		image.set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
	}
	void on_render(const Camera& camera) override
	{
		image.on_render(camera);
	}
};
class Boss :public Object
{
	public:
	Boss() = default;
	Boss(int x, int y)
	{
		logic_pos = Vector2(y, x);
		real_pos = Vector2(y * OBJECT_SIZE, x * OBJECT_SIZE);
		collision_box->set_enabled(true);
		collision_box->set_position(Vector2(real_pos.x + OBJECT_SIZE / 2, real_pos.y + OBJECT_SIZE / 2));
		collision_box->set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
		collision_box->set_layer_dst(CollisionLayer::None);
		collision_box->set_layer_src(CollisionLayer::OOO);
		collision_box->set_on_collide([&]() {
			collision_box->set_enabled(false);
			});
		image.set_image("path");
		image.set_position(real_pos);
		image.set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
		boss_idle_left.add_frame(ResourcesManager::instance()->find_image("boss_idle_left"), 6);
		boss_idle_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
		boss_idle_left.set_interval(150.0f);
		boss_idle_left.set_loop(true);
		boss_idle_left.set_position(Vector2(real_pos.x+OBJECT_SIZE/2,real_pos.y+OBJECT_SIZE));
	}
	void on_render(const Camera& camera) override
	{
		image.on_render(camera);
		boss_idle_left.on_render(camera);
	}
private:
	Animation boss_idle_left;
};

class Trap:public Object
{
	public:
	Trap() = default;
	Trap(int x, int y)
	{
		logic_pos = Vector2(y, x);
		real_pos = Vector2(y * OBJECT_SIZE, x * OBJECT_SIZE);
		collision_box->set_enabled(true);
		collision_box->set_position(Vector2(real_pos.x + OBJECT_SIZE / 2, real_pos.y + OBJECT_SIZE / 2));
		collision_box->set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
		collision_box->set_layer_dst(CollisionLayer::None);
		collision_box->set_layer_src(CollisionLayer::OOO);
		collision_box->set_on_collide([&]() {
			collision_box->set_enabled(false);
			image.set_image("path");
			});
		image.set_image("trap");
		image.set_position(real_pos);
		image.set_size(Vector2(OBJECT_SIZE, OBJECT_SIZE));
	}
	void on_render(const Camera& camera) override
	{
		image.on_render(camera);
	}
};

class Player
{
public:
	Player()
	{
		collision_box = CollisionManager::instance()->create_collision_box();
		collision_box->set_enabled(true);
		collision_box->set_size(Vector2(OBJECT_SIZE-30, OBJECT_SIZE-30));
		collision_box->set_layer_dst(CollisionLayer::OOO);
		collision_box->set_layer_src(CollisionLayer::Role);
        collision_box->set_on_collide([&]() {  
            real_pos = old_real_pos;  
            collision_box->set_position(Vector2(real_pos.x, real_pos.y - OBJECT_SIZE/2));
        });

		player_idle_left.add_frame(ResourcesManager::instance()->find_image("player_idle_left"),5);
		player_idle_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
		player_idle_left.set_interval(150.0f);
		player_idle_left.set_loop(true);
		player_idle_right.add_frame(ResourcesManager::instance()->find_image("player_idle_right"), 5);
		player_idle_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);	
		player_idle_right.set_interval(150.0f);
		player_idle_right.set_loop(true);
		player_run_left.add_frame(ResourcesManager::instance()->find_image("player_run_left"), 10);
		player_run_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
		player_run_left.set_interval(0.1f);
		player_run_left.set_loop(true);
		player_run_right.add_frame(ResourcesManager::instance()->find_image("player_run_right"), 10);
		player_run_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
		player_run_right.set_interval(0.1f);
		player_run_right.set_loop(true);
		current_animation = &player_idle_right;
	}
	const Vector2& get_position()
	{
		return real_pos;
	}

	void set_position(const Vector2& pos)
	{
		logic_pos = Vector2(pos.y,pos.x);
		real_pos = Vector2((pos.y +0.5)* OBJECT_SIZE, pos.x * OBJECT_SIZE+OBJECT_SIZE);
		old_real_pos = real_pos;
		collision_box->set_position(Vector2(real_pos.x, real_pos.y-OBJECT_SIZE/2));
	}
	~Player() {
		if (collision_box != nullptr)
		{
			CollisionManager::instance()->destroy_collision_box(collision_box);
			collision_box = nullptr;
		}
	}
	void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (msg.vkcode)
			{
				// 'A'
			case 0x41:
				is_left_key_down = true;
				break;
				// 'D'
			case 0x44:
				is_right_key_down = true;
				break;
				// 'W'
			case 0x57:
				is_up_key_down=true;
				break;
			case 0x53: // 'S'
				is_down_key_down = true;
				break;
			}
			break;
		case WM_KEYUP:
			switch (msg.vkcode)
			{
				// 'A'
			case 0x41:
				is_left_key_down = false;
				break;
				// 'D'
			case 0x44:
				is_right_key_down = false;
				break;
			case 0x57: // 'W'
				is_up_key_down = false;
				break;
			case 0x53: // 'S'
				is_down_key_down = false;
				break;
			}
			break;
		default:
			break;
		}
	}
	void on_update(int delta)
	{
		old_real_pos = real_pos;
		int xdirection = is_right_key_down - is_left_key_down;
		if (xdirection != 0)
		{
			is_facing_right = xdirection > 0;
			current_animation = is_facing_right ? &player_run_right : &player_run_left;
			real_pos.x += xdirection * 0.3f * delta;
		}
		int ydirection = is_down_key_down - is_up_key_down;
		if (ydirection != 0)
		{
			real_pos.y += ydirection * 0.3f * delta;
			current_animation = is_facing_right ? &player_run_right : &player_run_left;
		}
		if (xdirection == 0 && ydirection == 0)
		{
			current_animation = is_facing_right ? &player_idle_right : &player_idle_left;
		}
		current_animation->on_update(delta);
		collision_box->set_position(Vector2(real_pos.x, real_pos.y -OBJECT_SIZE/2));
		CollisionManager::instance()->process_collide();
	}
	void on_render(const Camera& camera)
	{
		if (current_animation != nullptr)
		{
			current_animation->set_position(real_pos);
			current_animation->on_render(camera);
		}
	}
private:
	CollisionBox* collision_box = nullptr;
	Vector2 old_real_pos;
	Vector2 real_pos;
	Vector2 logic_pos;
	Animation player_idle_left;
	Animation player_idle_right;
	Animation player_run_left;
	Animation player_run_right;
	Animation* current_animation = nullptr;

	bool is_left_key_down = false;
	bool is_right_key_down = false;
	bool is_up_key_down = false;
	bool is_down_key_down = false;

	bool is_facing_right = true;
};
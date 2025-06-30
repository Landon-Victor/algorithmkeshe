#pragma once
#include <string>
#include "collision_manager.h"
#include "collision_box.h"
#include "static_image.h"
#include "camera.h"
#include "animation.h"

const int OBJECT_SIZE = 90;
class GameScene;
class Object
{
public:
    Object();
    Object(GameScene* scene);
    virtual ~Object();
    virtual void on_render(const Camera& camera) = 0;
protected:
    Vector2 logic_pos;
    Vector2 real_pos;
    CollisionBox* collision_box = nullptr;
    StaticImage image;
    GameScene* game_scene = nullptr;
};

class Wall : public Object
{
public:
    Wall() = default;
    Wall(int x, int y, GameScene* scene);
    ~Wall();
    void on_render(const Camera& camera) override;
};

class Money : public Object
{
public:
    Money() = default;
    Money(int x, int y, GameScene* scene);
    void on_render(const Camera& camera) override;
};

class Start : public Object
{
public:
    Start() = default;
    Start(int x, int y, GameScene* scene);
    void on_render(const Camera& camera) override;
};

class Exit : public Object
{
public:
    Exit() = default;
    Exit(int x, int y, GameScene* scene);
    void on_render(const Camera& camera) override;
};

class Path : public Object
{
public:
    Path() = default;
    Path(int x, int y, GameScene* scene);
    void on_render(const Camera& camera) override;
};

class Locker : public Object
{
public:
    Locker() = default;
    Locker(int x, int y, GameScene* scene);
    void on_render(const Camera& camera) override;
};

class Boss : public Object
{
public:
    Boss() = default;
    Boss(int x, int y, GameScene* scene);
    void on_render(const Camera& camera) override;
private:
    Animation boss_idle_left;
};

class Trap : public Object
{
public:
    Trap() = default;
    Trap(int x, int y, GameScene* scene);
    void on_render(const Camera& camera) override;
};

class Player
{
public:
    Player();
    ~Player();
    const Vector2& get_real_pos();
	const Vector2& get_logic_pos();
    void set_position(const Vector2& pos, GameScene* g);
    void on_input(const ExMessage& msg);
    void on_update(int delta);
    void on_render(const Camera& camera);
    void clear_move()
    {
        is_left_key_down = false;
        is_right_key_down = false;
        is_up_key_down = false;
		is_down_key_down = false;
    }
    void change_auto_move()
    {
		is_auto_moving = is_auto_moving ? false : true;
	}
    void clear_auto_move()
    {
        is_auto_moving = false;
        auto_move_index = 1;
	}
    void set_path(const std::vector<Vector2>& p)
    {
        path = p;
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
    GameScene* game_scene = nullptr;
    std::vector<Vector2> path;
    bool is_left_key_down = false;
    bool is_right_key_down = false;
    bool is_up_key_down = false;
    bool is_down_key_down = false;
    bool is_facing_right = true;

	bool is_moving = false;
    bool is_auto_moving = false;
	int auto_move_index = 1;
};
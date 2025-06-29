#pragma once
#include "scene_manager.h"
#include "map_generator.h"
#include "static_image.h"
#include "maze_solver.h"
#include "object.h"
#include "button.h"

extern const int OBJECT_SIZE;

class GameScene : public Scene
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
    friend class DecodeScene;
public:
    GameScene();
    ~GameScene() = default;
    void on_enter() override;
    void on_exit() override;
    void on_render(const Camera& camera) override;
    void on_input(const ExMessage& message) override;
    void on_update(int delta) override;

private:
    void little_on_render();
    void on_render_right();
	void on_input_right(const ExMessage& message);
	void on_update_right(int delta);

    int size = 8;
    Map map;
    std::vector<Object*> objects;
    Player player;
    StaticImage dilao;
    StaticImage cover;
    MapGenerator map_generator;
    MazeSolver maze_solver;
    std::vector<Vector2> best_path;
    int cur_money = 0;
    Button automatic;
	Button path;
    bool is_path = false;
	bool is_skip_enter = false;
};
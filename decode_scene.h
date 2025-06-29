#pragma once
#include"scene_manager.h"
#include "password_solver.h"
#include "object.h"
#include "textbox.h"
#include "game_scene.h"

struct try_info
{
	std::string try_password;
	bool is_correct;
};
class DecodeScene :public Scene  
{  
public:  
    DecodeScene() : try_times(0) {}  
    DecodeScene(GameScene* g) : g(g), try_times(0) {}  
    ~DecodeScene() {}  
    void on_enter() override;  
    void on_exit() override;  
    void on_update(int delta) override;  
    void on_render(const Camera& camera) override;  
    void on_input(const ExMessage& msg) override;  
private:  
    void generate_question();  
private:  
    std::string question;  
    PasswordSolver solver;  
    std::vector<try_info> try_history;  
    int try_times;  
    Textbox textbox;  
    std::vector<std::vector<int>> clues;  
    GameScene* g;  
};
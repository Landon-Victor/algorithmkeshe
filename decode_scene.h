#pragma once
#include"scene_manager.h"
#include "PasswordSolver.h"
#include "object.h"

struct try_info
{
	std::string try_password;
	bool is_correct;
};
class DecodeScene :Scene
{
public:
	DecodeScene() = default;
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
	Player player;
};
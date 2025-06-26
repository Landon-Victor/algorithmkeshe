#pragma once
#include "scene_manager.h"
#include "attack_strategy.h"
#include "camera.h"
class BossScene : public Scene
{
public:
	void on_enter()override {
		attackStrategy.test();

	}
	void on_exit()override {}
	void on_input(const ExMessage& msg)override {}
	void on_update(int delta)override {}
	void on_render(const Camera& camera)override {}
private:
	AttackStrategy attackStrategy;
};
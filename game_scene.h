#pragma once
#include "scene_manager.h"

class GameScene :public Scene
{
public:
	GameScene() {}
	~GameScene() = default;
	void on_enter() {}
	void on_exit() {}
	void on_render(const Camera& camera)override
	{

	}
	void on_input(const ExMessage& message)override
	{

	}
	void on_update(int delta)override
	{

	}
};
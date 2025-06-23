#pragma once
#include<graphics.h>
#include "scene_manager.h"

class HelloScene :public Scene
{
public:
	HelloScene() = default;
	~HelloScene() = default;
	void on_enter() {}
	void on_exit() {}
	void on_update(int delta) {
		//Hello.on_update(delta);
	}
	void on_render(const Camera& camera) {
		//Hello.on_render(camera);
	}
	void on_input(const ExMessage& message) {
	}
};
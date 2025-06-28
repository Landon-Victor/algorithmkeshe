#pragma once
#include "scene_manager.h"
#include "button.h"
#include "test.h"

class HelloScene :public Scene
{
public:
	HelloScene()
	{
		startgame.set_pos(280, 240);
		startgame.set_size(240, 60);
		startgame.set_image("startgame");
		startgame.set_on_click([&]()
			{
				SceneManager::instance()->switch_to(SceneManager::SceneType::Game);

			});
		test.set_pos(280, 320);
		test.set_size(240, 60);
		test.set_image("startgame");	
		test.set_on_click([&]()
			{
				t.test_main();
			});
	}
	~HelloScene() = default;
	void on_enter() {}
	void on_exit() {}
	void on_update(int delta) {
		startgame.on_update(delta);
		test.on_update(delta);
	}
	void on_render(const Camera& camera) {
		startgame.on_render();
		test.on_render();
	}
	void on_input(const ExMessage& message) {
		startgame.on_input(message);
		test.on_input(message);
	}
private:
	Button startgame;
	Button test;
	Test t;
};
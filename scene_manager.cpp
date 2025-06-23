#include <graphics.h>
#include "scene_manager.h"
#include "hello_scene.h"
#include "game_scene.h"

SceneManager* SceneManager::manager = nullptr;

SceneManager* SceneManager::instance()
{
	if (!manager)
		manager = new SceneManager();

	return manager;
}

SceneManager::SceneManager()
{
	hello_scene = new HelloScene();
	game_scene = new GameScene();
	current_scene = hello_scene;
	current_scene->on_enter();
}

SceneManager::~SceneManager()
{
	delete login_scene;
	delete register_scene;
	delete hello_scene;
	delete game_scene;
}

void SceneManager::set_current_scene(Scene* scene)
{
	current_scene = scene;
	current_scene->on_enter();
}

void SceneManager::switch_to(SceneType type)
{
	current_scene->on_exit();
	switch (type)
	{
	case SceneType::Login:
		current_scene = login_scene;
		break;
	case SceneType::Hello:
		current_scene = hello_scene;
		break;
	case SceneType::Register:
		current_scene = register_scene;
		break;
	case SceneType::Game:
		current_scene = game_scene;
		break;
	default:
		break;
	}
	current_scene->on_enter();
}

void SceneManager::on_update(int delta)
{
	current_scene->on_update(delta);
}

void SceneManager::on_render(const Camera& camera)
{
	current_scene->on_render(camera);
}

void SceneManager::on_input(ExMessage& msg)
{
	current_scene->on_input(msg);
}
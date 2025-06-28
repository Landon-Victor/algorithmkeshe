#pragma once
#include <graphics.h>
#include <iostream>
#include "camera.h"

// 页面的抽象基类

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	// 进入页面
	virtual void on_enter() = 0;
	// 更新页面
	virtual void on_update(int delta) = 0;
	// 渲染页面
	virtual void on_render(const Camera& camera) = 0;
	// 处理输入
	virtual void on_input(const ExMessage& msg) = 0;
	// 退出页面
	virtual void on_exit() = 0;

private:

};
class SceneManager
{
public:
	enum class SceneType
	{
		Login,
		Register,
		Hello,
		Game,
		Boss
	};

	static SceneManager* instance();

public:
	SceneManager();
	~SceneManager();

	void set_current_scene(Scene* scene);

	void switch_to(SceneType type);

	void on_update(int delta);

	void on_render(const Camera& camera);

	void on_input(ExMessage& msg);


private:
	Scene* current_scene = nullptr;

	static SceneManager* manager;

	Scene* login_scene = nullptr;
	Scene* register_scene = nullptr;
	Scene* hello_scene = nullptr;
	Scene* game_scene = nullptr;
	Scene* boss_scene = nullptr;
};
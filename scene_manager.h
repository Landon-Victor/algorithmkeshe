#pragma once
#include <graphics.h>
#include <iostream>
#include "camera.h"

// ҳ��ĳ������

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	// ����ҳ��
	virtual void on_enter() = 0;
	// ����ҳ��
	virtual void on_update(int delta) = 0;
	// ��Ⱦҳ��
	virtual void on_render(const Camera& camera) = 0;
	// ��������
	virtual void on_input(const ExMessage& msg) = 0;
	// �˳�ҳ��
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
#pragma once

#include "resources_manager.h"
#include "util.h"

class StaticImage
{
public:
	StaticImage() = default;
	~StaticImage() = default;

	void on_render(const Camera& camera)
	{
		int start_x = camera.get_position().x;
		int start_y = camera.get_position().y;
		int end_x = start_x + camera.get_camera_width();
		int end_y = start_y + camera.get_camera_width();
		if (position.x < start_x || position.x+size.x > end_x || position.y < start_y || position.y+size.y > end_y)
			return; // Outside camera view, no need to render
		if (img)
		{
			Rect rect_dst = { (int)position.x,(int)position.y, (int)size.x,(int)size.y };
			putimage_alpha(&camera, img, &rect_dst);
		}
	}

	void on_render()
	{
		if (img)
		{
			Rect rect_dst = { (int)position.x,(int)position.y, (int)size.x,(int)size.y };
			putimage_alpha(img, &rect_dst);
		}
	}

	void set_image(const std::string& name)
	{
		img = ResourcesManager::instance()->find_image(name);
	}

	void set_position(Vector2 pos)
	{
		position = pos;
	}

	void set_size(Vector2 s)
	{
		size = s;
	}

private:
	Vector2 position;
	Vector2 size;
	IMAGE* img = nullptr;
};
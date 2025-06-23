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
		if (img)
		{
			Rect rect_dst = { (int)position.x,(int)position.y, (int)size.x,(int)size.y };
			putimage_alpha(&camera, img, &rect_dst);
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
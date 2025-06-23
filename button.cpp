#include "button.h"

#include <iostream>

Button::Button() : x(0), y(0), w(0), h(0)
{
	state = State::IDLE;
}

Button::~Button()
{
}

void Button::set_image(std::string button_name)
{
	std::string path = "button_" + button_name;
	std::cout << path + "_1" << std::endl;
	idle = *ResourcesManager::instance()->find_image((path + "_1"));
	hover = *ResourcesManager::instance()->find_image((path + "_2"));
	press = *ResourcesManager::instance()->find_image((path + "_3"));
}

void Button::on_render(const Camera& camera)
{
	Rect rect_dst = { x, y, w, h };

	switch (state)
	{
	case State::IDLE:
		putimage_alpha(&camera, &idle, &rect_dst);
		break;
	case State::HOVER:
		putimage_alpha(&camera, &hover, &rect_dst);
		break;
	case State::PRESS:
		putimage_alpha(&camera, &press, &rect_dst);
		break;
	}

}

void Button::on_input(const ExMessage& msg)
{
	switch (msg.message)
	{
	case WM_MOUSEMOVE:
		if (isInside(msg.x, msg.y))
			state = State::HOVER;
		else
			state = State::IDLE;
		break;
	case WM_LBUTTONDOWN:
		if (isInside(msg.x, msg.y))
			state = State::PRESS;
		break;

	case WM_LBUTTONUP:
		if (state == State::PRESS && isInside(msg.x, msg.y))
		{
			state = State::HOVER;
			play_audio(_T("ui_confirm"), ResourcesManager::instance()->get_volume(), false);
			if (on_click)
				on_click();
		}
		else
		{
			state = State::IDLE;
		}
		break;

	}
}

void Button::on_update(int delta)
{
}

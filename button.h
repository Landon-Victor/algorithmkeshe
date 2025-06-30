#pragma once
#include <graphics.h>
#include <string>
#include <functional>

#include "util.h"
#include "camera.h"
#include "resources_manager.h"

// 按钮类

class Button
{
public:
    // 按钮当前状态（无状态、鼠标放在按钮上、点击按钮）
    enum class State
    {
        IDLE,
        HOVER,
        PRESS
    };

public:
    Button();
    ~Button();

    // 设置图片
    void set_image(std::string button_name);

     void on_render(const Camera& camera);
     void on_render();

    virtual void on_input(const ExMessage& msg);

    virtual void on_update(int delta);

    // 设置点击后的回调函数
    void set_on_click(std::function<void()> on_click)
    {
        this->on_click = on_click;
    }

    void set_pos(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    void set_size(int w, int h)
    {
        this->w = w;
        this->h = h;
    }
	int get_x() const { return x; }
	int get_y() const { return y; }
	int get_w() const { return w; }
protected:
    int x, y;
    int w, h;
    IMAGE idle;
    IMAGE hover;
    IMAGE press;
    State state;

    // 点击后的回调函数
    std::function<void()> on_click;

    // 当前鼠标的坐标是否在按钮上
    bool isInside(int px, int py) const
    {
        return (px >= x && px <= x + w && py >= y && py <= y + h);
    }
};

class Button2 : public Button
{
public:
    Button2() {
        state = State::IDLE;
		timer.set_one_shot(false);
        timer.set_callback([this]() {
			state = State::IDLE;
			timer.pause();
			});
        timer.pause();
    };
    void set_timer_wait_time(int interval)
    {
		timer.set_wait_time(interval);
	}
    void on_input(const ExMessage& msg) override
    {
        if (state == State::PRESS)
            return;
        switch (msg.message)
        {
        case WM_MOUSEMOVE:
            if (isInside(msg.x, msg.y))
                state = State::HOVER;
            else
                state = State::IDLE;
            break;
        case WM_LBUTTONDOWN:
            if (isInside(msg.x, msg.y)) {
                state = State::PRESS;
                if (on_click) {
                    on_click();
                }
                timer.restart();
            }
            break;
        }
	}
    void on_update(int delta) override
    {
        if(is_clicked) {
            state = State::PRESS;
            if (on_click) {
                on_click();
            }
            timer.restart();
			is_clicked = false;
		}
		timer.on_update(delta);
    }
    void set_is_clicked(bool is_clicked) {
		this->is_clicked = is_clicked;
    }
private:
	Timer timer;
	bool is_clicked = false; //是否点击
};
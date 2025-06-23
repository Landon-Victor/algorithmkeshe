#pragma once
#include <graphics.h>
#include "button.h"
#include "camera.h"
#include "animation.h"

class Textbox
{
public:
	Textbox();
	~Textbox();
	virtual void Set_Textbox(int x1, int y1, int x2, int y2, int max)
	{
		maxlen = max;
		left = x1, top = y1, right = x2, bottom = y2;
		cursor_pos = { float(left),float(bottom - 5) };
		cursor_index = -1;
	}

	void set_callback(std::function<void()> callback)
	{
		on_change = callback;
	}

	std::wstring get_Textbox()
	{
		return text;
	}

	void Clear()
	{
		text.clear();
	}

	virtual void on_update(int delta);
	virtual void on_render(const Camera& Camera);
	virtual void on_input(const ExMessage& msg);
protected:
	int left = 0, top = 0, right = 0, bottom = 0;	// 控件坐标
	std::wstring text;				// 文本框内容
	size_t maxlen = 0;							    // 文本框最大内容长度
	Animation cursor;					            // 光标动画
	bool isActive = false;					        // 是否处于活动状态
	Vector2 cursor_pos;					            // 光标位置
	int  cursor_index;			                // 光标索引位置
	std::function<void()> on_change;	        // 文本回调函数
protected:
	bool isInside(int x, int y)
	{
		return (left <= x && x <= right && top <= y && y <= bottom);
	}
	void cursor_index_to_pos();
	void change_cursor_index(int);
};


class TextboxSecurity : public Textbox
{
public:
	TextboxSecurity() = default;
	TextboxSecurity(const TextboxSecurity&) = delete;
	TextboxSecurity& operator=(const TextboxSecurity&) = delete;
	~TextboxSecurity() = default;
	void on_render(const Camera& Camera);
};

class TextboxSwitch : public Textbox
{
public:
	TextboxSwitch() = default;
	TextboxSwitch(const TextboxSwitch&) = delete;
	TextboxSwitch& operator=(const TextboxSwitch&) = delete;
	~TextboxSwitch() = default;
	void on_render(const Camera& Camera);
	void set_text(const std::wstring& text)
	{
		this->text = text;
	}
};

class TextboxButton :public TextboxSwitch
{
public:
	TextboxButton();
	~TextboxButton() = default;
	void on_render(const Camera& Camera) override;
	void on_input(const ExMessage& msg) override;
	void on_update(int delta) override;
	void Set_Textbox(int x1, int y1, int x2, int y2, int max) override;
private:
	Button left;
	Button right;
};
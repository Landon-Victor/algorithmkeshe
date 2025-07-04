#pragma once

#include <graphics.h>
#include <windows.h>
#include <string>
#include "camera.h"
#include <mciapi.h>

struct Rect
{
	int x, y;
	int w, h;
};

inline void flip_image(IMAGE* src, IMAGE* dst)
{
	int w = src->getwidth();
	int h = src->getheight();
	Resize(dst, w, h);
	DWORD* src_buffer = GetImageBuffer(src);
	DWORD* dst_buffer = GetImageBuffer(dst);
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int idx_src = y * w + x;
			int idx_dst = y * w + (w - x - 1);
			dst_buffer[idx_dst] = src_buffer[idx_src];
		}
	}
}

#pragma comment(lib, "MSIMG32.LIB")
#pragma comment(lib,"WINMM.lib")
#pragma comment(lib,"MSIMG32.lib")

inline void putimage_alpha(const Camera* camera, IMAGE* img, const Rect* rect_dst, const Rect* rect_src = nullptr)
{
	int dst_x = rect_dst->x - (int)camera->get_position().x;
	int dst_y = rect_dst->y - (int)camera->get_position().y;

	static BLENDFUNCTION blend_func = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };
	AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, rect_dst->w, rect_dst->h,
		GetImageHDC(img), rect_src ? rect_src->x : 0, rect_src ? rect_src->y : 0,
		rect_src ? rect_src->w : img->getwidth(), rect_src ? rect_src->h : img->getheight(), blend_func);

}

inline void putimage_alpha(IMAGE* img, const Rect* rect_dst, const Rect* rect_src = nullptr)
{
	static BLENDFUNCTION blend_func = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };
	AlphaBlend(GetImageHDC(GetWorkingImage()), rect_dst->x, rect_dst->y, rect_dst->w, rect_dst->h,
		GetImageHDC(img), rect_src ? rect_src->x : 0, rect_src ? rect_src->y : 0,
		rect_src ? rect_src->w : img->getwidth(), rect_src ? rect_src->h : img->getheight(), blend_func);
}

inline void line(const Camera& camera, int x1, int y1, int x2, int y2)
{
	const Vector2& pos_camera = camera.get_position();
	line((x1 - (int)pos_camera.x), (y1 - (int)pos_camera.y), (x2 - (int)pos_camera.x), (y2 - (int)pos_camera.y));
}


inline void load_audio(LPCTSTR path, LPCTSTR id)
{
	static TCHAR str_cmd[512];
	_stprintf_s(str_cmd, _T("open %s alias %s"), path, id);
	mciSendString(str_cmd, NULL, 0, NULL);
}

inline void play_audio(LPCTSTR id, int volume, bool is_loop = false)
{
	static TCHAR str_cmd[512];
	_stprintf_s(str_cmd, _T("play %s %s from 0"), id, is_loop ? _T("repeat") : _T(""));
	mciSendString(str_cmd, NULL, 0, NULL);
	_stprintf_s(str_cmd, _T("setaudio \"%s\" volume to %d"), id, volume);
	mciSendString(str_cmd, NULL, 0, NULL);
}

inline void stop_audio(LPCTSTR id)
{
	static TCHAR str_cmd[512];
	_stprintf_s(str_cmd, _T("stop %s"), id);
	mciSendString(str_cmd, NULL, 0, NULL);
}

inline int range_random(int min_num, int max_num)
{
	return min_num + rand() % (max_num - min_num + 1);
}

inline std::wstring get_current_time()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	wchar_t buf[32];
	swprintf(buf, 32, L"%04d.%02d.%02d-%02d.%02d.%02d",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	return buf;
}

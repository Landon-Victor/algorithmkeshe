#include <graphics.h>

#include "util.h"
#include "atlas.h"
#include "scene_manager.h"
#include "resources_manager.h"
#include <thread>

#pragma comment(lib, "Winmm.lib")

void put_background()
{
	IMAGE* img = ResourcesManager::instance()->find_image("background");
	Rect rect_dst = { 0,0,880,640 };
	putimage_alpha(img, &rect_dst);
}

int main()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	HWND hwnd = initgraph(880, 640, EW_SHOWCONSOLE);
	SetWindowText(hwnd, _T("Hollow Katana"));

	try
	{
		ResourcesManager::instance()->load();
	}
	catch (const LPCTSTR id)
	{
		TCHAR err_msg[512];
		_stprintf_s(err_msg, _T("无法加载：%s"), id);
		MessageBox(hwnd, err_msg, _T("资源加载失败"), MB_OK | MB_ICONERROR);
		return -1;
	}

	ExMessage msg;
	const int FPS = 60;

	bool running = true;

	initgraph(880, 640);

	BeginBatchDraw();

	SceneManager* scene_manager = SceneManager::instance();

	while (running)
	{
		DWORD frame_start_time = GetTickCount();

		while (peekmessage(&msg))
		{
			scene_manager->on_input(msg);
		}

		static DWORD last_tick_time = GetTickCount();
		DWORD current_tick_time = GetTickCount();
		DWORD delta_tick = current_tick_time - last_tick_time;
		scene_manager->on_update(delta_tick);
		last_tick_time = current_tick_time;

		cleardevice();
		put_background();
		scene_manager->on_render(*ResourcesManager::instance()->get_camera());

		FlushBatchDraw();

		DWORD frame_end_time = GetTickCount();
		DWORD frame_delta_time = frame_end_time - frame_start_time;
		if (frame_delta_time < 1000 / FPS)
			Sleep(1000 / FPS - frame_delta_time);
	}
	EndBatchDraw();

	return 0;
}

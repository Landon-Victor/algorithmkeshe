#include "util.h"
#include "resources_manager.h"
#include <fstream>  
#include <sstream>

struct ImageResInfo
{
	std::string id;
	LPCTSTR path;
};

struct AtlasResInfo
{
	std::string id;
	LPCTSTR path;
	int num_frame = 0;
};

static const std::vector<ImageResInfo> image_info_list =
{
	{"background",                        _T(R"(resources\background.png)")},
	{"button_startgame_1",                _T(R"(resources\ui\button_startgame_1.png)")},
	{"button_startgame_2",                _T(R"(resources\ui\button_startgame_2.png)")},
	{"button_startgame_3",                _T(R"(resources\ui\button_startgame_3.png)")},
	{"player_idle_right",                 _T(R"(resources\player_idle_right.png)")},
	{"player_run_right",                  _T(R"(resources\player_run_right.png)")},
	{"wall",                              _T(R"(resources\wall.png)")},
	{"dilao",                             _T(R"(resources\dilao.png)")},
	{"cover",                             _T(R"(resources\cover.png)")},
	{"boss_idle_left",                    _T(R"(resources\boss_idle_left.png)")},
	{"wall1",                             _T(R"(resources\wall1.png)")},
	{"wall2",                             _T(R"(resources\wall2.png)")},
	{"wall3",                             _T(R"(resources\wall3.png)")},
	{"wall4",                             _T(R"(resources\wall4.png)")},
	{"start",  					          _T(R"(resources\start.png)")},
	{"exit",                              _T(R"(resources\exit.png)")},
	{"locker",						      _T(R"(resources\locker.png)")},
	{"path",                              _T(R"(resources\path.png)")},
	{"money",							  _T(R"(resources\money.png)")},
	{"trap",							  _T(R"(resources\trap.png)")},
	{"button_test_1",                _T(R"(resources\ui\button_test_1.png)")},
	{"button_test_2",                _T(R"(resources\ui\button_test_2.png)")},
	{"button_test_3",                _T(R"(resources\ui\button_test_3.png)")},
	{"button_auto_1",                _T(R"(resources\ui\button_auto_1.png)")},
	{"button_auto_2",                _T(R"(resources\ui\button_auto_2.png)")},
	{"button_auto_3",                _T(R"(resources\ui\button_auto_3.png)")},
	{"button_path_1",                _T(R"(resources\ui\button_path_1.png)")},
	{"button_path_2",                _T(R"(resources\ui\button_path_2.png)")},
	{"button_path_3",                _T(R"(resources\ui\button_path_3.png)")},
	{"button_backmenu_1",         _T(R"(resources\ui\button_backmenu_1.png)")},
	{"button_backmenu_2",         _T(R"(resources\ui\button_backmenu_2.png)")},
	{"button_backmenu_3",         _T(R"(resources\ui\button_backmenu_3.png)")},
	{"button_exit_1",                _T(R"(resources\ui\button_exit_1.png)")},
	{"button_exit_2",                _T(R"(resources\ui\button_exit_2.png)")},
	{"button_exit_3",                _T(R"(resources\ui\button_exit_3.png)")},
	{"cursor",							_T(R"(resources\cursor.png)")},
};

static inline bool check_image_valid(IMAGE* image)
{
	return GetImageBuffer(image);
}

void ResourcesManager::load()
{
	AddFontResourceEx(_T("resources/font.ttf"), FR_PRIVATE, NULL);

	for (const auto& info : image_info_list)
	{
		IMAGE* image = new IMAGE();
		loadimage(image, info.path);
		if (!check_image_valid(image))
			throw info.path;
		image_pool[info.id] = image;
	}
	flip_image("player_idle_right", "player_idle_left", 5);
	flip_image("player_run_right", "player_run_left", 10);
}

ResourcesManager* ResourcesManager::manager = nullptr;

ResourcesManager* ResourcesManager::instance()
{
	if (!manager)
		manager = new ResourcesManager();

	return manager;
}

ResourcesManager::ResourcesManager()
{
	main_camera = new Camera();
}

ResourcesManager::~ResourcesManager()
{
	delete main_camera;
}


Atlas* ResourcesManager::find_atlas(const std::string& id) const
{
	const auto& itor = atlas_pool.find(id);
	if (itor == atlas_pool.end())
		return nullptr;

	return itor->second;
}

IMAGE* ResourcesManager::find_image(const std::string& id) const
{
	const auto& itor = image_pool.find(id);
	if (itor == image_pool.end())
		return nullptr;

	return itor->second;
}

void ResourcesManager::flip_image(IMAGE* src_image, IMAGE* dst_image, int num_h)
{
	int w = src_image->getwidth();
	int h = src_image->getheight();
	int w_frame = w / num_h;
	Resize(dst_image, w, h);
	DWORD* src_buffer = GetImageBuffer(src_image);
	DWORD* dst_buffer = GetImageBuffer(dst_image);
	for (int i = 0; i < num_h; i++)
	{
		int x_left = i * w_frame;
		int x_right = (i + 1) * w_frame;
		for (int y = 0; y < h; y++)
		{
			for (int x = x_left; x < x_right; x++)
			{
				int idx_src = y * w + x;
				int idx_dst = y * w + x_right - (x - x_left);
				dst_buffer[idx_dst] = src_buffer[idx_src];
			}
		}
	}

}

void ResourcesManager::flip_image(const std::string& src_id, const std::string dst_id, int num_h)
{
	IMAGE* src_image = image_pool[src_id];
	IMAGE* dst_image = new IMAGE();

	flip_image(src_image, dst_image, num_h);

	image_pool[dst_id] = dst_image;
}

void ResourcesManager::flip_atlas(const std::string& src_id, const std::string dst_id)
{
	Atlas* src_atlas = atlas_pool[src_id];
	Atlas* dst_atlas = new Atlas();

	for (int i = 0; i < src_atlas->get_size(); i++)
	{
		IMAGE img_flipped;
		flip_image(src_atlas->get_image(i), &img_flipped);
		dst_atlas->add_image(img_flipped);
	}

	atlas_pool[dst_id] = dst_atlas;
}
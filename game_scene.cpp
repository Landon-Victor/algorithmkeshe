#include "game_scene.h"
#include <algorithm>
#include <graphics.h>

GameScene::GameScene() {}

void GameScene::on_enter() {
    if (is_skip_enter)
    {
        is_skip_enter = false;
        return;
    }
	automatic.set_image("auto");
	automatic.set_pos(640, 500);
	automatic.set_size(240, 60);
    automatic.set_on_click([this]() {
        switch (current_scene) {
        case 0:
            player.change_auto_move();
            break;
        case 1:
			is_auto_decode = is_auto_decode ? false : true;
            break;
        case 2:
			is_auto_attack = is_auto_attack ? false : true;
            break;
		default:
            break;
        }
		});
	path.set_image("path");
	path.set_pos(640, 580);
	path.set_size(240, 60);
    path.set_on_click([this]() {
        is_path = is_path ? false : true;
		});
    dilao.set_position(Vector2(0, 0));
    dilao.set_size(Vector2(640, 640));
    dilao.set_image("dilao");
    map = map_generator.get_a_map(size);
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            switch (map[i][j]) {
            case Content::start:
                player.set_position(Vector2(i, j), this);
                break;
            case Content::wall:
                objects.push_back(new Wall(i, j, this));
                break;
            case Content::money:
                objects.push_back(new Money(i, j, this));
                break;
            case Content::trap:
                objects.push_back(new Trap(i, j, this));
                break;
            case Content::boss:
                objects.push_back(new Boss(i, j, this));
                break;
            case Content::locker:
                objects.push_back(new Locker(i, j, this));
                break;
            case Content::none:
                objects.push_back(new Path(i, j, this));
                break;
            case Content::exit:
                objects.push_back(new Exit(i, j, this));
                break;
            default:
                break;
            }
        }
    }
    cover.set_image("cover");
    cover.set_position(Vector2(640, 0));
    cover.set_size(Vector2(240, 640));
    maze_solver.init();
    maze_solver.solve();
    best_path = maze_solver.getPath();
	player.set_path(best_path);
}

void GameScene::on_exit() {
    player.clear_move();
}

void GameScene::on_render(const Camera& camera) {
    if (map_generator.get_is_show()) {
        map_generator.on_render();
        return;
    }
    dilao.on_render();
    for (auto& it : objects) {
        it->on_render(camera);
    }
    player.on_render(camera);
    // 边框
    int x1 = 0, y1 = 0;
    int x2 = 640, y2 = 640;
    setlinecolor(RGB(255, 255, 255));
    line(x1, y1, x2, y1);
    line(x1, y2, x2, y2);
    line(x1, y1, x1, y2);
    line(x2, y1, x2, y2);
    cover.on_render();
    on_render_right();
}

void GameScene::on_input(const ExMessage& message) {
    player.on_input(message);
    on_input_right(message);
}

void GameScene::on_update(int delta) {
    map_generator.on_update(delta);
    player.on_update(delta);
    Camera* camera = ResourcesManager::instance()->get_camera();
    int CameraWidth = camera->get_camera_width();
    if (camera == nullptr) return;
    Vector2 pos = player.get_real_pos() - Vector2(320, 320);
    if (pos.x < 0) pos.x = 0;
    if (pos.y < 0) pos.y = 0;
    if (pos.x + CameraWidth > (2 * size + 1) * OBJECT_SIZE)
        pos.x = (2 * size + 1) * OBJECT_SIZE - CameraWidth;
    if (pos.y + CameraWidth > (2 * size + 1) * OBJECT_SIZE)
        pos.y = (2 * size + 1) * OBJECT_SIZE - CameraWidth;
    camera->set_position(pos);
	on_update_right(delta);
}

void GameScene::little_on_render() {
    // 小地图参数
    const int mini_map_size = 200; // 小地图区域大小
    const int offset_x = 640;      // 右上角起始x（cover区域内）
    const int offset_y = 20;       // 右上角起始y
    int rows = map.size();
    int cols = map[0].size();

    // 偶数行/列为墙，奇数为格子
    int cell_w = mini_map_size / size;
    int cell_h = mini_map_size / size;
    int wall_w = 2;
    int wall_h = 2;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int x = offset_x, y = offset_y, w = cell_w, h = cell_h;
            if (j % 2 == 0) {
                x += (j / 2) * cell_w + (j / 2) * wall_w;
                w = wall_w;
            }
            else {
                x += (j / 2) * cell_w + ((j + 1) / 2) * wall_w;
                w = cell_w;
            }
            if (i % 2 == 0) {
                y += (i / 2) * cell_h + (i / 2) * wall_h;
                h = wall_h;
            }
            else {
                y += (i / 2) * cell_h + ((i + 1) / 2) * wall_h;
                h = cell_h;
            }

            if (i % 2 == 1 && j % 2 == 1) {
                switch (map[i][j]) {
                case Content::money:
                    setfillcolor(RGB(255, 165, 0));
                    solidrectangle(x + 1, y + 1, x + w - 1, y + h - 1);
                    break;
                case Content::locker:
                    setfillcolor(RGB(0, 128, 255));
                    solidrectangle(x + 1, y + 1, x + w - 1, y + h - 1);
                    break;
                case Content::trap:
                    setfillcolor(RGB(255, 0, 0));
                    solidrectangle(x + 1, y + 1, x + w - 1, y + h - 1);
                    break;
                case Content::start:
                case Content::exit:
                    setfillcolor(RGB(0, 255, 0));
                    solidrectangle(x + 1, y + 1, x + w - 1, y + h - 1);
                    break;
                default:
                    break;
                }
                if (is_path) {
                    Vector2 temp(i, j);
                    if (std::find(best_path.begin(), best_path.end(), temp) != best_path.end()) {
                        int dot_radius = cell_w / 4;
                        int cx = x + w / 2;
                        int cy = y + h / 2;
                        setfillcolor(RGB(255, 255, 0)); // 黄色
                        setlinecolor(RGB(255, 255, 0));
                        fillcircle(cx, cy, dot_radius);
                    }
                }
                if (j == player.get_logic_pos().x && i == player.get_logic_pos().y) {
                    int dot_radius = cell_w / 6;
                    int cx = x + w / 2;
                    int cy = y + h / 2;
                    setfillcolor(RGB(255, 105, 180)); // 紫色
                    setlinecolor(RGB(255,0,0));
                    fillcircle(cx, cy, dot_radius);
                }
            }
            else if (map[i][j] == Content::wall) {
                setfillcolor(WHITE);
                solidrectangle(x, y, x + w, y + h);
            }
        }
    }
    if (is_path)
    {
        // 绘制最优路径
        setlinecolor(RGB(255, 255, 0)); // 黄色
        setlinestyle(PS_SOLID, 2);
        for (size_t k = 0; k < best_path.size() - 1; ++k) {
            Vector2 start = best_path[k];
            Vector2 end = best_path[k + 1];
            int start_x = offset_x + ((int(start.y) / 2) * cell_w + (int(start.y) / 2) * wall_w) + cell_w / 2;
            int start_y = offset_y + ((int(start.x) / 2) * cell_h + (int(start.x) / 2) * wall_h) + cell_h / 2;
            int end_x = offset_x + ((int(end.y) / 2) * cell_w + (int(end.y) / 2) * wall_w) + cell_w / 2;
            int end_y = offset_y + ((int(end.x) / 2) * cell_h + (int(end.x) / 2) * wall_h) + cell_h / 2;
            line(start_x, start_y, end_x, end_y);
        }
    }
    // 绘制两列图例
    int legend_x1 = 660; // 第一列x
    int legend_x2 = 740; // 第二列x
    int legend_y = 250;  // 起始y
    int rect_w = 18, rect_h = 18;
    int gap_y = 8;
    int text_offset = 24;

    // 图例内容（名称与颜色）
    struct LegendItem {
        COLORREF color;
        const TCHAR* name;
        bool is_circle; // 是否用圆形表示
    } legend_items[] = {
        { RGB(255, 165, 0), _T("金币"), false },
        { RGB(255, 0, 0),   _T("陷阱"), false },
        { RGB(0, 128, 255), _T("机关"), false },
        { RGB(0, 255, 0),   _T("终点"), false },
        { WHITE,            _T("墙体"), false },
        { RGB(255, 255, 0), _T("最优路径"), true },
        { RGB(255, 105, 180), _T("玩家"), true }
    };

    int legend_count = sizeof(legend_items) / sizeof(legend_items[0]);
    int col_count = 2;
    int row_count = (legend_count + 1) / 2;

    for (int i = 0; i < legend_count; ++i) {
        int col = i / row_count;
        int row = i % row_count;
        int lx = (col == 0) ? legend_x1 : legend_x2;
        int ly = legend_y + row * (rect_h + gap_y);

        setfillcolor(legend_items[i].color);
        if (legend_items[i].is_circle) {
            fillcircle(lx + rect_w / 2, ly + rect_h / 2, rect_w / 2 - 2);
        }
        else {
            solidrectangle(lx, ly, lx + rect_w, ly + rect_h);
        }
        outtextxy(lx + text_offset, ly, legend_items[i].name);
    }
}

void GameScene::on_render_right() {
    little_on_render();
    int info_x = 660;
    int info_y = 350; 
    TCHAR buf[64];
    _stprintf_s(buf, _T("当前金币：%d"), cur_money);

    LOGFONT oldStyle;
    int oldBkMode = getbkmode();
    COLORREF oldTextColor = gettextcolor();
    gettextstyle(&oldStyle);

    // 设置新样式
    settextstyle(32, 0, _T("微软雅黑"));
    setbkmode(TRANSPARENT);
    settextcolor(RGB(255, 215, 0));

    // 绘制内容
    outtextxy(660, 350, buf);

    // 恢复原样式
    settextstyle(&oldStyle);
    setbkmode(oldBkMode);
    settextcolor(oldTextColor);
    path.on_render();
	automatic.on_render();
}

void GameScene::on_update_right(int delta) {
	path.on_update(delta);
	automatic.on_update(delta);
}
void GameScene::on_input_right(const ExMessage& message) {
    path.on_input(message);
    automatic.on_input(message);
}

void load()
{

}
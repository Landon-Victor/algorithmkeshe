#pragma once
#include "scene_manager.h"
#include "attack_strategy.h"
#include "camera.h"
#include <graphics.h>
class BossScene : public Scene
{
public:
	BossScene() = default;
	BossScene(GameScene* gameScene) : game_scene(gameScene) {}
	void on_enter()override {
		clear();
		game_scene->player.clear_move();
		game_scene->current_scene = 2;
		int a = rand() % 3+1;
		std::string filename =  "boss_"+to_string(a) +".json";
		load(filename);
		attack1.set_image("attack1");
		attack1.set_on_click([this]() {
			lose_boss_blood(attacks[0].hurt);
			next = true;
			cur_attacks_cd[0] = attacks[0].cd+1;
			attack_history.push_back(1);
			});
		attack1.set_pos(10, 500);
		attack1.set_size(240, 60);
		attack1.set_timer_wait_time(attacks[0].cd+1);
		attack2.set_image("attack2");
		attack2.set_on_click([this]() {
			lose_boss_blood(attacks[1].hurt);
			next = true;
			cur_attacks_cd[1] = attacks[1].cd + 1;
			attack_history.push_back(2);
			});
		attack2.set_pos(260, 500);
		attack2.set_size(240, 60);
		attack2.set_timer_wait_time(attacks[1].cd+1);
		attack3.set_image("attack3");
		attack3.set_on_click([this]() {
			lose_boss_blood(attacks[2].hurt);
			next = true;
			cur_attacks_cd[2] = attacks[2].cd + 1;
			attack_history.push_back(3);
			});
		attack3.set_pos(10, 570);
		attack3.set_size(240, 60);
		attack3.set_timer_wait_time(attacks[2].cd+1);
		attack4.set_image("attack4");
		attack4.set_on_click([this]() {
			lose_boss_blood(attacks[3].hurt);
			next = true;
			cur_attacks_cd[3] = attacks[3].cd + 1;
			attack_history.push_back(4);
			});
		attack4.set_pos(260, 570);
		attack4.set_size(240, 60);
		attack4.set_timer_wait_time(attacks[3].cd+1);
		boss_idle_right.add_frame(ResourcesManager::instance()->find_image("boss_idle_right"),6);
		boss_idle_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
		boss_idle_right.set_interval(150.0f);
		boss_idle_right.set_loop(true);
		boss_idle_right.set_position(Vector2(150,450));
		player_idle_left.add_frame(ResourcesManager::instance()->find_image("player_idle_left"), 5);
		player_idle_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
		player_idle_left.set_interval(150.0f);
		player_idle_left.set_loop(true);
		player_idle_left.set_position(Vector2(350, 450));
		blood.set_image("blood");
		blood.set_position(Vector2(10, 10));
		blood.set_size(Vector2(274, 250));
		attack_history.clear();
		cur_bosses_blood = bosses_blood;
		cur_attacks_cd.resize(attacks.size(), 0);
		attack_strategy.test(filename);
		timer.pause();
		timer.set_one_shot(false);
		timer.set_wait_time(1000);
		timer.set_callback([this]() {
			switch (attack_strategy.best_attack_sequence[cur_auto_index]+1) {
				case 1:
					attack1.set_is_clicked(1);
					break;
				case 2:
					attack2.set_is_clicked(1);
					break;
				case 3:
					attack3.set_is_clicked(1);
					break;
				case 4:
					attack4.set_is_clicked(1);
					break;
				default:
					break;
			}
			cur_auto_index++;
			});
	}
	void on_exit()override {}
	void on_input(const ExMessage& msg)override {
		attack1.on_input(msg);
		attack2.on_input(msg);
		attack3.on_input(msg);
		attack4.on_input(msg);
		game_scene->on_input_right(msg);
	}
	void on_update(int delta)override {
		if (is_return)
		{
			game_scene->current_scene = 0;
			SceneManager::instance()->switch_to(SceneManager::SceneType::Game);
			return;
		}
		if (game_scene->is_auto_attack)
		{
			timer.resume();
			timer.on_update(delta);
		}
		attack1.on_update(next);
		attack2.on_update(next);
		attack3.on_update(next);
		attack4.on_update(next);
		if (next) {
			game_scene->cur_money--;
			next = false;
			cur_n++;
			for (auto& cd : cur_attacks_cd) {
				if (cd == 0)continue;
				cd -=1;
			}
		}
		game_scene->on_update_right(delta);
		boss_idle_right.on_update(delta);
		player_idle_left.on_update(delta);
	}
	void on_render(const Camera& camera)override {
		attack1.on_render();
		attack2.on_render();
		attack3.on_render();
		attack4.on_render();
		game_scene->on_render_right();
		boss_idle_right.on_render();
		player_idle_left.on_render();
		blood.on_render();
		const int bar_count = 3;
		const int bar_x = 93; 
		const int bar_width = 172;
		const int bar_height = 24;
		const int bar_y[3] = { 40, 125, 211 };

		LOGFONT oldStyle;
		int oldBkMode = getbkmode();
		COLORREF oldTextColor = gettextcolor();
		gettextstyle(&oldStyle);

		for (int i = 0; i < bar_count; ++i) {
			int max_blood = bosses_blood[i];
			int cur_blood = cur_bosses_blood[i];
			float percent = (float)cur_blood / max_blood;
			int fill_width = (int)(bar_width * percent);

			setfillcolor(RGB(220, 40, 40));
			solidrectangle(bar_x, bar_y[i], bar_x + fill_width, bar_y[i] + bar_height);

			setbkmode(TRANSPARENT);
			settextcolor(WHITE);
			wchar_t buf[32];
			swprintf(buf, 32, L"%d / %d", cur_blood, max_blood);
			settextstyle(20, 0, _T("微软雅黑"));
			outtextxy(bar_x + bar_width + 20, bar_y[i] + (bar_height - 16) / 2, buf);
		}

		// 技能历史
		int history_y = 470;
		int history_x = 10;
		settextcolor(RGB(255, 255, 255));
		setbkmode(TRANSPARENT);
		settextstyle(20, 0, _T("宋体"));
		std::wstring history_str = L"技能历史：";
		for (int i =0; i < attack_history.size(); ++i) {
				history_str += std::to_wstring(attack_history[i] ) + L" ";
		}
		outtextxy(history_x, history_y, history_str.c_str());
		
		// 按钮技能信息渲染
		struct BtnInfo {
			Button2* btn;
			int idx;
		} btns[4] = { {&attack1,0},{&attack2,1},{&attack3,2},{&attack4,3} };

		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		settextstyle(18, 0, _T("微软雅黑"));
		for (const auto& b : btns) {
			int x = b.btn->get_x();
			int y = b.btn->get_y();
			int w = b.btn->get_w();
			// 第一行：伤害和cd
			wchar_t line1[64];
			swprintf(line1, 64, L"伤害 %d  CD %d", attacks[b.idx].hurt, attacks[b.idx].cd);
			outtextxy(x + 150, y + 8, line1);
			// 第二行：剩余cd
			int left_cd = cur_attacks_cd[b.idx];
			wchar_t line2[32];
			swprintf(line2, 32, L"剩余CD %d", left_cd);
			outtextxy(x + 150, y + 32, line2);
		}
		settextcolor(RGB(255, 255, 255));
		settextstyle(28, 0, _T("微软雅黑"));
		wchar_t round_buf[32];
		swprintf(round_buf, 32, L"当前轮次：%d", cur_n +1);
		outtextxy(400, 20, round_buf);
		settextstyle(&oldStyle);
		setbkmode(oldBkMode);
		settextcolor(oldTextColor);
		

	}
private:
	AttackStrategy attack_strategy;
	GameScene* game_scene;
	int cur_n = 0;//当前轮次
	Button2 attack1;//一技能
	Button2 attack2;//二技能
	Button2 attack3;//三技能
	Button2 attack4;//四技能
	std::vector<int> bosses_blood;// boss群血量
	std::map<int, Attack> attacks;//我的技能
	Animation boss_idle_right;
	Animation player_idle_left;
	std::vector<int> attack_history;//技能历史
	StaticImage blood;//血条
	std::vector<int> cur_bosses_blood;//当前boss血量
	std::vector<int> cur_attacks_cd;//当前技能冷却时间
	bool next = false;//是否进入下一轮
	Timer timer;
	int cur_auto_index = 0; // 当前自动攻击索引
	bool is_return = false; // 是否返回
private:
	void clear()
	{
		cur_auto_index = 0;
		next = false;
		cur_n = 0;
		attack_history.clear();
		game_scene->is_auto_attack = false;
		attack1.on_update(1000);
		attack2.on_update(1000);
		attack3.on_update(1000);
		attack4.on_update(1000);
		is_return = false;
	}
	void load(const std::string& filename) {
		std::ifstream ifs(filename, std::ios::binary);
		if (!ifs.is_open()) return;

		Json::Value root;
		Json::Reader reader;
		if (!reader.parse(ifs, root)) return;

		// 读取boss血量
		bosses_blood.clear();
		const Json::Value& arr_boss = root["B"];
		for (Json::Value::ArrayIndex i = 0; i < arr_boss.size(); ++i) {
			bosses_blood.push_back(arr_boss[i].asInt());
		}

		// 读取技能
		attacks.clear();
		const Json::Value& arr_skills = root["PlayerSkills"];
		for (Json::Value::ArrayIndex i = 0; i < arr_skills.size(); ++i) {
			if (arr_skills[i].isArray() && arr_skills[i].size() == 2) {
				Attack atk;
				atk.hurt = arr_skills[i][0].asInt();
				atk.cd = arr_skills[i][1].asInt();
				attacks[(int)i] = atk;
			}
		}
	}
	void lose_boss_blood(int hurt) {
		for(int i = 0; i < cur_bosses_blood.size(); ++i) {
			if (cur_bosses_blood[i] > 0) {
				cur_bosses_blood[i] -= hurt;
				if (cur_bosses_blood[i] <= 0) {
					cur_bosses_blood[i] = 0;
					if (i == cur_bosses_blood.size() - 1) 
						is_return = true;
				}
				return;
			}
		}
	}
};
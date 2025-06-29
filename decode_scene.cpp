#include "decode_scene.h"
#include "textbox.h"
#include <graphics.h>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>

void DecodeScene::generate_question() {
    solver.load();
	clues = solver.getClues();
    question= solver.getQuestion();
}

void DecodeScene::on_enter() {
    try_history.clear();
    try_times = 0;
    generate_question();
    textbox.Set_Textbox(200, 260, 440, 300, 4);
    textbox.Clear();
}

void DecodeScene::on_exit() {}

void DecodeScene::on_update(int delta) {
    textbox.on_update(delta);
	g->on_update_right(delta);
}

void DecodeScene::on_input(const ExMessage& msg) {
    textbox.on_input(msg);
    if (msg.message == WM_KEYDOWN && msg.ch == '\r') { // 回车提交
        std::wstring ws = textbox.get_Textbox();
        std::string guess(ws.begin(), ws.end());
        if (guess.length() == 3) {
            bool correct = solver.verifyPassword(guess);
            if (correct) {

            }
            else {
                g->cur_money--;
            }
            try_info info{ guess, correct };
            try_history.push_back(info);
            try_times++;
            textbox.Clear();
        }
    }
    g->on_input_right(msg);
}

void DecodeScene::on_render(const Camera& camera) {
    setbkmode(TRANSPARENT);

    // 背景色块（左侧区域）
    setfillcolor(RGB(245, 245, 250));
    solidrectangle(40, 30, 600, 600);

    // 标题
    settextcolor(RGB(60, 60, 120));
    settextstyle(36, 0, _T("微软雅黑"));
    outtextxy(220, 50, _T("密码解密"));

    // 分割线
    setlinecolor(RGB(180, 180, 200));
    line(60, 100, 580, 100);

    // 题目（哈希值）
    settextcolor(RGB(80, 80, 80));
    settextstyle(18, 0, _T("Consolas"));
    std::wstring w_hash(question.begin(), question.end());
    outtextxy(60, 110, _T("题目（哈希）："));
    settextcolor(RGB(120, 0, 180));
    outtextxy(70, 130, w_hash.c_str());

    // 线索区
    settextcolor(RGB(80, 80, 80));
    settextstyle(20, 0, _T("微软雅黑"));
    outtextxy(60, 150, _T("线索："));
    int clue_y = 180;
    for (const auto& clue : clues) {
        std::wstring clue_str;
        for (size_t i = 0; i < clue.size(); ++i) {
            clue_str += std::to_wstring(clue[i]);
            if (i + 1 < clue.size()) clue_str += L",";
        }
        settextcolor(RGB(0, 128, 192));
        outtextxy(80, clue_y, clue_str.c_str());
        clue_y += 24;
    }

    // 再次分割线
    setlinecolor(RGB(180, 180, 200));
    line(60, clue_y + 8, 580, clue_y + 8);

    // 文本框区
    settextcolor(RGB(60, 60, 120));
    settextstyle(20, 0, _T("微软雅黑"));
    outtextxy(60, clue_y + 20, _T("请输入三位数字密码："));
    textbox.on_render(camera);

    // 历史猜测区
    int y = clue_y + 70;
    settextcolor(RGB(80, 80, 80));
    settextstyle(18, 0, _T("微软雅黑"));
    outtextxy(60, y, _T("历史猜测："));
    y += 28;
    for (const auto& info : try_history) {
        TCHAR buf[64];
        if (info.is_correct) {
            settextcolor(RGB(0, 180, 0));
            _stprintf_s(buf, _T("%hs  正确"), info.try_password.c_str());
        }
        else {
            settextcolor(RGB(200, 0, 0));
            _stprintf_s(buf, _T("%hs  错误"), info.try_password.c_str());
        }
        outtextxy(80, y, buf);
        y += 24;
    }

    settextstyle(16, 0, _T("宋体"));
	g->on_render_right();
}

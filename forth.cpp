#include <graphics.h>
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <conio.h>
#include <windows.h>
using namespace std;

// 按钮结构体
struct Button {
    int x, y, width, height;
    wstring text;
    bool isPressed;
    COLORREF color;
    string condition; // 用于逻辑判断的英文条件名

    Button(int _x, int _y, int _w, int _h, wstring _text, string _condition, COLORREF _color = LIGHTGRAY)
        : x(_x), y(_y), width(_w), height(_h), text(_text), condition(_condition), isPressed(false), color(_color) {}

    bool isClicked(int mx, int my) {
        return mx >= x && mx <= x + width && my >= y && my <= y + height;
    }

    void draw() {
        setfillcolor(isPressed ? DARKGRAY : color);
        setlinecolor(BLACK);
        fillrectangle(x, y, x + width, y + height);
        rectangle(x, y, x + width, y + height);

        settextcolor(BLACK);
        setbkmode(TRANSPARENT);
        settextstyle(16, 0, _T("微软雅黑"));

        // 计算文本居中位置
        int textWidth = textwidth(text.c_str());
        int textHeight = textheight(text.c_str());
        int textX = x + (width - textWidth) / 2;
        int textY = y + (height - textHeight) / 2;

        outtextxy(textX, textY, text.c_str());
    }
};

// 检查0-9范围内的素数
bool isPrimeDigit(int digit) {
    return digit == 2 || digit == 3 || digit == 5 || digit == 7;
}

// 检查部分解是否满足所有条件
bool satisfiesPartial(const vector<int>& password, const vector<string>& conditions) {
    for (const string& cond : conditions) {
        if (cond == "each prime") {
            for (int d : password) {
                if (!isPrimeDigit(d)) {
                    return false;
                }
            }
        }
        else if (cond == "distinct") {
            set<int> s(password.begin(), password.end());
            if (s.size() != password.size()) {
                return false;
            }
        }
        else if (cond == "first even") {
            if (password.size() >= 1 && (password[0] % 2 != 0)) {
                return false;
            }
        }
        else if (cond == "first odd") {
            if (password.size() >= 1 && (password[0] % 2 == 0)) {
                return false;
            }
        }
        else if (cond == "second even") {
            if (password.size() >= 2 && (password[1] % 2 != 0)) {
                return false;
            }
        }
        else if (cond == "second odd") {
            if (password.size() >= 2 && (password[1] % 2 == 0)) {
                return false;
            }
        }
        else if (cond == "third even") {
            if (password.size() >= 3 && (password[2] % 2 != 0)) {
                return false;
            }
        }
        else if (cond == "third odd") {
            if (password.size() >= 3 && (password[2] % 2 == 0)) {
                return false;
            }
        }
    }
    return true;
}

// 回溯函数
void backtrack(vector<int>& current, int pos, const vector<string>& conditions, vector<vector<int>>& results) {
    if (pos == 3) {
        if (satisfiesPartial(current, conditions)) {
            results.push_back(current);
        }
        return;
    }

    for (int digit = 0; digit <= 9; ++digit) {
        current.push_back(digit);
        if (satisfiesPartial(current, conditions)) {
            backtrack(current, pos + 1, conditions, results);
        }
        current.pop_back();
    }
}

// 绘制标题
void drawTitle() {
    settextcolor(WHITE);
    settextstyle(32, 0, _T("微软雅黑"));
    setbkmode(TRANSPARENT);
    wstring title = L"密码解密游戏";
    int titleWidth = textwidth(title.c_str());
    outtextxy((800 - titleWidth) / 2, 20, title.c_str());
}

// 绘制说明文字
void drawInstructions() {
    settextcolor(YELLOW);
    settextstyle(16, 0, _T("微软雅黑"));
    setbkmode(TRANSPARENT);

    outtextxy(50, 80, L"点击下方按钮选择密码条件：");
    outtextxy(50, 100, L"绿色按钮表示已选择的条件");
}

// 绘制结果区域
void drawResults(const vector<vector<int>>& results, bool needUpdate = false) {
    static vector<vector<int>> lastResults;

    // 只有在结果发生变化时才重绘
    if (!needUpdate && results == lastResults) {
        return;
    }

    lastResults = results;

    // 清除结果区域
    setfillcolor(BLACK);
    fillrectangle(400, 150, 750, 590);

    settextcolor(WHITE);
    settextstyle(20, 0, _T("微软雅黑"));
    setbkmode(TRANSPARENT);
    outtextxy(400, 150, L"找到的密码：");

    if (results.empty()) {
        settextcolor(RED);
        outtextxy(400, 180, L"未找到满足条件的密码");
    }
    else {
        settextcolor(LIGHTGREEN);
        settextstyle(24, 0, _T("微软雅黑"));

        int y = 180;
        for (size_t i = 0; i < results.size() && i < 15; ++i) {
            wstring password = L"";
            for (int digit : results[i]) {
                password += to_wstring(digit);
            }
            outtextxy(400, y, password.c_str());
            y += 30;

            if (i == 14 && results.size() > 15) {
                settextcolor(YELLOW);
                settextstyle(16, 0, _T("微软雅黑"));
                wstring more = L"... 还有 " + to_wstring(results.size() - 15) + L" 个结果";
                outtextxy(400, y, more.c_str());
                break;
            }
        }
    }
}

// 绘制当前选择的条件
void drawSelectedConditions(const vector<string>& conditions, bool needUpdate = false) {
    static vector<string> lastConditions;

    // 只有在条件发生变化时才重绘
    if (!needUpdate && conditions == lastConditions) {
        return;
    }

    lastConditions = conditions;

    // 清除条件显示区域
    setfillcolor(BLACK);
    fillrectangle(50, 450, 350, 580);

    settextcolor(CYAN);
    settextstyle(16, 0, _T("微软雅黑"));
    setbkmode(TRANSPARENT);
    outtextxy(50, 450, L"当前选择的条件：");
    // 当没有条件时额外绘制一条提示信息
    if (conditions.empty()) {
        settextcolor(RGB(180, 180, 180));
        outtextxy(50, 470, L"(无选择的条件)");
        return;
    }

    int y = 470;
    for (const string& cond : conditions) {
        wstring wcond;
        // 将英文条件转换为中文显示
        if (cond == "each prime") wcond = L"每位都是质数";
        else if (cond == "distinct") wcond = L"各位数字不重复";
        else if (cond == "first even") wcond = L"第一位是偶数";
        else if (cond == "first odd") wcond = L"第一位是奇数";
        else if (cond == "second even") wcond = L"第二位是偶数";
        else if (cond == "second odd") wcond = L"第二位是奇数";
        else if (cond == "third even") wcond = L"第三位是偶数";
        else if (cond == "third odd") wcond = L"第三位是奇数";
        else wcond = wstring(cond.begin(), cond.end());

        outtextxy(50, y, wcond.c_str());
        y += 25;
    }
}

int main() {
    // 初始化图形窗口
    initgraph(800, 600);
    setbkcolor(BLACK);
    cleardevice();

    // 创建按钮（中文显示）
    vector<Button> buttons = {
        Button(50, 150, 140, 40, L"每位都是质数", "each prime", LIGHTGRAY),
        Button(200, 150, 140, 40, L"各位数字不重复", "distinct", LIGHTGRAY),
        Button(50, 200, 140, 40, L"第一位是偶数", "first even", LIGHTGRAY),
        Button(200, 200, 140, 40, L"第一位是奇数", "first odd", LIGHTGRAY),
        Button(50, 250, 140, 40, L"第二位是偶数", "second even", LIGHTGRAY),
        Button(200, 250, 140, 40, L"第二位是奇数", "second odd", LIGHTGRAY),
        Button(50, 300, 140, 40, L"第三位是偶数", "third even", LIGHTGRAY),
        Button(200, 300, 140, 40, L"第三位是奇数", "third odd", LIGHTGRAY),
        Button(50, 360, 100, 50, L"解密", "solve", LIGHTBLUE),
        Button(200, 360, 100, 50, L"清除", "clear", RGB(255, 127, 127))
    };

    vector<string> selectedConditions;
    vector<vector<int>> results;
    bool needRedraw = true;
    bool conditionsChanged = false;
    bool resultsChanged = false;

    // 初始绘制
    drawTitle();
    drawInstructions();
    drawSelectedConditions(selectedConditions, true);
    drawResults(results, true);

    for (auto& button : buttons) {
        button.draw();
    }

    // 主循环
    while (true) {
        // 只在需要时重绘
        if (needRedraw) {
            for (auto& button : buttons) {
                button.draw();
            }
            needRedraw = false;
        }

        if (conditionsChanged) {
            drawSelectedConditions(selectedConditions, true);
            conditionsChanged = false;
        }

        if (resultsChanged) {
            drawResults(results, true);
            resultsChanged = false;
        }

        // 处理鼠标点击
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            ScreenToClient(GetHWnd(), &cursorPos);

            for (size_t i = 0; i < buttons.size(); ++i) {
                if (buttons[i].isClicked(cursorPos.x, cursorPos.y)) {
                    if (i < 8) { // 条件按钮
                        if (buttons[i].isPressed) {
                            // 取消选择
                            buttons[i].isPressed = false;
                            buttons[i].color = LIGHTGRAY;
                            // 从选择列表中移除
                            auto it = find(selectedConditions.begin(), selectedConditions.end(), buttons[i].condition);
                            if (it != selectedConditions.end()) {
                                selectedConditions.erase(it);
                            }
                        }
                        else {
                            // 选择条件
                            buttons[i].isPressed = true;
                            buttons[i].color = LIGHTGREEN;
                            selectedConditions.push_back(buttons[i].condition);
                        }
                        needRedraw = true;
                        conditionsChanged = true;
                    }
                    else if (i == 8) { // 解密按钮
                        if (!selectedConditions.empty()) {
                            results.clear();
                            vector<int> current;
                            backtrack(current, 0, selectedConditions, results);
                            resultsChanged = true;
                        }
                    }
                    else if (i == 9) { // 清除按钮
                        selectedConditions.clear();
                        results.clear();
                        for (auto& btn : buttons) {
                            if (btn.condition != "solve" && btn.condition != "clear") {
                                btn.isPressed = false;
                                btn.color = LIGHTGRAY;
                            }
                        }
                        needRedraw = true;
                        conditionsChanged = true;
                        resultsChanged = true;
                    }

                    Sleep(200); // 防止重复点击
                    break;
                }
            }
        }

        // 检查退出
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }

        Sleep(50);
    }

    closegraph();
    return 0;
}
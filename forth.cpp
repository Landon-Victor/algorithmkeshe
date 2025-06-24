#include <graphics.h>
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <conio.h>
#include <windows.h>
using namespace std;

// ��ť�ṹ��
struct Button {
    int x, y, width, height;
    wstring text;
    bool isPressed;
    COLORREF color;
    string condition; // �����߼��жϵ�Ӣ��������

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
        settextstyle(16, 0, _T("΢���ź�"));

        // �����ı�����λ��
        int textWidth = textwidth(text.c_str());
        int textHeight = textheight(text.c_str());
        int textX = x + (width - textWidth) / 2;
        int textY = y + (height - textHeight) / 2;

        outtextxy(textX, textY, text.c_str());
    }
};

// ���0-9��Χ�ڵ�����
bool isPrimeDigit(int digit) {
    return digit == 2 || digit == 3 || digit == 5 || digit == 7;
}

// ��鲿�ֽ��Ƿ�������������
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

// ���ݺ���
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

// ���Ʊ���
void drawTitle() {
    settextcolor(WHITE);
    settextstyle(32, 0, _T("΢���ź�"));
    setbkmode(TRANSPARENT);
    wstring title = L"���������Ϸ";
    int titleWidth = textwidth(title.c_str());
    outtextxy((800 - titleWidth) / 2, 20, title.c_str());
}

// ����˵������
void drawInstructions() {
    settextcolor(YELLOW);
    settextstyle(16, 0, _T("΢���ź�"));
    setbkmode(TRANSPARENT);

    outtextxy(50, 80, L"����·���ťѡ������������");
    outtextxy(50, 100, L"��ɫ��ť��ʾ��ѡ�������");
}

// ���ƽ������
void drawResults(const vector<vector<int>>& results, bool needUpdate = false) {
    static vector<vector<int>> lastResults;

    // ֻ���ڽ�������仯ʱ���ػ�
    if (!needUpdate && results == lastResults) {
        return;
    }

    lastResults = results;

    // ����������
    setfillcolor(BLACK);
    fillrectangle(400, 150, 750, 590);

    settextcolor(WHITE);
    settextstyle(20, 0, _T("΢���ź�"));
    setbkmode(TRANSPARENT);
    outtextxy(400, 150, L"�ҵ������룺");

    if (results.empty()) {
        settextcolor(RED);
        outtextxy(400, 180, L"δ�ҵ���������������");
    }
    else {
        settextcolor(LIGHTGREEN);
        settextstyle(24, 0, _T("΢���ź�"));

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
                settextstyle(16, 0, _T("΢���ź�"));
                wstring more = L"... ���� " + to_wstring(results.size() - 15) + L" �����";
                outtextxy(400, y, more.c_str());
                break;
            }
        }
    }
}

// ���Ƶ�ǰѡ�������
void drawSelectedConditions(const vector<string>& conditions, bool needUpdate = false) {
    static vector<string> lastConditions;

    // ֻ�������������仯ʱ���ػ�
    if (!needUpdate && conditions == lastConditions) {
        return;
    }

    lastConditions = conditions;

    // ���������ʾ����
    setfillcolor(BLACK);
    fillrectangle(50, 450, 350, 580);

    settextcolor(CYAN);
    settextstyle(16, 0, _T("΢���ź�"));
    setbkmode(TRANSPARENT);
    outtextxy(50, 450, L"��ǰѡ���������");
    // ��û������ʱ�������һ����ʾ��Ϣ
    if (conditions.empty()) {
        settextcolor(RGB(180, 180, 180));
        outtextxy(50, 470, L"(��ѡ�������)");
        return;
    }

    int y = 470;
    for (const string& cond : conditions) {
        wstring wcond;
        // ��Ӣ������ת��Ϊ������ʾ
        if (cond == "each prime") wcond = L"ÿλ��������";
        else if (cond == "distinct") wcond = L"��λ���ֲ��ظ�";
        else if (cond == "first even") wcond = L"��һλ��ż��";
        else if (cond == "first odd") wcond = L"��һλ������";
        else if (cond == "second even") wcond = L"�ڶ�λ��ż��";
        else if (cond == "second odd") wcond = L"�ڶ�λ������";
        else if (cond == "third even") wcond = L"����λ��ż��";
        else if (cond == "third odd") wcond = L"����λ������";
        else wcond = wstring(cond.begin(), cond.end());

        outtextxy(50, y, wcond.c_str());
        y += 25;
    }
}

int main() {
    // ��ʼ��ͼ�δ���
    initgraph(800, 600);
    setbkcolor(BLACK);
    cleardevice();

    // ������ť��������ʾ��
    vector<Button> buttons = {
        Button(50, 150, 140, 40, L"ÿλ��������", "each prime", LIGHTGRAY),
        Button(200, 150, 140, 40, L"��λ���ֲ��ظ�", "distinct", LIGHTGRAY),
        Button(50, 200, 140, 40, L"��һλ��ż��", "first even", LIGHTGRAY),
        Button(200, 200, 140, 40, L"��һλ������", "first odd", LIGHTGRAY),
        Button(50, 250, 140, 40, L"�ڶ�λ��ż��", "second even", LIGHTGRAY),
        Button(200, 250, 140, 40, L"�ڶ�λ������", "second odd", LIGHTGRAY),
        Button(50, 300, 140, 40, L"����λ��ż��", "third even", LIGHTGRAY),
        Button(200, 300, 140, 40, L"����λ������", "third odd", LIGHTGRAY),
        Button(50, 360, 100, 50, L"����", "solve", LIGHTBLUE),
        Button(200, 360, 100, 50, L"���", "clear", RGB(255, 127, 127))
    };

    vector<string> selectedConditions;
    vector<vector<int>> results;
    bool needRedraw = true;
    bool conditionsChanged = false;
    bool resultsChanged = false;

    // ��ʼ����
    drawTitle();
    drawInstructions();
    drawSelectedConditions(selectedConditions, true);
    drawResults(results, true);

    for (auto& button : buttons) {
        button.draw();
    }

    // ��ѭ��
    while (true) {
        // ֻ����Ҫʱ�ػ�
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

        // ���������
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            ScreenToClient(GetHWnd(), &cursorPos);

            for (size_t i = 0; i < buttons.size(); ++i) {
                if (buttons[i].isClicked(cursorPos.x, cursorPos.y)) {
                    if (i < 8) { // ������ť
                        if (buttons[i].isPressed) {
                            // ȡ��ѡ��
                            buttons[i].isPressed = false;
                            buttons[i].color = LIGHTGRAY;
                            // ��ѡ���б����Ƴ�
                            auto it = find(selectedConditions.begin(), selectedConditions.end(), buttons[i].condition);
                            if (it != selectedConditions.end()) {
                                selectedConditions.erase(it);
                            }
                        }
                        else {
                            // ѡ������
                            buttons[i].isPressed = true;
                            buttons[i].color = LIGHTGREEN;
                            selectedConditions.push_back(buttons[i].condition);
                        }
                        needRedraw = true;
                        conditionsChanged = true;
                    }
                    else if (i == 8) { // ���ܰ�ť
                        if (!selectedConditions.empty()) {
                            results.clear();
                            vector<int> current;
                            backtrack(current, 0, selectedConditions, results);
                            resultsChanged = true;
                        }
                    }
                    else if (i == 9) { // �����ť
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

                    Sleep(200); // ��ֹ�ظ����
                    break;
                }
            }
        }

        // ����˳�
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }

        Sleep(50);
    }

    closegraph();
    return 0;
}
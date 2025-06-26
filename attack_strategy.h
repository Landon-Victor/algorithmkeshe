#pragma once
#include <vector>
#include <queue>
#include <map>

struct Attack
{
	int hurt;//伤害
	int cd;//技能cd
};

struct Node
{
	int cn;//已用回合数
	int ln;//回合数下界
	std::vector<int> valid_attacks;//可用技能
	std::vector<int> c_bosses_blood;//当前每个boss的血量
};

// 比较器
struct NodeCmp {
	bool operator()(const Node& a, const Node& b) const;
};

class AttackStrategy
{
public:
	AttackStrategy() = default;
	void load();
	void test();
	void branch_bound();
	bool check_leaf(const Node& node) const;
	std::vector<int> lose_bosses_blood(int hurt, std::vector<int> c_bosses_blood);
	int calculate_lower_bound(const Node& node) const;
	int get_attack(std::vector<int>& valid_attacks) const;
private:
	std::vector<int> bosses_blood;
	std::map<int, Attack> attacks;
	std::priority_queue<Node, std::vector<Node>, NodeCmp> min_heap;//根据回合数下界排序的最小堆
	int bestn;//最少回合数
};
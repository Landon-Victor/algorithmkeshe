#include <fstream>
#include <iostream>
#include "attack_strategy.h"
#include "json/json.h"


bool NodeCmp::operator()(const Node& a, const Node& b) const {
	return a.ln > b.ln;
}

void AttackStrategy::branch_bound()
{
	bestn = 0;
	best_attack_sequence.clear();
	min_heap = std::priority_queue<Node, std::vector<Node>, NodeCmp>();
	Node root;
	root.c_bosses_blood = bosses_blood;
	root.valid_attacks.resize(attacks.size(), 0);
	root.cn = 0;
	root.ln = calculate_lower_bound(root);
	root.attack_sequence.clear();
	min_heap.push(root);

	while (!min_heap.empty())
	{
		Node node = min_heap.top();
		min_heap.pop();
		if (check_leaf(node)) {
			bestn = node.cn;
			best_attack_sequence = node.attack_sequence;
			return;
		}
		for (int i = 0; i < node.valid_attacks.size(); ++i)
		{
			if (node.valid_attacks[i] > 0) continue;
			Attack& attack = attacks[i];
			Node next_node;
			next_node.cn = node.cn + 1;
			next_node.c_bosses_blood = lose_bosses_blood(attack.hurt, node.c_bosses_blood);
			//cd处理
			for (int cd : node.valid_attacks)
			{
				if (cd > 0) next_node.valid_attacks.push_back(cd - 1);
				else next_node.valid_attacks.push_back(0);
			}
			next_node.valid_attacks[i] = attack.cd;
			//回合数下界计算
			next_node.ln = next_node.cn + calculate_lower_bound(node);
			next_node.attack_sequence = node.attack_sequence;
			next_node.attack_sequence.push_back(i);
			min_heap.push(next_node);
		}
	}
}

bool AttackStrategy::check_leaf(const Node& node) const
{
	for (int blood : node.c_bosses_blood)
	{
		if (blood > 0) return false;
	}
	return true;
}

std::vector<int> AttackStrategy::lose_bosses_blood(int hurt, std::vector<int> c_bosses_blood)
{
	for (int& blood : c_bosses_blood)
	{
		if (blood <= 0) continue;
		blood -= hurt;
		if (blood < 0) blood = 0;
		return c_bosses_blood;
	}
	return c_bosses_blood;
}

int AttackStrategy::calculate_lower_bound(const Node& node) const
{
	//限界函数的设置
	//核心思想是把boss群看作一个boss,使用贪心策略打该boss
	//贪心策略是每次使用伤害最大的技能
	int sum_blood = 0;
	for (int blood : node.c_bosses_blood)
	{
		if (blood > 0) {
			sum_blood += blood;
		}
	}
	int count = 0;
	std::vector<int> valid_attacks = node.valid_attacks;
	while (sum_blood > 0)
	{
		int idx = get_attack(valid_attacks);
		if (idx == -1) break;
		sum_blood -= attacks.at(idx).hurt;
		count++;
	}
	return count;
}

int AttackStrategy::get_attack(std::vector<int>& valid_attacks) const
{
	int max_attack = -1;
	int max_index = -1;
	for (int i = 0; i < valid_attacks.size(); ++i)
	{
		if (valid_attacks[i] > 0)
			continue;
		if (attacks.at(i).hurt > max_attack) {
			max_attack = attacks.at(i).hurt;
			max_index = i;
		}
	}
	//更新cd表
	for (int& cd : valid_attacks)
	{
		if (cd > 0) cd = cd - 1;
	}
	if (max_index != -1)
		valid_attacks[max_index] = attacks.at(max_index).cd;
	return max_index;
}

void AttackStrategy::load(const std::string filename)
{
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

void AttackStrategy::test(const std::string filename)
{
	load(filename);
	if (bosses_blood.empty() || attacks.empty()) return;
	branch_bound();
	std::cout << "Minimum number of turns required: " << bestn << std::endl;
	std::cout << "Attack sequence: ";
	for (int idx : best_attack_sequence) {
		std::cout << idx << " ";
	}
	std::cout << std::endl;
}
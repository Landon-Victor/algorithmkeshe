#pragma once
#include <vector>
#include <queue>
#include <map>

struct Attack
{
	int hurt;//�˺�
	int cd;//����cd
};

struct Node
{
	int cn;//���ûغ���
	int ln;//�غ����½�
	std::vector<int> valid_attacks;//���ü���
	std::vector<int> c_bosses_blood;//��ǰÿ��boss��Ѫ��
};

// �Ƚ���
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
	std::priority_queue<Node, std::vector<Node>, NodeCmp> min_heap;//���ݻغ����½��������С��
	int bestn;//���ٻغ���
};
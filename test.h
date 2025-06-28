#pragma once
#include <iostream>
#include "map_generator.h"
#include "PasswordSolver.h"
#include "attack_strategy.h"
#include "maze_solver.h"
class Test 
{
public:
	Test() {}
	~Test() {}
	void  test_main()
	{
		while (1) {
			std::cout << "Test start" << std::endl;
			std::cout << "------------------------" << std::endl;
			std::cout << "选择要测试的任务\n" << std::endl;
			std::cout << "1. 测试地图生成" << std::endl;
			std::cout << "2. 测试动态规划寻路" << std::endl;
			std::cout << "3. 测试密码破解" << std::endl;
			std::cout << "4. 测试boss攻击策略" << std::endl;
			std::cout << "5. 测试所有功能" << std::endl;
			std::cout << "0. 退出" << std::endl;
			std::cout << "------------------------" << std::endl;
			int choice;
			std::cin >> choice;
			switch (choice)
			{
			case 1:
				test1();
				break;
			case 2:
				test2();
				break;
			case 3:
				test3();
				break;
			case 4:
				test4();
				break;
			case 5:
				test5();
				break;
			case 0:
				std::cout << "退出测试" << std::endl;
				return;
			default:
				std::cout << "无效的选择，请重新输入。" << std::endl;
			}
		}
	}
private:
	void test1()
	{
		std::cout << "请输入地图尺寸" << std::endl;
		int size;
		std::cin >> size;
		if (size <= 0) {
			std::cout << "地图尺寸必须大于0" << std::endl;
			return;
		}
		map_generator.get_a_map(size);
	}
	void test2()
	{
		std::cout << "请输入测试地图文件名" << std::endl;
		std::string filename;
		std::cin >> filename;
		if (filename.empty()) {
			std::cout << "文件名不能为空" << std::endl;
			return;
		}
		maze_solver.init(filename);
		maze_solver.solve();
		maze_solver.printPath();
		std::cout << "请输入保存路径的文件名" << std::endl;
		std::cin >> filename;
		maze_solver.save_path(filename);
	}
	void test3()
	{
		std::cout << "请输入解密任务要读取的文件名" << std::endl;
		std::string filename;
		std::cin >> filename;
		if (filename.empty()) {
			std::cout << "文件名不能为空" << std::endl;
			return;
		}
		password_solver.load(filename);
		std::vector<std::string> possible_passwords=password_solver.solve();
		if (possible_passwords.empty()) {
			std::cout << "没有找到可能的密码" << std::endl;
		} else {
			std::cout << "可能的密码有：" << std::endl;
			int count = 0;
			for (const auto& password : possible_passwords) {
				count++;
				std::cout << password << "   ";
				bool is_correct = password_solver.verifyPassword(password);
				if (is_correct) {
					std::cout << "正确 "<< std::endl;
					std::cout << "尝试次数：" << count << std::endl;
					break;
				} else {
					std::cout << "失败 "<< std::endl;
				}
			}
		}
	}
	void test4()
	{
		std::cout << "请输入boss战的文件名" << std::endl;
		std::string filename;
		std::cin >> filename;
		if (filename.empty()) {
			std::cout << "文件名不能为空" << std::endl;
			return;
		}
		attack_strategy.test(filename);
	}
	void test5()
	{
		std::cout << "开始测试所有功能" << std::endl;
		std::cout << "------------------------" << std::endl;
		std::cout << "地图生成测试：" << std::endl;
		test1();
		std::cout << "------------------------" << std::endl;
		std::cout << "动态规划寻路测试：" << std::endl;
		test2();
		std::cout << "------------------------" << std::endl;
		std::cout << "密码破解测试：" << std::endl;
		test3();
		std::cout << "------------------------" << std::endl;
		std::cout << "boss攻击策略测试：" << std::endl;
		test4();
		std::cout << "------------------------" << std::endl;
	}
private:
	MapGenerator map_generator;
	PasswordSolver password_solver;
	AttackStrategy attack_strategy;
	MazeSolver maze_solver;
};
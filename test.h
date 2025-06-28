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
			std::cout << "ѡ��Ҫ���Ե�����\n" << std::endl;
			std::cout << "1. ���Ե�ͼ����" << std::endl;
			std::cout << "2. ���Զ�̬�滮Ѱ·" << std::endl;
			std::cout << "3. ���������ƽ�" << std::endl;
			std::cout << "4. ����boss��������" << std::endl;
			std::cout << "5. �������й���" << std::endl;
			std::cout << "0. �˳�" << std::endl;
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
				std::cout << "�˳�����" << std::endl;
				return;
			default:
				std::cout << "��Ч��ѡ�����������롣" << std::endl;
			}
		}
	}
private:
	void test1()
	{
		std::cout << "�������ͼ�ߴ�" << std::endl;
		int size;
		std::cin >> size;
		if (size <= 0) {
			std::cout << "��ͼ�ߴ�������0" << std::endl;
			return;
		}
		map_generator.get_a_map(size);
	}
	void test2()
	{
		std::cout << "��������Ե�ͼ�ļ���" << std::endl;
		std::string filename;
		std::cin >> filename;
		if (filename.empty()) {
			std::cout << "�ļ�������Ϊ��" << std::endl;
			return;
		}
		maze_solver.init(filename);
		maze_solver.solve();
		maze_solver.printPath();
		std::cout << "�����뱣��·�����ļ���" << std::endl;
		std::cin >> filename;
		maze_solver.save_path(filename);
	}
	void test3()
	{
		std::cout << "�������������Ҫ��ȡ���ļ���" << std::endl;
		std::string filename;
		std::cin >> filename;
		if (filename.empty()) {
			std::cout << "�ļ�������Ϊ��" << std::endl;
			return;
		}
		password_solver.load(filename);
		std::vector<std::string> possible_passwords=password_solver.solve();
		if (possible_passwords.empty()) {
			std::cout << "û���ҵ����ܵ�����" << std::endl;
		} else {
			std::cout << "���ܵ������У�" << std::endl;
			int count = 0;
			for (const auto& password : possible_passwords) {
				count++;
				std::cout << password << "   ";
				bool is_correct = password_solver.verifyPassword(password);
				if (is_correct) {
					std::cout << "��ȷ "<< std::endl;
					std::cout << "���Դ�����" << count << std::endl;
					break;
				} else {
					std::cout << "ʧ�� "<< std::endl;
				}
			}
		}
	}
	void test4()
	{
		std::cout << "������bossս���ļ���" << std::endl;
		std::string filename;
		std::cin >> filename;
		if (filename.empty()) {
			std::cout << "�ļ�������Ϊ��" << std::endl;
			return;
		}
		attack_strategy.test(filename);
	}
	void test5()
	{
		std::cout << "��ʼ�������й���" << std::endl;
		std::cout << "------------------------" << std::endl;
		std::cout << "��ͼ���ɲ��ԣ�" << std::endl;
		test1();
		std::cout << "------------------------" << std::endl;
		std::cout << "��̬�滮Ѱ·���ԣ�" << std::endl;
		test2();
		std::cout << "------------------------" << std::endl;
		std::cout << "�����ƽ���ԣ�" << std::endl;
		test3();
		std::cout << "------------------------" << std::endl;
		std::cout << "boss�������Բ��ԣ�" << std::endl;
		test4();
		std::cout << "------------------------" << std::endl;
	}
private:
	MapGenerator map_generator;
	PasswordSolver password_solver;
	AttackStrategy attack_strategy;
	MazeSolver maze_solver;
};
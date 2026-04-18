#include "App.h"
#include <iostream>
#include <string>
#include "WaterManager.h"

void App::run()
{
	std::cout << "========================================\n  欢迎使用学生水费管理系统\n";

	WaterManager manager;
	while (1) {
		std::cout << "----------------------------------------\n 根据以下标识输入操作：\n";
		std::cout << "\t1.\t列出所有学生信息\n";
		std::cout << "\t2.\t列出单个学生的总水费记录\n";
		std::cout << "\t3.\t列出单个学生的所有水费记录\n";
		std::cout << "\t4.\t列出单个学生目标月份的水费记录\n";
		std::cout << "\t5.\t添加学生\n";
		std::cout << "\t6.\t设置学生信息\n";
		std::cout << "\t7.\t移除学生\n";
		std::cout << "\t8.\t添加目标学生的水费记录\n";
		std::cout << "\t9.\t设置目标学生目标月份的水费记录\n";
		std::cout << "\t10.\t移除目标学生目标月份的水费记录\n";
		std::cout << "\t0.\t退出程序\n";
		std::cout << "----------------------------------------\n";
		std::cout << "\n输入操作标识：";

		std::string input;
		std::getline(std::cin, input);

		if (input == "exit" || input == "e" || input == "exit()" || input == "0") {
			std::cout << "程序即将退出\n";
			return;
		}

		if (input == "?" || input == "？" || input == "." || input == "。") {
			std::cout << input;
			std::getline(std::cin, input);
			continue;
		}

		int operation = 0;
		try {
			operation = std::stoi(input);
		}
		catch (...) {
			std::cout << "你输的啥？ 按下ENTER键重新输入\n";
			std::getline(std::cin, input);
			continue;
		}

		if (operation <= -1 || operation >= 11) {
			std::cout << "输入的序号不在操作选项列表中 按下ENTER键重新输入\n";
			std::getline(std::cin, input);
			continue;
		}

		std::cout << "\n";

		std::string id;
		int year, month;
		double usage;
		result res;

		switch (operation) {
		case 1:
			listAllStudents(manager);
			break;
		case 2:
			listTotalRecord(manager);
			break;
		case 5:
			addStudent(manager);
		}
	}
}

void App::listAllStudents(WaterManager& manager)
{
	int value = 1;
	int* pageIndex = &value;
	result res;
	while (1) {
		res = manager.getAllStudents(pageIndex, 50
		);
		std::cout << "\n--------------------------------------\n";
		std::cout << res.info;

		while (1) {
			std::cout << "\n--------------------------------------\n 输入 n 进入下一页；\n 输入 p 进入上一页；\n 输入数字指定页数；\n 输入 exit 返回上一级；\n--------------------------------------\n";
			std::cout << "请输入操作标识：";
			std::string input;
			std::getline(std::cin, input);

			if (input == "") {
				std::cout << "请输入值 按下ENTER键以继续";
				std::getline(std::cin, input);
				continue;
			}

			if (input == "e" || input == "exit" || input == "exit()") return;

			try {
				*pageIndex = std::stoi(input);
				break;
			}
			catch (...) {
			}

			if (input == "n" || input == "next" ) {
				(*pageIndex)++;
				break;
			}

			if (input == "p" || input == "previous") {
				(*pageIndex)--;
				break;
			}

			std::cout << "你输入的标识不合法 按下ENTER键以继续";
			std::getline(std::cin, input);
		}
	}
}

void App::listTotalRecord(WaterManager& manager)
{
	while (1) {
		std::cout << "--------------------------------------\n 输入学号查询信息；\n 输入 exit 返回上一级；\n--------------------------------------\n";
		std::cout << "请输入操作标识：";
		std::string input;
		std::getline(std::cin, input);

		if (input == "e" || input == "exit" || input == "exit()") return;

		if (input == "") {
			std::cout << "请输入值 按下ENTER键以继续\n";
			std::getline(std::cin, input);
			continue;
		}

		Student* student = manager.queryStudent(input);
		if (student == nullptr) {
			std::cout << "查询失败。原因：找不到目标学号的学生\n按下ENTER键以继续\n";
			std::getline(std::cin, input);
			continue;
		}

		std::cout << 
			"学号：" + (*student).id +
			" 姓名：" + (*student).name +
			" 总用水量：" + std::to_string((*student).getTotalUsage()) +
			" 总消费：" + std::to_string((*student).getTotalCost()) + "\n";

		std::cout << "按下ENTER键以继续\n";
		std::getline(std::cin, input);
	}
}

void App::addStudent(WaterManager& manager) {
	while (1) {
		std::string input;
		std::cout << "--------------------------------------\n 输入学号和姓名添加学生；\n 输入 exit 在中途取消添加操作；\n--------------------------------------\n";
		
		while (1) {
			std::cout << "请输入学号：";
			std::getline(std::cin, input);

			if (input == "e" || input == "exit" || input == "exit()") return;

			if (!input.empty()) break;

			std::cout << "你需要输入一些内容\n";
		}

		std::string id = input;

		while (1) {
			std::cout << "请输入姓名：";
			std::getline(std::cin, input);

			if (input == "e" || input == "exit" || input == "exit()") return;

			if (!input.empty()) break;

			std::cout << "你需要输入一些内容\n";
		}

		std::string name = input;

		result res = manager.addStudent(id, name);
		if (!res.success) {
			std::cout << "添加失败！原因：" << res.info;
		}
		else {
			std::cout << "添加成功：" << res.info;
		}

		std::cout << "\n--------------------------------------\n 输入 yes 继续添加操作；\n 按ENTER键返回上一级；\n--------------------------------------\n";
		std::cout << "请输入标识：";
		std::getline(std::cin, input);

		if (input == "yes" || input == "y") {
			continue;
		}
		else {
			return;
		}
	}
}


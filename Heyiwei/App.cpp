#include "App.h"
#include <iostream>
#include <string>
#include "WaterManager.h"

static bool checkExit(std::string input) {
	if (input == "exit" || input == "e" || input == "exit()") return true;
	return false;
}

void App::run()
{
	std::cout << "========================================\n  欢迎使用学生水费管理系统\n";

	WaterManager manager;
	while (1) {
		std::cout << "----------------------------------------\n 根据以下标识输入操作：\n";
		std::cout << "\t1.\t列出所有学生信息\n";
		std::cout << "\t2.\t添加学生\n";
		std::cout << "\te.\t退出程序\n";
		std::cout << "----------------------------------------\n";
		std::cout << "\n输入操作标识：";

		std::string input;
		std::getline(std::cin, input);

		if (checkExit(input)) {
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

		if (operation < 1 || operation > 2) {
			std::cout << "输入的序号不在操作选项列表中 按下ENTER键重新输入\n";
			std::getline(std::cin, input);
			continue;
		}

		std::cout << "\n";

		switch (operation) {
		case 1:
			listAllStudents(manager);
			break;
		case 2:
			addStudent(manager);
			break;
		}
	}
}

int allStudentsPageIndex = 1;

void App::listAllStudents(WaterManager& manager)
{
	int* pageIndex = &allStudentsPageIndex;
	result res;
	while (1) {
		res = manager.getAllStudents(pageIndex, 16);
		std::cout << "\n--------------------------------------\n";
		std::cout << res.info;
		std::string input;

		while (1) {
			std::cout << "\n--------------------------------------\n";
			std::cout << "\tn\t进入下一页；\n";
			std::cout << "\tp\t进入上一页；\n";
			std::cout << "\t数字\t指定要查询的页数；\n";
			std::cout << "\ts[学号]\t对指定学号的学生执行操作\n";
			std::cout << "\texit\t返回上一级；\n";
			std::cout << "--------------------------------------\n";
			std::cout << "请输入操作标识：";
			std::getline(std::cin, input);

			if (input.empty()) {
				std::cout << "请输入值";
				continue;
			}

			if (checkExit(input)) return;

			if (input.length() >= 2 && (input[0] == 's' || input[0] == 'S')) {
				std::string numStr = input.substr(1);
				if (manager.getStudent(numStr) == nullptr) {
					std::cout << "\n没有找到指定学号的学生\n";
					break;
				}
				operateOnStudent(manager, numStr);
				break;
			}

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

			std::cout << "你输入的标识不合法";
		}
	}
}

int allRecordsPageIndex = 1;

void App::listAllRecords(WaterManager& manager, const std::string id)
{
	int* pageIndex = &allRecordsPageIndex;
	result res;

	while (1) {
		res = manager.getAllRecords(id, pageIndex, 16);
		std::cout << "\n--------------------------------------\n";
		std::cout << res.info;
		std::string input;

		while (1) {
			std::cout << "\n--------------------------------------\n";
			std::cout << " 输入 n 进入下一页；\n";
			std::cout << " 输入 p 进入上一页；\n";
			std::cout << " 输入 数字 指定要查询的页数；\n";
			std::cout << " 输入 s[年-月] 对指定月份的记录执行操作\n";
			std::cout << " 输入 exit 返回上一级；\n";
			std::cout << "--------------------------------------\n";
			std::cout << "请输入操作标识：";
			std::getline(std::cin, input);

			if (input.empty()) {
				std::cout << "请输入值";
				continue;
			}

			if (checkExit(input)) return;

			if (input.length() >= 2 && (input[0] == 's' || input[0] == 'S')) {
				std::string dateStr = input.substr(1);

				// 解析年-月格式
				size_t dashPos = dateStr.find('-');
				if (dashPos == std::string::npos) {
					std::cout << "格式错误，请使用 年-月 格式，例如：s2026-04";
					continue;
				}

				try {
					int year = std::stoi(dateStr.substr(0, dashPos));
					int month = std::stoi(dateStr.substr(dashPos + 1));

					// 验证年月有效性
					if (year < 1 || month < 1 || month > 12) {
						std::cout << "年份或月份无效";
						continue;
					}

					// 调用操作指定月份记录的函数
					operateOnRecord(manager, id, year, month);

					// 操作完成后刷新当前页面
					break;
				}
				catch (...) {
					std::cout << "年月格式不正确，请使用数字";
					continue;
				}
			}

			try {
				*pageIndex = std::stoi(input);
				break;
			}
			catch (...) {
			}

			if (input == "n" || input == "next") {
				(*pageIndex)++;
				break;
			}

			if (input == "p" || input == "previous") {
				(*pageIndex)--;
				break;
			}

			std::cout << "你输入的标识不合法";
		}
	}
}

void App::operateOnRecord(WaterManager& manager, const std::string id, int year, int month)
{
	std::string input;
	auto* student = manager.getStudent(id);
	if (student == nullptr) {
		std::cout << "没有找到目标学号的学生";
		return;
	}
	int index = student->getWaterRecordIndex(year, month);
	if (index == -1) {
		std::cout << "未找到目标年月份的水费记录";
		return;
	}
	while (1) {
		std::cout <<
			"\n 学号：" + id +
			std::to_string(year) + " 年 " +
			std::to_string(month) + " 月的水费记录  " +
			std::to_string(student->records[index].usage) + " （吨） " +
			std::to_string(student->records[index].cost) + " （元）\n";
		std::cout << "----------------------------------------\n";
		std::cout << " 根据以下标识输入操作：\n";
		std::cout << "\t1.\t设置这个水费记录\n";
		std::cout << "\t2.\t移除这个水费记录\n";
		std::cout << "\te.\t返回上一级\n";
		std::cout << "----------------------------------------\n";
		std::cout << "\n输入操作标识：";

		std::getline(std::cin, input);

		if (checkExit(input)) return;

		if (input.empty()) {
			std::cout << "你需要输入一些内容，或输入 exit 返回上一级\n";
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

		if (operation < 1 || operation > 2) {
			std::cout << "输入的序号不在操作选项列表中 按下ENTER键重新输入\n";
			std::getline(std::cin, input);
			continue;
		}

		switch (operation) {
		case 1:
			
			break;
		case 2:

			break;
		}
	}
}

void App::operateOnStudent(WaterManager& manager, const std::string id) {
	std::string input;
	auto* student = manager.getStudent(id);
	if (student == nullptr) {
		std::cout << "没有找到目标学号的学生";
		return;
	}
	while (1) {
		std::cout << "\n 学号：" + id + " 姓名：" + student->name + "\n";
		std::cout << "----------------------------------------\n";
		std::cout << " 根据以下标识输入操作：\n";
		std::cout << "\t1.\t查询他的所有水费记录\n";
		std::cout << "\t2.\t设置他的姓名\n";
		std::cout << "\t3.\t添加他的水费记录\n";
		std::cout << "\t4.\t移除他\n";
		std::cout << "\te.\t返回上一级\n";
		std::cout << "----------------------------------------\n";
		std::cout << "\n输入操作标识：";

		std::getline(std::cin, input);

		if (checkExit(input)) return;

		if (input.empty()) {
			std::cout << "你需要输入一些内容，或输入 exit 返回上一级\n";
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

		if (operation < 1 || operation > 4) {
			std::cout << "输入的序号不在操作选项列表中 按下ENTER键重新输入\n";
			std::getline(std::cin, input);
			continue;
		}

		result res;

		switch (operation) {
		case 1:
			listAllRecords(manager, id);
			break;
		case 2:
			
			break;
		case 3:
			addWaterRecord(manager, id);
			break;
		case 4:
			std::cout << "确定要移除他吗？(yes/no)";
			std::getline(std::cin, input);
			if (input == "yes") {
				res = manager.removeStudent(id);
				std::cout << res.info + "\n";
				return;
			}
			else {
				std::cout << "已取消移除操作";
			}
		}
	}
}

void App::listTotalRecord(WaterManager& manager)
{
	while (1) {
		std::cout << "--------------------------------------\n";
		std::cout << " 输入学号查询信息；";
		std::cout << "\n 输入 exit 返回上一级；"; 
		std::cout << "\n--------------------------------------\n";
		std::cout << "请输入操作标识：";
		std::string input;
		std::getline(std::cin, input);

		if (checkExit(input)) return;

		if (input.empty()) {
			std::cout << "请输入值\n";
			continue;
		}

		Student* student = manager.getStudent(input);
		if (student == nullptr) {
			std::cout << "查询失败。原因：找不到目标学号的学生\n按下ENTER键以继续\n";
			std::getline(std::cin, input);
			continue;
		}

		std::cout << 
			"学号：" + (*student).id +
			" 姓名：" + (*student).name +
			"  总用水量：" + std::to_string((*student).getTotalUsage()) +
			" 吨，总消费：" + std::to_string((*student).getTotalCost()) + " 元\n";

		std::cout << "按下ENTER键以继续\n";
		std::getline(std::cin, input);
	}
}

void App::addStudent(WaterManager& manager) {
	while (1) {
		std::cout << "--------------------------------------\n 输入学号和姓名添加学生；\n 输入 exit 在中途取消添加操作；\n--------------------------------------\n";
		std::string input;

		while (1) {
			std::cout << "请输入学号：";
			std::getline(std::cin, input);

			if (checkExit(input)) return;

			if (input.empty()) {
				std::cout << "你需要输入一些内容，或输入 exit 取消添加操作\n";
				continue;
			}

			Student* student = manager.getStudent(input);
			if (student == nullptr) break;
			std::cout << "不可添加具有相同学号的学生\n";
			continue;
		}

		std::string id = input;

		while (1) {
			std::cout << "请输入姓名：";
			std::getline(std::cin, input);

			if (checkExit(input)) return;

			if (input.empty()) {
				std::cout << "你需要输入一些内容，或输入 exit 取消添加操作\n";
				continue;
			}
			break;
		}

		std::string name = input;

		result res = manager.addStudent(id, name);
		if (!res.success) {
			std::cout << "添加失败！原因：" << res.info;
		}
		else {
			std::cout << "添加成功：" << res.info;
		}

		std::cout << "\n--------------------------------------\n 按 ENTER 键继续添加操作；\n 输入 exit 返回上一级；\n--------------------------------------\n";
		std::cout << "请输入标识：";
		std::getline(std::cin, input);

		if (input == "e" || input == "exit" || input == "exit()") return;
	}
}

void App::addWaterRecord(WaterManager& manager)
{
	std::string input;
	std::string id;
	int year, month;
	double usage;
	while (1) {
	addWaterRecordBegin:

		std::cout << "--------------------------------------\n";
		std::cout << " 输入学号，然后添加水费记录；\n";
		std::cout << " 输入 exit 返回上一级；\n";
		std::cout << "--------------------------------------\n";

		while (1) {
			std::cout << "请输入操作标识：";
			std::getline(std::cin, input);

			if (checkExit(input)) return;

			if (input.empty()) {
				std::cout << "你需要输入一些内容，或输入 exit 取消添加操作\n";
				continue;
			}
			Student* student = manager.getStudent(input);
			if (student != nullptr) break;
			std::cout << "添加失败。原因：找不到目标学号的学生\n";
			goto addWaterRecordBegin;
		}

		id = input;

		while (1) {
			std::cout << "\n请输入年份：";
			std::getline(std::cin, input);

			if (checkExit(input)) goto addWaterRecordBegin;

			if (input.empty()) {
				std::cout << "你需要输入一些内容，或输入 exit 取消添加操作\n";
				continue;
			}

			try {
				year = std::stoi(input);

				if (year <= 0) throw;

				if (year <= 1900) 
					std::cout << "哥们真是个古人\n";
				else if (year <= 2000) 
					std::cout << "哥们真是活在上个世纪的人\n";

				auto now = std::time(nullptr);
				std::tm localTimeStruct;
				auto* localTime = &localTimeStruct;
#if defined(_MSC_VER)
				localtime_s(localTime, &now);
#else
				localtime_r(&now, localTime);
#endif
				int currentYear = localTime->tm_year + 1900;

				if (year > currentYear)
					std::cout << "哥们真是从未来穿越回来的人\n";

				break;
			}
			catch (const std::exception&) {
				std::cout << "你输入的年份不合法\n";
				continue;
			}
		}

		while (1) {
			std::cout << "\n请输入月份：";
			std::getline(std::cin, input);

			if (checkExit(input)) goto addWaterRecordBegin;

			if (input.empty()) {
				std::cout << "你需要输入一些内容，或输入 exit 取消添加操作\n";
				continue;
			}

			try {
				month = std::stoi(input);
				if (month >= 1 && month <= 12) break;
			}
			catch (...) {
				std::cout << "请输入一个有效数字\n";
				continue;
			}

			std::cout << "你输入的月份不合法\n";
		}

		while (1) {
			std::cout << "\n请输入用水量（吨数）：";
			std::getline(std::cin, input);

			if (checkExit(input)) goto addWaterRecordBegin;

			if (input.empty()) {
				std::cout << "你需要输入一些内容，或输入 exit 取消添加操作\n";
				continue;
			}

			try {
				usage = std::stoi(input);
				if (usage < 0) throw;
				break;
			}
			catch (...) {
				std::cout << "你输入的数据不合法\n";
				continue;
			}
		}

		result res = manager.addWaterRecord(id, year, month, usage);

		if (res.success) {
			std::cout << "\n" + res.info + "\n";
		}
		else {
			std::cout << "添加记录失败：" + res.info + "\n";
		}
	}
}

void App::addWaterRecord(WaterManager& manager, const std::string id)
{
	std::string input;
	int year, month;
	double usage;

	while (1) {
		std::cout << "为 " + id + " 添加水费记录";

		std::cout << "\n请输入年份：";
		std::getline(std::cin, input);

		if (checkExit(input)) return;

		if (input.empty()) {
			std::cout << "你需要输入一些内容，或输入 exit 取消添加操作\n";
			continue;
		}

		try {
			year = std::stoi(input);

			if (year <= 0) {
				std::cout << "你输入的年份不合法\n";
				continue;
			}

			if (year <= 1900)
				std::cout << "哥们真是个古人\n";
			else if (year <= 2000)
				std::cout << "哥们真是活在上个世纪的人\n";

			auto now = std::time(nullptr);
			std::tm localTimeStruct;
			auto* localTime = &localTimeStruct;
#if defined(_MSC_VER)
			localtime_s(localTime, &now);
#else
			localtime_r(&now, localTime);
#endif
			int currentYear = localTime->tm_year + 1900;

			if (year > currentYear)
				std::cout << "哥们真是从未来穿越回来的人\n";

			break;
		}
		catch (const std::exception&) {
			std::cout << "你输入的年份不合法\n";
			continue;
		}
	}

	while (1) {
		std::cout << "\n请输入月份：";
		std::getline(std::cin, input);

		if (checkExit(input)) return;

		if (input.empty()) {
			std::cout << "你需要输入一些内容，或输入 exit 取消添加操作\n";
			continue;
		}

		try {
			month = std::stoi(input);
			if (month >= 1 && month <= 12) break;
		}
		catch (...) {
			std::cout << "请输入一个有效数字\n";
			continue;
		}

		std::cout << "你输入的月份不合法\n";
	}

	while (1) {
		std::cout << "\n请输入用水量（吨数）：";
		std::getline(std::cin, input);

		if (checkExit(input)) return;

		if (input.empty()) {
			std::cout << "你需要输入一些内容，或输入 exit 取消添加操作\n";
			continue;
		}

		try {
			usage = std::stoi(input);
			if (usage < 0) throw;
			break;
		}
		catch (...) {
			std::cout << "你输入的数据不合法\n";
			continue;
		}
	}

	result res = manager.addWaterRecord(id, year, month, usage);

	if (res.success) {
		std::cout << "\n" + res.info + "\n";
	}
	else {
		std::cout << "添加记录失败：" + res.info + "\n";
	}
}



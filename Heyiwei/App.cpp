#include <iostream>
#include <string>
#include "WaterManager.h"
#include "App.h"

static bool checkExit(std::string input) {
	if (input == "/e" || input == "/exit") return true;
	return false;
}

static bool checkEmpty(std::string input) {
	if (input.empty())
		std::cout << "你需要输入一些内容，或输入 /e 取消操作\n";
	return input.empty();
}

/// @brief 展示主菜单列表。输入标识选择操作功能。
void App::run()
{
	//程序开始运行的地方
	std::cout << "===================================================\n";
	std::cout << "  欢迎使用学生水费管理系统";

	while (1) {
		std::cout << "\n\n---------------------------------------------------\n";
		std::cout << " 根据以下标识输入操作：\n";
		std::cout << "\t1.\t列出所有学生信息\n";
		std::cout << "\t2.\t添加学生\n";
		std::cout << "\t/e\t退出程序\n";
		std::cout << "---------------------------------------------------\n\n";
		std::cout << "请输入操作标识：";
 
		// 读取用户输入
		std::string input;
		std::getline(std::cin, input);
  
		// 判断是否输入了退出指令
		if (checkExit(input)) {
			std::cout << "程序即将退出";
			return;
		}

		// 判断是否在扣问号
		if (input == "?" || input == "？" || input == "." || input == "。") {
			std::cout << input;
			continue;
		}

		int operation = 0;
		try {
			// 尝试将用户输入转换为数字。转换失败将进入 catch 内部
			operation = std::stoi(input);
		}
		catch (...) {
			std::cout << "请输入一个有效标识";
			continue; // 进入下一个循环，重新输入内容
		}

		// 判断输入的序号是否在操作选项列表内
		if (operation < 1 || operation > 2) {
			std::cout << "输入的序号不在操作选项列表中";
			continue;
		}

		std::cout << "\n";

		switch (operation) {
		case 1:
			listAllStudents();
			break;
		case 2:
			addStudent();
			break;
		}
	}
}

int allStudentsPageIndex = 1; // 全局变量，可用于保存上次查阅时的页面序号

/// @brief 展示所有学生列表。输入标识或页码翻阅页面浏览，或输入 s[序号] 查找选择学生。
void App::listAllStudents()
{
	int* pageIndex = &allStudentsPageIndex; // 使用指针
	result res;
	while (1) {
		res = manager.getAllStudents(pageIndex, 12); // 传入页面索引的指针，调用的函数内部对索引的修改会反映在此。
		std::cout << "\n---------------------------------------------------\n";
		std::cout << res.info;
		std::string input;

		while (1) {
			std::cout << "\n\n---------------------------------------------------\n";
			std::cout << "\tn\t进入下一页；\n";
			std::cout << "\tp\t进入上一页；\n";
			std::cout << "\t数字\t指定要查询的页数；\n";
			std::cout << "\ts[学号]\t对指定学号的学生执行操作\n";
			std::cout << "\t/e\t返回上一级；\n";
			std::cout << "---------------------------------------------------\n\n";
			std::cout << "请输入操作标识：";
			std::getline(std::cin, input);

			// 判断是否输入了空内容
			if (checkEmpty(input)) continue;

			// 判断是否输入了退出指令
			if (checkExit(input)) return;

			// 判断输入的 s[学号] 格式
			if (input.length() >= 2 && (input[0] == 's' || input[0] == 'S')) {
				std::string numStr = input.substr(1);
				if (manager.getStudent(numStr) == nullptr) {
					std::cout << "\n没有找到指定学号的学生：" + numStr;
					break;
				}
				operateOnStudent(numStr);
				break;
			}

			try {
				// 将输入的字符串转换为数字
				*pageIndex = std::stoi(input);
				break;
			}
			catch (...) {
			}

			if (input == "n" || input == "next" ) {
				(*pageIndex)++; // 下一页
				break;
			}

			if (input == "p" || input == "previous") {
				(*pageIndex)--; // 上一页
				break;
			}

			std::cout << "请输入一个有效标识";
		}
	}
}

int allRecordsPageIndex = 1;

/// @brief 展示所有水费记录列表。输入标识或页码翻阅页面浏览，或输入 s[年-月] 查找选择记录。
/// @param id 学生学号
void App::listAllRecords(const std::string id)
{
	int* pageIndex = &allRecordsPageIndex;
	result res;

	while (1) {
		res = manager.getAllRecords(id, pageIndex, 12);
		std::cout << "\n---------------------------------------------------\n";
		std::cout << res.info;
		std::string input;

		while (1) {
			std::cout << "\n\n---------------------------------------------------\n";
			std::cout << "\tn\t进入下一页；\n";
			std::cout << "\tp\t进入上一页；\n";
			std::cout << "\t数字\t指定要查询的页数；\n";
			std::cout << "\ts[年-月]\t对指定月份的记录执行操作\n";
			std::cout << "\t/e\t返回上一级；\n";
			std::cout << "---------------------------------------------------\n\n";
			std::cout << "请输入操作标识：";
			std::getline(std::cin, input);

			if (checkEmpty(input)) continue;
   
			// 判断是否输入了退出指令
			if (checkExit(input)) return;
  
			// 判断输入的 s[年-月] 格式
			if (input.length() >= 2 && (input[0] == 's' || input[0] == 'S')) {
				std::string dateStr = input.substr(1);

				// 解析年-月格式
				size_t dashPos = dateStr.find('-');
				if (dashPos == std::string::npos) {
					std::cout << "格式错误，请使用 年-月 格式，例如：s2026-04";
					continue;
				}

				try {
					//尝试将输入的字符串转换为数字
					int year = std::stoi(dateStr.substr(0, dashPos));
					int month = std::stoi(dateStr.substr(dashPos + 1));

					// 验证年月有效性
					if (year < 1 || month < 1 || month > 12) {
						std::cout << "年份或月份无效";
						continue;
					}

					// 调用操作指定月份记录的函数
					operateOnRecord(id, year, month);

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

			std::cout << "请输入一个有效标识";
		}
	}
}

/// @brief 对指定月份的水费记录执行操作
/// @param id 学生学号
/// @param year 年份
/// @param month 月份
void App::operateOnRecord(const std::string id, int year, int month)
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
			"\n\n 学号：" + id + " 姓名：" + 
			student->name +" 在 " +
			std::to_string(year) + " 年 " +
			std::to_string(month) + " 月的水费记录：\n 用水量：" +
			std::to_string(student->records[index].usage) + "（吨），计费：" +
			std::to_string(student->records[index].cost) + "（元）\n";
		std::cout << "---------------------------------------------------\n";
		std::cout << " 根据以下标识输入操作：\n";
		std::cout << "\t1.\t设置这个水费记录\n";
		std::cout << "\t2.\t移除这个水费记录\n";
		std::cout << "\t/e\t返回上一级\n";
		std::cout << "---------------------------------------------------\n\n";
		std::cout << "输入操作标识：";

		std::getline(std::cin, input);

		if (checkEmpty(input)) continue;

		if (checkExit(input)) return;

		int operation = 0;
		try {
			operation = std::stoi(input);
		}
		catch (...) {
			std::cout << "请输入一个有效标识";
			continue;
		}

		if (operation < 1 || operation > 2) {
			std::cout << "输入的序号不在操作选项列表中 按下ENTER键重新输入";
			std::getline(std::cin, input);
			continue;
		}

		result res;

		switch (operation) {
		case 1:
			setWaterRecord(id, year, month);
			break;
		case 2:
			std::cout << "确定要移除这个吗？(yes/no)";
			std::getline(std::cin, input);
			if (input == "yes") {
				res = manager.removeWaterRecord(id, year, month);
				std::cout << res.info;
				return;
			}
			else {
				std::cout << "已取消移除操作";
			}
			break;
		}
	}
}

/// @brief 对单个学生执行操作。输入指定标识查看所有水费记录、设置姓名、添加水费记录、移除学生。
/// @param id 学生学号
void App::operateOnStudent(const std::string id) {
	std::string input;
	auto* student = manager.getStudent(id);
	if (student == nullptr) {
		std::cout << "没有找到目标学号的学生";
		return;
	}
	while (1) {
		std::cout << "\n\n 学号：" + id + "   姓名：" + student->name + "\n";
		std::cout << "---------------------------------------------------\n";
		std::cout << " 根据以下标识输入操作：\n";
		std::cout << "\t1.\t查询他的所有水费记录\n";
		std::cout << "\t2.\t设置他的姓名\n";
		std::cout << "\t3.\t添加他的水费记录\n";
		std::cout << "\t4.\t移除他\n";
		std::cout << "\t/e\t返回上一级\n";
		std::cout << "---------------------------------------------------\n\n";
		std::cout << "输入操作标识：";

		std::getline(std::cin, input);

		if (checkEmpty(input)) continue;

		if (checkExit(input)) return;

		int operation = 0;
		try {
			operation = std::stoi(input);
		}
		catch (...) {
			std::cout << "请输入一个有效标识";
			continue;
		}

		if (operation < 1 || operation > 4) {
			std::cout << "输入的序号不在操作选项列表中";
			continue;
		}

		result res;

		switch (operation) {
		case 1:
			listAllRecords(id);
			break;
		case 2:
			setName(id);
			break;
		case 3:
			addWaterRecord(id);
			break;
		case 4:
			std::cout << "\n确定要移除他吗？(yes/no) ";
			std::getline(std::cin, input);
			if (input == "yes") {
				res = manager.removeStudent(id);
				std::cout << res.info;
				return;
			}
			else {
				std::cout << "已取消移除操作";
			}
		}
	}
}

/// @brief 设置指定学生的名字
/// @param id 学生学号
void App::setName(const std::string id) {
	std::string name;
	if (!enterName(name)) return;
	result res = manager.setStudent(id, name);
	std::cout << res.info;
}

/// @brief 设置指定学生在指定年月的水费记录
/// @param id 学生学号
/// @param year 年份
/// @param month 月份
void App::setWaterRecord(const std::string id, int year, int month)
{
	double usage;
	if (!enterUsage(usage)) return;
	result res = manager.setWaterRecord(id, year, month, usage);
	std::cout << res.info;
}

/// @brief 展示添加学生菜单列表。输入学号和姓名添加学生，或输入 `/e` 标识取消添加操作。
void App::addStudent() {
	Student student;
	while (1) {
		if (!enterStudent(student)) return;

		result res = manager.addStudent(student);
		std::cout << res.info + "\n";

		if (!promptContinue()) return;
	}
}

/// @brief 输入学生信息
/// @param student 学生对象
/// @return 是否输入成功
bool App::enterStudent(Student& student) {

	std::cout << "\n---------------------------------------------------\n";
	std::cout << "\t学号和姓名\t添加学生；\n";
	std::cout << "\t/e\t在中途取消操作；\n";
	std::cout << "---------------------------------------------------\n\n";

	std::string id;
	if (!enterId(id)) return false;

	std::string name;
	if (!enterName(name)) return false;

	student.id = id;
	student.name = name;

	return true;
}

/// @brief 输入学生学号
/// @param id 学号
/// @return 是否输入成功
bool App::enterId(std::string& id)
{
	std::string input;

	while (1) {
		std::cout << "请输入学号：";
		std::getline(std::cin, input);

		if (checkEmpty(input)) continue;

		if (checkExit(input)) return false;

		Student* student = manager.getStudent(input);
		if (student == nullptr) break;
		std::cout << "不可添加具有相同学号的学生\n\n";
		continue;
	}

	id = input;
	return true;
}

/// @brief 输入学生姓名
/// @param name 姓名
/// @return 是否输入成功
bool App::enterName(std::string& name)
{
	std::string input;

	while (1) {
		std::cout << "\n请输入姓名：";
		std::getline(std::cin, input);

		if (checkEmpty(input)) continue;

		if (checkExit(input)) return false;

		break;
	}

	name = input;
	return true;
}

/// @brief 输入年份
/// @param year 年份
/// @return 是否输入成功
bool App::enterYear(int& year) {
	std::string input;
	while (1) {
		std::cout << "\n请输入年份：";
		std::getline(std::cin, input);

		if (checkEmpty(input)) continue;

		if (checkExit(input)) return false;

		try {
			year = std::stoi(input);

			if (year <= 0) return false;

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
	return true;
}

/// @brief 输入月份
/// @param month 月份
/// @return 是否输入成功
bool App::enterMonth(int& month) {
	std::string input;
	while (1) {
		std::cout << "\n请输入月份：";
		std::getline(std::cin, input);

		if (checkEmpty(input)) continue;

		if (checkExit(input)) return false;

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
	return true;
}

/// @brief 输入用水量
/// @param usage 用水量
/// @return 是否输入成功
bool App::enterUsage(double& usage) {
	std::string input;
	while (1) {
		std::cout << "\n请输入用水量（吨数）：";
		std::getline(std::cin, input);

		if (checkEmpty(input)) continue;

		if (checkExit(input)) return false;

		try {
			usage = std::stod(input);
			if (usage < 0) throw;
			break;
		}
		catch (...) {
			std::cout << "你输入的数据不合法\n";
			continue;
		}
	}
	return true;
}

/// @brief 输入水费记录
/// @param record 水费记录对象
/// @return 是否输入成功
bool App::enterWaterRecord(WaterRecord& record)
{
	std::string input;

	std::cout << "\n---------------------------------------------------\n";
	std::cout << "\t年、月、用水量\t添加水费记录；\n";
	std::cout << "\t/e\t在中途取消添加操作；\n";
	std::cout << "---------------------------------------------------\n";

	int year, month;
	double usage;

	if (!enterYear(year)) return false;
	if (!enterMonth(month)) return false;
	if (!enterUsage(usage)) return false;

	record.year = year;
	record.month = month;
	record.usage = usage;
	record.cost = usage * PRICE_PER_TON;

	return true;
}

/// @brief 提示是否继续
/// @return 是否继续
bool App::promptContinue() {
	std::string input;

	std::cout << "\n---------------------------------------------------\n";
	std::cout << "\t按下ENTER\t继续添加操作；\n";
	std::cout << "\t/e\t返回上一级；\n";
	std::cout << "---------------------------------------------------\n\n";
	std::cout << "请输入标识：";
	std::getline(std::cin, input);

	return !checkExit(input);
}

/// @brief 添加水费记录
/// @param id 学号
void App::addWaterRecord(const std::string id)
{
	while (1) {
		WaterRecord record;
		if (!enterWaterRecord(record)) return;

		result res = manager.addWaterRecord(id, record);
		std::cout << "\n" + res.info;

		if (!promptContinue()) return;
	}
}
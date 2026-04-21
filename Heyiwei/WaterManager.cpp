#include "WaterManager.h"
#include <iostream>
#include <fstream> // 读写文件
#include <string>
#include "json.hpp"
#include "Windows.h"

using json = nlohmann::json;

// 控制台应用使用 GBK 中文编码，与 json 保存文件使用的 utf-8 编码不同。需要进行编码转换。
// 从控制台的 GBK 编码转换为 json 文件的 UTF-8 编码
std::string gbkToUtf8(const std::string& gbkStr) {
	if (gbkStr.empty()) return "";

	int wideLen = MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, nullptr, 0);
	if (wideLen == 0) return gbkStr;

	std::vector<wchar_t> wide(wideLen);
	MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, &wide[0], wideLen);

	int utf8Len = WideCharToMultiByte(CP_UTF8, 0, &wide[0], -1, nullptr, 0, nullptr, nullptr);
	if (utf8Len == 0) return gbkStr;

	std::vector<char> utf8(utf8Len);
	WideCharToMultiByte(CP_UTF8, 0, &wide[0], -1, &utf8[0], utf8Len, nullptr, nullptr);

	return std::string(utf8.data());
}

// 从 json 文件的 UTF-8 编码转换为控制台的 GBK 编码
std::string utf8ToGbk(const std::string& utf8Str) {
	if (utf8Str.empty()) return "";

	int wideLen = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
	if (wideLen == 0) return utf8Str;

	std::vector<wchar_t> wide(wideLen);
	MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wide[0], wideLen);

	int gbkLen = WideCharToMultiByte(CP_ACP, 0, &wide[0], -1, nullptr, 0, nullptr, nullptr);
	if (gbkLen == 0) return utf8Str;

	std::vector<char> gbk(gbkLen);
	WideCharToMultiByte(CP_ACP, 0, &wide[0], -1, &gbk[0], gbkLen, nullptr, nullptr);

	return std::string(gbk.data());
}

// 读取、写入水费记录数据
static void to_json(json& j, const WaterRecord& r) // 建立一个静态函数，此函数只在当前文件里面生效
{
	j = json{
		{"year", r.year},
		{"month", r.month},
		{"usage", r.usage},
		{"cost", r.cost}
	};
}

static void from_json(const json& j, WaterRecord& r)
{
	j.at("year").get_to(r.year);
	j.at("month").get_to(r.month);
	j.at("usage").get_to(r.usage);
	j.at("cost").get_to(r.cost);
}

// 读取、写入学生信息记录
static void to_json(json& j, const Student& s)
{
	j = json{
		{"id", gbkToUtf8(s.id)},
		{"name", gbkToUtf8(s.name)},
		{"records", s.records}
	};
}

static void from_json(const json& j, Student& s)
{
	std::string idUtf8, nameUtf8;
	j.at("id").get_to(idUtf8);
	j.at("name").get_to(nameUtf8);
	j.at("records").get_to(s.records);

	s.id = utf8ToGbk(idUtf8);
	s.name = utf8ToGbk(nameUtf8);
}

// 实例构造时读取保存的数据
WaterManager::WaterManager()
{
	loadFromFile();
}

// 实例销毁时保存数据
WaterManager::~WaterManager()
{
	saveToFile();
}

// 保存文件
void WaterManager::saveToFile()
{
	json j = students;
	std::ofstream file("students.json");
	file << j.dump(4);
}

// 读取文件
void WaterManager::loadFromFile()
{
	std::ifstream file("students.json");
	if (!file.is_open())
	{
		return;
	}
	json j;
	file >> j;
	students = j.get<std::vector<Student>>();
}

// 依据学号查找目标学生索引
int WaterManager::findStudentIndex(const std::string& id) {
	for (size_t i = 0; i < students.size(); ++i)
		if (students[i].id == id)
			return i;
	return -1;
}

// 添加学生
result WaterManager::addStudent(const std::string& id, const std::string& name){
	if (findStudentIndex(id) != -1)
		return { false, "不能添加具有相同学号的学生（指定学号的学生已存在：" + id + "）" };
	Student student;
	student.id = id;
	student.name = name;
	students.push_back(student);
	return { true, "添加学生成功：" + id + " " + name};
}

// 移除学生
result WaterManager::removeStudent(const std::string& id) {
	int index = findStudentIndex(id);
	if (index == -1)
		return { false, "指定学号的学生不存在：" + id };
	students.erase(students.begin() + index);
	return { true, "删除成功" };
}

// 添加目标学生水费记录
result WaterManager::addWaterRecord(const std::string& id, int year, int month, double usage) {
	int index = findStudentIndex(id);
	if (index == -1) 
		return { false, "指定学号的学生不存在：" + id };

	for (const auto& record : students[index].records) {
		if (record.year == year && record.month == month) {
			std::string info =
				"学生 " + id + 
				" 已存在 " + std::to_string(year) +
				" 年 " + std::to_string(month) + 
				" 月的水费记录，不能重复添加";
			return { false, info };
		}
	}

	double cost = usage * PRICE_PER_TON;
	WaterRecord record{};
	record.year = year;
	record.month = month;
	record.usage = usage;
	record.cost = cost;

	students[index].records.push_back(record);
	std::string info =
		"添加学生" + id + " 水费记录成功：" +
		std::to_string(year) + " 年 " +
		std::to_string(month) + " 月 共使用 " +
		std::to_string(usage) + " 吨水，计费 " +
		std::to_string(cost) + " 元";
	return { true, info };
}

// 设置目标学生目标月份的水费记录
result WaterManager::setWaterRecord(const std::string& id, int year, int month, double usage) {
	int index = findStudentIndex(id);
	if (index == -1)
		return { false, "指定学号的学生不存在：" + id };
	auto& records = students[index].records;
	for (auto it = records.begin(); it != records.end();) {
		if (it->year == year && it->month == month) {
			it->usage = usage;
			it->cost = usage * PRICE_PER_TON;
			return { true , "修改记录成功\n  " + queryTotalRecord(id).info };
		}
		++it;
	}
}

// 移除目标学生目标月份的水费记录
result WaterManager::removeWaterRecord(const std::string& id, int year, int month) {
	int index = findStudentIndex(id);
	if (index == -1)
		return { false, "指定学号的学生不存在：" + id };
	int r = students[index].getWaterRecordIndex(year, month);
	if (r == -1) 
		return { false, "未找到记录：" + std::to_string(year) + " 年 " + std::to_string(month) + " 月" };
	students[index].records.erase(students[index].records.begin() + r);
	return { true, "删除成功" };
}

result WaterManager::queryTotalRecord(const std::string& id) {
	int index = findStudentIndex(id);
	if (index == -1)
		return { false, "指定学号的学生不存在：" + id };
	std::string info = 
		"学号：" + students[index].id + 
		" 姓名：" + students[index].name + 
		" 总用水量：" + std::to_string(students[index].getTotalUsage()) + 
		" 总消费：" + std::to_string(students[index].getTotalCost());
	return { true, info };
}

result WaterManager::queryRecord(const std::string& id, int year, int month)
{
	int index = findStudentIndex(id);
	if (index == -1)
		return { false, "指定学号的学生不存在：" + id };
	
}

// 查询学生，返回指向学生的指针
Student* WaterManager::getStudent(const std::string& id) {
	int index = findStudentIndex(id);
	if (index == -1) return nullptr;
	return &students[index];
}

result WaterManager::queryAllRecords(const std::string& id) {
	int index = findStudentIndex(id);
	if (index == -1)
		return { false, "指定学号的学生不存在：" + id };
	std::string info = "学号 " + students[index].id + " 学生的详细水费记录：";
	for (const auto& record : students[index].records) {
		std::string recordStr =
			std::to_string(record.year) + "年" +
			std::to_string(record.month) + "月 记录 - 用水量：" +
			std::to_string(record.usage) + "吨 计费：" +
			std::to_string(record.cost);
		info += "\n" + recordStr;
	}
	if (students[index].records.empty())
		info += "\n暂无水费记录";
	return { true, info };
}

result WaterManager::getAllRecords(const std::string& id, int* pageIndex, int count) {
	int index = findStudentIndex(id);
	if (index == -1)
		return { false, "指定学号的学生不存在：" + id };

	auto& records = students[index].records;

	if (records.empty()) {
		return { true, "他暂时没有水费记录" };
	}

	int totalPages = (records.size() + count - 1) / count;

	if (*pageIndex < 1) *pageIndex = 1;
	if (*pageIndex > totalPages) *pageIndex = totalPages;
	int pageMax = ((*pageIndex) * count >= records.size()) ? records.size() : (*pageIndex) * count;

	std::string info =
		"显示 " + std::to_string(records.size()) +
		" 中的第 " + std::to_string(((*pageIndex) - 1) * count + 1) +
		"~" + std::to_string(pageMax) +
		"（页面序号 " + std::to_string(*pageIndex) + "）个水费记录：\n";

	for (size_t i = ((*pageIndex) - 1) * count; i < pageMax; ++i)
		info += 
		"\n\t" + std::to_string(records[i].year) + 
		"年" + std::to_string(records[i].month) + 
		"月 \t使用量：" + std::to_string(records[i].usage) +
		"（吨） \t计费：" + std::to_string(records[i].cost) + 
		"（元）";

	return { true, info };
}

// 获取所有学生信息
result WaterManager::getAllStudents(int* pageIndex, int count) {
	if (students.empty())
		return { true, "当前暂无学生记录" };

	int totalPages = (students.size() + count - 1) / count;

	if (*pageIndex < 1) *pageIndex = 1;
	if (*pageIndex > totalPages) *pageIndex = totalPages;
	int pageMax = ((*pageIndex) * count >= students.size()) ? students.size() : (*pageIndex) * count;

	std::string info = 
		"显示 " + std::to_string(students.size()) + 
		" 中的第 " + std::to_string(((*pageIndex) - 1) * count + 1) + 
		"~" + std::to_string(pageMax) +
		"（页面序号 " + std::to_string(*pageIndex) + "）名学生的信息：\n";
	
	for (size_t i = ((*pageIndex) - 1) * count; i < pageMax; ++i)
		info += "\n\t学号：" + students[i].id + " \t姓名：" + students[i].name;

	return { true, info };
}
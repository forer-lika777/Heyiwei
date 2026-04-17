#include "WaterManager.h"
#include <iostream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

static void to_json(json& j, const WaterRecord& r)
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

static void to_json(json& j, const Student& s)
{
	j = json{
		{"id", s.id},
		{"name", s.name},
		{"records", s.records}
	};
}

static void from_json(const json& j, Student& s)
{
	j.at("id").get_to(s.id);
	j.at("name").get_to(s.name);
	j.at("records").get_to(s.records);
}

WaterManager::WaterManager()
{
	loadFromFile();
}

WaterManager::~WaterManager()
{
	saveToFile();
}

void WaterManager::saveToFile()
{
	json j = students;
	std::ofstream file("students.json");
	file << j.dump(4);
}

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

int WaterManager::findStudentIndex(const std::string& id) {
	for (size_t i = 0; i < students.size(); ++i)
		if (students[i].id == id)
			return i;
	return -1;
}

result WaterManager::addStudent(const std::string& id, const std::string& name){
	if (findStudentIndex(id) != -1)
		return { false, "学号已存在" };
	Student student;
	student.id = id;
	student.name = name;
	students.push_back(student);
	return { true, "添加成功" };
}

result WaterManager::addWaterRecord(const std::string& id, int year, int month, double usage) {
	int index = findStudentIndex(id);
	if (index == -1) 
		return { false, "学生不存在" };
	WaterRecord record{};
	record.year = year;
	record.month = month;
	record.usage = usage;
	record.cost = usage * PRICE_PER_TON;

	students[index].records.push_back(record);
	return { true, "添加成功" };
}

result WaterManager::removeWaterRecord(const std::string& id, int year, int month) {
	int index = findStudentIndex(id);
	if (index == -1)
		return { false, "学生不存在" };
	auto& records = students[index].records;
	for (auto it = records.begin(); it != records.end(); /* 不在这里 ++ */) {
		if (it->year == year && it->month == month) {
			it = records.erase(it);  // erase 返回下一个有效迭代器
			return { true, "删除成功" };
		}
		++it;
	}
	return { false, "未找到记录" };
}

result WaterManager::queryStudent(const std::string& id) {
	int index = findStudentIndex(id);
	if (index == -1) 
		return { false, "学生不存在" };
	std::string info = 
		"学号：" + students[index].id + 
		" 姓名：" + students[index].name + 
		" 总用水量：" + std::to_string(students[index].getTotalUsage()) + 
		" 总消费：" + std::to_string(students[index].getTotalCost());
	return { true, info };
}

result WaterManager::queryAllRecords(const std::string& id) {
	int index = findStudentIndex(id);
	if (index == -1)
		return { false, "学生不存在" };
	std::string info = "学号 " + students[index].id + " 学生的详细水费记录：";
	for (const auto& record : students[index].records) {
		std::string recordStr =
			std::to_string(record.year) + "年" +
			std::to_string(record.month) + "月 记录 - 用水量：" +
			std::to_string(record.usage) + "吨 计费：" +
			std::to_string(record.cost);
		info += "\n" + recordStr;
	}
	if (students[index].records.empty()) {
		info += "\n暂无水费记录";
	}
	return { true, info };
}

result WaterManager::removeStudent(const std::string& id) {
	int index = findStudentIndex(id);
	if (index == -1)
		return { false, "学生不存在" };
	students.erase(students.begin() + index);
	return { true, "删除成功" };
}

result WaterManager::getAllStudents(int pageIndex, int count) {
	int index = pageIndex;
	if (pageIndex * count )
	std::string info = "显示数量 " + std::to_string(count) + " （页面序号 " + std::to_string(pageIndex) + "）名学生的信息：";
	
}
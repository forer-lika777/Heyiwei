#pragma once // 以.h为结尾的头文件中添加防止这个头文件在编译的过程中重复编译导致报错
#include <vector>
#include <string>
#include "Student.h"

struct result {
	bool success; // 利用布尔值判断成功与否
	std::string info; // 利用一个字符串字段来对成功与否的说明
};

// 水费管理类的声明
class WaterManager
{
private: // 外部函数无法调用部分
	std::vector<Student> students; // 定义一个用于储存每个学生数据的动态数组
	const double PRICE_PER_TON = 2.5; // 定义常量水费

	int findStudentIndex(const std::string& id); // 传递id这个数据用传递地址的方法
	void saveToFile();
	void loadFromFile();

public: // 外部函数可以调用部分
	WaterManager();
	~WaterManager();

	result getAllStudents(int* pageIndex, int count);
	result getAllRecords(const std::string& id, int* paggeIndex, int count);
	Student* getStudent(const std::string& id);

	result queryTotalRecord(const std::string& id);
	result queryAllRecords(const std::string& id);
	result queryRecord(const std::string& id, int year, int month);

	result addStudent(const std::string& id, const std::string& name);
	result setStudent(const std::string& id, const std::string& name);
	result removeStudent(const std::string& id);

	result addWaterRecord(const std::string& id, int year, int month, double usage);
	result setWaterRecord(const std::string& id, int year, int month, double usage);
	result removeWaterRecord(const std::string& id, int year, int month);
};


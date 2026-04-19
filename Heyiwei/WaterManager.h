#pragma once
#include <vector>
#include <string>
#include "Student.h"

struct result {
	bool success;
	std::string info;
};

// 水费管理类的声明
class WaterManager
{
private:
	std::vector<Student> students;
	const double PRICE_PER_TON = 2.5;

	int findStudentIndex(const std::string& id);
	void saveToFile();
	void loadFromFile();

public:
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


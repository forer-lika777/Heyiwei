#pragma once
#include <vector>
#include <string>
#include "Student.h"

struct result {
	bool success;
	std::string info;
};

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

	result addStudent(const std::string& id, const std::string& name);
	result addWaterRecord(const std::string& id, int year, int month, double usage);
	result removeWaterRecord(const std::string& id, int year, int month);
	result queryStudent(const std::string& id);
	result queryAllRecords(const std::string& id);
	result getAllStudents(int pageIndex, int count);
	result removeStudent(const std::string& id);
};


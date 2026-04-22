#pragma once
#include <string>
#include <vector>

struct WaterRecord {
	int year;
	int month;
	double usage;
	double cost;
};

class Student { // 学生类，包含学号、姓名和水费记录等信息
public:
	std::string id;
	std::string name;
	std::vector<WaterRecord> records;

	double getTotalUsage() const;
	double getTotalCost() const;
	int getWaterRecordIndex(int year, int month);
};


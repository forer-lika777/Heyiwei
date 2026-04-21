#pragma once
#include <string>
#include <vector>

struct WaterRecord {
	int year;
	int month;
	double usage;
	double cost;
};

class Student { // class默认为private，struct默认为public 功能上相等
public:
	std::string id;
	std::string name;
	std::vector<WaterRecord> records;

	double getTotalUsage() const;
	double getTotalCost() const;
	int getWaterRecordIndex(int year, int month);
};


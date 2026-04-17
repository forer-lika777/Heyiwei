#pragma once
#include <string>
#include <vector>

struct WaterRecord {
	int year;
	int month;
	double usage;
	double cost;
};

class Student {
public:
	std::string id;
	std::string name;
	std::vector<WaterRecord> records;

	double getTotalUsage() const;
	double getTotalCost() const;
};


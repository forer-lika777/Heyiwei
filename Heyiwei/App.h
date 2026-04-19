#pragma once
#include "WaterManager.h"

class App
{
public:
	void run();
	void listAllStudents(WaterManager& manager);
	void listTotalRecord(WaterManager& manager);
	void listAllRecords(WaterManager& manager, const std::string id);
	void addStudent(WaterManager& manager);
	void addWaterRecord(WaterManager& manager);
	void addWaterRecord(WaterManager& manager, const std::string id);
	void operateOnStudent(WaterManager& manager, const std::string id);
	void operateOnRecord(WaterManager& manager, const std::string id, int year, int month);
	std::string getYear();
};


#pragma once
#include "WaterManager.h"

class App
{
private:
	WaterManager& manager;
	const double PRICE_PER_TON = 2.5; // 定义常量水费
public:
	App(WaterManager& mgr) : manager(mgr) {}
	void run();
	void listAllStudents();
	void listAllRecords(const std::string id);

	void addStudent();
	void addWaterRecord(const std::string id);

	void operateOnStudent(const std::string id);
	void operateOnRecord(const std::string id, int year, int month);

	void setName(const std::string id);
	void setWaterRecord(const std::string id, int year, int month);

	bool promptContinue();
	bool enterWaterRecord(WaterRecord& record);
	bool enterStudent(Student& student);
	bool enterId(std::string& id);
	bool enterName(std::string& name);
	bool enterYear(int& year);
	bool enterMonth(int& month);
	bool enterUsage(double& usage);
};


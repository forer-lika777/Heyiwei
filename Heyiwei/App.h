#pragma once
#include "WaterManager.h"

class App
{
private:
	WaterManager& manager;
public:
	App(WaterManager& mgr) : manager(mgr) {}
	void run();
	void listAllStudents();
	void listTotalRecord();
	void listAllRecords(const std::string id);
	void addStudent();
	void addWaterRecord();
	void addWaterRecord(const std::string id);
	void operateOnStudent(const std::string id);
	void operateOnRecord(const std::string id, int year, int month);
	bool enterWaterRecord(WaterRecord& record);
	bool enterStudent(Student& student);
	bool enterId(std::string& id);
	bool enterName(std::string& name);
};


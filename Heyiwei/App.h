#pragma once
#include "WaterManager.h"

class App
{
public:
	void run();
	void listAllStudents(WaterManager& manager);
	void listTotalRecord(WaterManager& manager);
	void addStudent(WaterManager& manager);
};


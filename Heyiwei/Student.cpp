#include "Student.h"

// 获取学生的总计费
double Student::getTotalCost() const {
	double cost = 0;
	for (const auto& r : records)
		cost += r.usage;
	return cost;
}

// 获取学生的总用水吨数
double Student::getTotalUsage() const {
	double usage = 0;
	for (const auto& r : records) 
		usage += r.usage;
	return usage;
}

// 依据年月获取目标水费记录的索引
int Student::getWaterRecordIndex(int year, int month)
{
	for (size_t i = 0; i < records.size(); ++i) {
		if (records[i].year == year && records[i].month == month) {
			return static_cast<int>(i);
		}
	}
	return -1;
}
#include "Student.h"

double Student::getTotalCost() const {
	double cost = 0;
	for (const auto& r : records)
		cost += r.usage;
	return cost;
}

double Student::getTotalUsage() const {
	double usage = 0;
	for (const auto& r : records) 
		usage += r.usage;
	return usage;
}
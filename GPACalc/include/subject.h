#pragma once
#include <vector>

struct subject {
	float w;
	float s;
};

class Subjects {

public:
	Subjects();
	void push_subject(float w, float s);
	void delete_subject(int id);
	subject view_subject(int id);
	float calculate_gpa();
	float get_max();
	float get_min();

private:
	std::vector<subject*> vc;
	float sum_wt, maxx, minn;
};

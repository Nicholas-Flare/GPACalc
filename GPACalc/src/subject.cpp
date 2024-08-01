#include "../include/subject.h"

Subjects::Subjects()
{
	vc = std::vector<subject*>();
	sum_wt = 0.0f;
	maxx = 0.0f;
	minn = 5.0f;
}

void Subjects::push_subject(float w, float s)
{
	subject *tmp = new subject{w,s};
	vc.push_back(tmp);
	sum_wt += w * s;
	maxx = s > maxx ? s : maxx;
	minn = s < minn ? s : minn;
}

void Subjects::delete_subject(int id)
{
	
}



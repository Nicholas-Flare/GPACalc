#pragma once
#include<wx/wx.h>

struct Entry {
	wxString sems;
	wxString cls;
	double weight;
	double score;
};

Entry* CreateManualEntry(double w, double s);
bool IsMinimumAvailable(Entry e);

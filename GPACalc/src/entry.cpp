#include "../include/entry.h"

Entry* CreateManualEntry(double w, double s) {
	return new Entry{ wxT("[Manual]"), wxT("[Manual]"), w, s };
}

bool IsMinimumAvailable(Entry e) {
	bool res = e.score >= 0.0f and e.score <= 5.0f;
	res = res && (e.weight >= 0.0f);
	return res;
}

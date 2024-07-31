#pragma once

#include <vector>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/spinctrl.h>
#include <wx/clipbrd.h>

#include "entry.h"

enum {
	wxID_BTN_IMP_OK = 200,
	wxID_BTN_IMP_RF,
};

class ImportFrame : public wxDialog {

public:
	ImportFrame(wxWindow* w, std::vector<Entry*>*, wxString r_str);
private:
	wxPanel* pnl;
	wxListCtrl* ls;
	wxTextCtrl* tc;
	wxButton* btn_ok, * btn_refresh;

	wxString raw_str;
	std::vector<Entry*> res, * addr;

	void CbButtonOk(wxCommandEvent& e);
	void CbButtonRefresh(wxCommandEvent& e);
};

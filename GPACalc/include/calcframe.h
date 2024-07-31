#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/spinctrl.h>
#include <wx/aboutdlg.h>

enum {
	wxID_LIC=100,
	wxID_ABT,
	wxID_DESC
};

class CalcFrame : public wxFrame 
{
public:
	CalcFrame();
private:
	wxListCtrl* ls;
	wxStaticText* gpa, *instr, *lab1, *lab2;
	wxButton* btn_add;
	wxTextCtrl* tc_score;
	wxSpinCtrlDouble* tc_weight;
	wxPanel *pnl;

	double sum_score, sum_weight;
	double max_score, min_score;
	int count,sel;

	void add_entry(wxCommandEvent& e);
	void drop_entry(wxKeyEvent& e);
	void OnFocused(wxListEvent& e);

	void OpenLicenses(wxCommandEvent& e);
	void OpenAbout(wxCommandEvent& e);
	void CloseApp(wxCommandEvent& e);
};


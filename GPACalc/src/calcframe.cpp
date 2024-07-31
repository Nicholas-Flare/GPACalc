#include "../include/calcframe.h"

CalcFrame::CalcFrame()
	:wxFrame(nullptr, wxID_ANY, "GPA Calculator")
{

	this->SetSize(800, 600);

	wxMenu* m_misc = new wxMenu;
	m_misc->Append(wxID_LIC, "Licenses", "Love for open-source libs and knowledge <3");
	m_misc->Append(wxID_ABT, "About", "Get to know the creator - Chow-chow (@Nicholas-Flare)");
	m_misc->AppendSeparator();
	m_misc->Append(wxID_EXIT);

	wxMenuBar* mb = new wxMenuBar;
	mb->Append(m_misc, "Misc.");
	SetMenuBar(mb);

	sum_score = 0.0f;
	sum_weight = 0.0f;
	count = 0, sel = -1;
	max_score = 0.0f;
	min_score = 5.0f;

	CreateStatusBar();
	SetStatusText("Ready. Count 0 Item(s).");

	pnl = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	ls = new wxListCtrl(pnl, wxID_ANY, wxDefaultPosition, wxDefaultSize,wxLC_REPORT);
	ls->AppendColumn("Weight", wxLIST_FORMAT_CENTER);
	ls->SetColumnWidth(0, 125);
	ls->AppendColumn("Score (5.0)", wxLIST_FORMAT_CENTER);
	ls->SetColumnWidth(1, 275);
	tc_weight = new wxSpinCtrlDouble(pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, 25), 16384L, 0.f, 15.f, 2.f, 0.01f);
	tc_score = new wxTextCtrl(pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, 25), wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_NUMERIC));
	btn_add = new wxButton(pnl, wxID_ANY, "ADD ENTRY", wxDefaultPosition, wxDefaultSize, 0);

	gpa = new wxStaticText(pnl, wxID_ANY, "GPA=0.0000", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_VERTICAL);
	instr = new wxStaticText(pnl, wxID_ANY, "Select an entry and press \"DELETE\" to drop the selected entry.");
	lab1 = new wxStaticText(pnl, wxID_ANY, "Weight:");
	lab2 = new wxStaticText(pnl, wxID_ANY, "Score:");

	wxBoxSizer* main_box = new wxBoxSizer(wxVERTICAL);
	
	main_box->Add(ls, wxSizerFlags().Expand().Proportion(3));
	main_box->AddSpacer(3);

	wxBoxSizer* opt_box = new wxBoxSizer(wxVERTICAL);
	main_box->Add(opt_box, wxSizerFlags().Expand().Proportion(2));

	opt_box->Add(gpa, wxSizerFlags().Expand().Proportion(4));
	opt_box->AddSpacer(20);

	wxBoxSizer* vals_box = new wxBoxSizer(wxHORIZONTAL);
	opt_box->Add(vals_box,wxSizerFlags().Expand());
	vals_box->Add(lab1, wxSizerFlags().Proportion(1));
	vals_box->Add(tc_weight, wxSizerFlags().Expand().Proportion(2));
	vals_box->AddSpacer(10);
	vals_box->Add(lab2, wxSizerFlags().Proportion(1));
	vals_box->Add(tc_score, wxSizerFlags().Expand().Proportion(4));

	opt_box->AddSpacer(10);
	opt_box->Add(btn_add, wxSizerFlags().Expand());
	opt_box->AddSpacer(10);
	opt_box->Add(instr);

	wxGridSizer* gd = new wxGridSizer(1, 1, 5, 5);
	gd->Add(main_box, wxSizerFlags().Expand().Border(wxALL, 25));

	pnl->SetSizer(gd);
	gd->SetSizeHints(this);

	Bind(wxEVT_BUTTON, &CalcFrame::add_entry, this, wxID_ANY);
	pnl->Bind(wxEVT_CHAR_HOOK, &CalcFrame::drop_entry, this, wxID_ANY);
	Bind(wxEVT_LIST_ITEM_SELECTED, &CalcFrame::OnFocused, this, wxID_ANY);
	tc_score->Bind(wxEVT_TEXT_ENTER, &CalcFrame::add_entry, this, wxID_ANY);
	Bind(wxEVT_MENU, &CalcFrame::OpenAbout, this, wxID_ABT);
	Bind(wxEVT_MENU, &CalcFrame::OpenLicenses, this, wxID_LIC);
	Bind(wxEVT_MENU, &CalcFrame::CloseApp, this, wxID_EXIT);
}

void CalcFrame::add_entry(wxCommandEvent& e)
{

	if (tc_score->IsEmpty()) return;

	double s = 0.0f, w=0.0f;
	tc_score->GetValue().ToDouble(&s);
	w = tc_weight->GetValue();
	sum_score += s * w;
	sum_weight += w;
	double gpa_res = sum_score / sum_weight;
	count += 1;
	ls->InsertItem(0, wxString::Format("%.2f", w));
	ls->SetItem(0, 1, wxString::Format("%.2f", s));

	SetStatusText(wxString::Format("Ready. Count %d Item(s).", count));
	gpa->SetLabel(wxString::Format("GPA=%.4f", gpa_res));

}

void CalcFrame::drop_entry(wxKeyEvent& e) {

	int kc = e.GetKeyCode();
	if (kc != WXK_DELETE || sel < 0) e.Skip();
	else {
		double s = 0.0f, w = 0.0f;

		ls->GetItemText(sel, 0).ToDouble(&w);
		ls->GetItemText(sel, 1).ToDouble(&s);

		ls->DeleteItem(sel);
		count -= 1;
		sum_score -= w * s;
		sum_weight -= w;
		
		double gpa_res = 0.0f;
		if (sum_weight) gpa_res = sum_score / sum_weight;
		
		gpa->SetLabel(wxString::Format("GPA=%.4f", gpa_res));
		SetStatusText(wxString::Format("Ready. Count %d Item(s).", count));
	}

}

void CalcFrame::OnFocused(wxListEvent& e)
{

	sel = e.GetIndex();
	SetStatusText(wxString::Format("Selecting row %d", sel + 1));

}

void CalcFrame::OpenLicenses(wxCommandEvent& e)
{
	wxAboutDialogInfo aboutInfo;
	aboutInfo.SetName("GPACalc");
	aboutInfo.SetVersion("1.0.1");
	aboutInfo.SetDescription(_("This application uses WxWidgets. Visit the link below to see License."));
	aboutInfo.SetWebSite("https://wxwidgets.org/about/licence/");

	wxAboutBox(aboutInfo);
}

void CalcFrame::OpenAbout(wxCommandEvent& e)
{

	wxAboutDialogInfo aboutInfo;
	aboutInfo.SetName("GPACalc");
	aboutInfo.SetVersion("1.0.1");
	aboutInfo.SetDescription(_("A simple WxWidgets-based calculator developed with NEU GPA system in mind :D\nGPA calculated based on announcement below."));
	aboutInfo.SetCopyright("2024 (C)");
	aboutInfo.SetWebSite("http://aao.neu.edu.cn/2019/0318/c2340a86970/pagem.htm");
	aboutInfo.AddDeveloper("@Nicholas-Flare (Chow-chow)");

	wxAboutBox(aboutInfo);

}

void CalcFrame::CloseApp(wxCommandEvent& e)
{
	Close(true);
}

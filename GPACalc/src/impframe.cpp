#include "../include/impframe.h"

ImportFrame::ImportFrame(wxWindow* w, std::vector<Entry*>* v, wxString r_str)
	: wxDialog(w, wxID_ANY, "Import from Clipboard...")
{

	this->raw_str = wxString();

	res = std::vector<Entry*>();
	addr = v;

	pnl = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	//List
	ls = new wxListCtrl(pnl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	ls->AppendColumn("Semester", wxLIST_FORMAT_CENTER);
	ls->AppendColumn("Class", wxLIST_FORMAT_CENTER);
	ls->AppendColumn("Weight", wxLIST_FORMAT_CENTER);
	ls->AppendColumn("Score", wxLIST_FORMAT_CENTER);

	tc = new wxTextCtrl(pnl, wxID_ANY, raw_str, wxDefaultPosition, wxDefaultSize, wxTC_MULTILINE);
	tc->SetEditable(false);

	btn_ok = new wxButton(pnl, wxID_BTN_IMP_OK, "CONFIRM AND IMPORT");
	btn_refresh = new wxButton(pnl, wxID_BTN_IMP_RF, "RE-OBTAIN CLIPBOARD");

	wxBoxSizer* main_bx = new wxBoxSizer(wxVERTICAL);
	main_bx->Add(ls, wxSizerFlags().Expand().Proportion(8));
	main_bx->AddSpacer(10);
	main_bx->Add(btn_ok, wxSizerFlags().Expand().Proportion(1));
	main_bx->AddSpacer(3);
	main_bx->Add(btn_refresh, wxSizerFlags().Expand().Proportion(1));
	main_bx->AddSpacer(10);
	main_bx->Add(tc, wxSizerFlags().Expand().Proportion(8));

	wxGridSizer* gs = new wxGridSizer(1, 1, 10, 10);
	gs->Add(main_bx, wxSizerFlags().Expand().Border(wxALL, 25));
	pnl->SetSizer(gs);
	gs->SetSizeHints(this);

	Bind(wxEVT_BUTTON, &ImportFrame::CbButtonRefresh, this, wxID_BTN_IMP_RF);
	Bind(wxEVT_BUTTON, &ImportFrame::CbButtonOk, this, wxID_BTN_IMP_OK);

}

void ImportFrame::CbButtonOk(wxCommandEvent& e)
{

	for (auto i : res) {
		addr->push_back(i);
	}
	this->Close();

}

void ImportFrame::CbButtonRefresh(wxCommandEvent& e)
{

	ls->DeleteAllItems();

	wxTheClipboard->Open();
	wxTextDataObject dt;
	wxTheClipboard->GetData(dt);

	auto str_v = std::vector<wxString>();

	this->raw_str = wxString(dt.GetText());
	raw_str.Replace(" ", "");
	raw_str.Replace("\t", " ");

	long long len = raw_str.Len();
	int prev = 0;
	for (int i = 0; i < len; i++) {
		wxUniChar chr = raw_str.GetChar(i);
		if (chr == wxUniChar('\n') or chr == wxUniChar(' ')) {
			wxString tmp = raw_str.SubString(prev, i);
			str_v.push_back(tmp);
			prev = i + 1;
		}
	}

	if ((str_v.size() + 1) % 14 != 0)
		wxLogWarning(wxString::Format("The string obtained seems not to fit the proper format."
		"You should inspect the content of clipboard in the Text Field below. The content has %d cells.", (int)(str_v.size())));

	for (int i = 0; (i + 12) < str_v.size(); i += 14) {

		Entry* tmp = new Entry{};
		tmp->sems = str_v[i + 0];
		tmp->cls = str_v[i + 3];
		str_v[i + 6].ToDouble(&(tmp->weight));
		str_v[i + 12].ToDouble(&(tmp->score));

		res.push_back(tmp);

		int cur = ls->GetItemCount();
		ls->InsertItem(cur, tmp->sems);
		ls->SetItem(cur, 1, tmp->cls);
		ls->SetItem(cur, 2, wxString::Format("%.2f", tmp->weight));
		ls->SetItem(cur, 3, wxString::Format("%.2f", tmp->score));

	}

}

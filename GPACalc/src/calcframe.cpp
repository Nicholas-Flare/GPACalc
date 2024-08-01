#include "../include/calcframe.h"

CalcFrame::CalcFrame()
	:wxFrame(nullptr, wxID_ANY, "GPACalc")
{

	p = fs::path();
	is_ready = false;
	sel = -1;

	PopulateFrame();
	PopulateMenu();
	PopulateSizers();
	BindElements();

	this->SetTitle("GPACalc - No File Opened.");
	DisableElements();
	
}

void CalcFrame::PopulateMenu()
{

	m_misc = new wxMenu;
	m_misc->Append(wxID_M_LICENSE, "&Licenses", "Love for open-source libs and knowledge <3");
	m_misc->Append(wxID_M_ABOUT, "&About", "Get to know the creator - @Nicholas-Flare");

	m_recs = new wxMenu;
	m_recs->Append(wxID_M_NEW, "&New File", "Create a new data file to store your scores.");
	m_recs->Append(wxID_M_LOAD, "&Load File", "Load a previous data file.");
	m_recs->Append(wxID_M_SAVE, "&Save File", "Save to somewhere for future use.");
	m_recs->Append(wxID_M_SEARCH, "Search &Record", "Search for specific records.");

	mb = new wxMenuBar;
	mb->Append(m_recs, "&Recs");
	mb->Append(m_misc, "&Misc");
	SetMenuBar(mb);

}

void CalcFrame::PopulateFrame()
{

	pnl = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	//List
	ls = new wxListCtrl(pnl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	ls->AppendColumn("Semester", wxLIST_FORMAT_CENTER);
	ls->AppendColumn("Class", wxLIST_FORMAT_CENTER);
	ls->AppendColumn("Weight", wxLIST_FORMAT_CENTER);
	ls->AppendColumn("Score", wxLIST_FORMAT_CENTER);
	ls->SetColumnWidth(0, 100);
	ls->SetColumnWidth(1, 125);
	ls->SetColumnWidth(2, 70);
	ls->SetColumnWidth(3, 70);

	tc_weight = new wxSpinCtrlDouble(pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, 25), 16384L, 0.f, 15.f, 2.f, 0.01f);
	tc_score = new wxTextCtrl(pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, 25), wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_NUMERIC));
	btn_add = new wxButton(pnl, wxID_BTN_ADD, "ADD ENTRY");
	btn_clip_impt = new wxButton(pnl, wxID_BTN_IMPORT, "IMPORT FROM CLIPBOARD");
	btn_del = new wxButton(pnl, wxID_BTN_DEL, "DROP ENTRY");

	gpa = new wxStaticText(pnl, wxID_ANY, "GPA=0.0000", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_VERTICAL);
	lab1 = new wxStaticText(pnl, wxID_ANY, "Weight:");
	lab2 = new wxStaticText(pnl, wxID_ANY, "Score:");

}

void CalcFrame::PopulateSizers()
{

	wxBoxSizer* main_box = new wxBoxSizer(wxVERTICAL);

	main_box->Add(ls, wxSizerFlags().Expand().Proportion(3));
	main_box->AddSpacer(3);

	wxBoxSizer* opt_box = new wxBoxSizer(wxVERTICAL);
	main_box->Add(opt_box, wxSizerFlags().Expand().Proportion(2));

	opt_box->Add(gpa, wxSizerFlags().Expand().Proportion(4));
	opt_box->AddSpacer(20);

	wxBoxSizer* vals_box = new wxBoxSizer(wxHORIZONTAL);
	opt_box->Add(vals_box, wxSizerFlags().Expand());
	vals_box->Add(lab1, wxSizerFlags().Proportion(1));
	vals_box->Add(tc_weight, wxSizerFlags().Expand().Proportion(2));
	vals_box->AddSpacer(10);
	vals_box->Add(lab2, wxSizerFlags().Proportion(1));
	vals_box->Add(tc_score, wxSizerFlags().Expand().Proportion(4));

	opt_box->AddSpacer(10);
	opt_box->Add(btn_add, wxSizerFlags().Expand());
	opt_box->Add(btn_clip_impt, wxSizerFlags().Expand());
	opt_box->Add(btn_del, wxSizerFlags().Expand());
	opt_box->AddSpacer(10);

	wxGridSizer* gd = new wxGridSizer(1, 1, 5, 5);
	gd->Add(main_box, wxSizerFlags().Expand().Border(wxALL, 25));

	pnl->SetSizer(gd);
	gd->SetSizeHints(this);

}

void CalcFrame::BindElements()
{

	Bind(wxEVT_CLOSE_WINDOW, &CalcFrame::CbOnClose, this, wxID_ANY);
	Bind(wxEVT_MENU, &CalcFrame::CbNewFile, this, wxID_M_NEW);
	Bind(wxEVT_MENU, &CalcFrame::CbSaveFile, this, wxID_M_SAVE);
	Bind(wxEVT_BUTTON, &CalcFrame::CbButtonAdd, this, wxID_BTN_ADD);
	Bind(wxEVT_MENU, &CalcFrame::CbLoadFile, this, wxID_M_LOAD);
	Bind(wxEVT_BUTTON, &CalcFrame::CbButtonImport, this, wxID_BTN_IMPORT);
	Bind(wxEVT_BUTTON, &CalcFrame::CbButtonDelete, this, wxID_BTN_DEL);
	Bind(wxEVT_LIST_ITEM_FOCUSED, &CalcFrame::CbOnFocus, this, wxID_ANY);
	Bind(wxEVT_MENU, &CalcFrame::CbAbout, this, wxID_M_ABOUT);
	Bind(wxEVT_MENU, &CalcFrame::CbLicense, this, wxID_M_LICENSE);
	Bind(wxEVT_MENU, &CalcFrame::CbSearchEntry, this, wxID_M_SEARCH);

}

void CalcFrame::DisableElements()
{

	ls->Disable();
	btn_add->Disable();
	btn_clip_impt->Disable();
	btn_del->Disable();
	tc_score->Disable();
	tc_weight->Disable();

}

void CalcFrame::EnableElements()
{

	ls->Enable();
	btn_add->Enable();
	btn_clip_impt->Enable();
	btn_del->Enable();
	tc_score->Enable();
	tc_weight->Enable();

}

void CalcFrame::UpdateStatistics()
{

	sqlite3_stmt* stm = nullptr;
	double gpa_res = GetGPA();
	gpa->SetLabel(wxString::Format("GPA=%.4f", gpa_res));

}

bool CalcFrame::CreateNew()
{
	
	Close();
	int res = sqlite3_open(":memory:", &pdb);

	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to create an in-memory database.\n Error code = %04x", res));
		return false;
	}

	const char* sql_create_table = "CREATE TABLE entries(semester TEXT, class TEXT, weight DOUBLE, score DOUBLE)";
	res = sqlite3_exec(pdb, sql_create_table, NULL, NULL, NULL);

	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to create a table.\n Error code = %04x", res));
		return false;
	}

	this->p.clear();
	return true;

}

bool CalcFrame::Load(const fs::path p)
{
	if (!fs::exists(p)) return false;
	DisableElements();

	sqlite3* db_temp = nullptr;
	int res = sqlite3_open(wxString(p.generic_string().c_str()).c_str(), &db_temp); /// If encoding fails, then it it here.
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to open an database.\n Error code = %04x\n Path = \"%s\"", res, p.generic_u8string().c_str()));
		return false;
	}
	
	Close();
	this->p = fs::path(p);
	pdb = db_temp;
	is_ready = true;
	EnableElements();
	
}

bool CalcFrame::Save(const fs::path p)
{
	if (!(this->p.empty())) return true;
	
	sqlite3* db_temp = nullptr;
	int res = sqlite3_open(p.generic_string().c_str(), &db_temp);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to save an database.\n Error code = %04x\n Path = \"%s\"", res, p.generic_u8string().c_str()));
		if (!db_temp) {
			sqlite3_close(db_temp);
			db_temp = NULL;
		}
		return false;
	}

	sqlite3_backup* bak = sqlite3_backup_init(db_temp, "main", pdb, "main");
	if (!bak) {
		wxLogError(wxString::Format("Error occurs while trying to copy an database to the disk.\n Error code = %04x\n Path = \"%s\"", res, p.generic_u8string().c_str()));
		if (!db_temp) {
			sqlite3_close(db_temp);
			db_temp = NULL;
		}
		return false;
	}
	sqlite3_backup_step(bak, -1);
	sqlite3_backup_finish(bak);

	res = sqlite3_errcode(db_temp);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to write an database to the disk.\n Error code = %04x\n Path = \"%s\"", res, p.generic_u8string().c_str()));
		if (!db_temp) {
			sqlite3_close(db_temp);
			db_temp = NULL;
		}
		return false;
	}

	sqlite3_close(pdb);
	pdb = db_temp;
	return true;

}

bool CalcFrame::Verify()
{

	//Test 1: Is a database? Yes, because there is a connection.
	if (!pdb) {
		wxLogError(wxString::Format("Fucking bug."));
		return false;
	}
	//Test 2: Is there a table named "entries"?
	sqlite3_stmt* stm_t2 = nullptr;
	wxString sql_t2("SELECT COUNT(*) FROM sqlite_master WHERE type=='table' AND name=='entries'");
	int res = sqlite3_prepare_v2(pdb, sql_t2.ToUTF8(), -1, &stm_t2, nullptr);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Fucking bug.\n%04x", res));
		return false;
	}
	sqlite3_step(stm_t2);
	int count = sqlite3_column_int(stm_t2, 0);
	sqlite3_finalize(stm_t2);
	if (count != 1) {
		wxLogError(wxString::Format("Error occurs while trying to load the datasheet (Test 2). Maybe you've picked a wrong file?"));
		return false;
	}

	//Test 3: Is the table constructed properly?
	sqlite3_stmt* stm_t3 = nullptr;
	wxString sql_t3("SELECT * FROM entries");
	sqlite3_prepare_v2(pdb, sql_t3.ToUTF8(), -1, &stm_t3, nullptr);
	sqlite3_step(stm_t3);
	if (sqlite3_column_type(stm_t3, 0) == SQLITE_TEXT and \
		sqlite3_column_type(stm_t3, 1) == SQLITE_TEXT and \
		sqlite3_column_type(stm_t3, 2) == SQLITE_FLOAT and \
		sqlite3_column_type(stm_t3, 3) == SQLITE_FLOAT) 
	{
		sqlite3_finalize(stm_t3);
		return true;
	}
	sqlite3_finalize(stm_t3);
	wxLogError(wxString::Format("Error occurs while trying to load the datasheet (Test 3). Maybe you've picked a wrong file?"));
	return false;

}

bool CalcFrame::Close()
{
	if (pdb) {
		sqlite3_close(pdb);
		pdb = NULL;
		is_ready = false;
		DisableElements();
		ls->DeleteAllItems();
		return true;
	}
	is_ready = false;
	DisableElements();
	ls->DeleteAllItems();
	return false;
}

bool CalcFrame::PushEntry(const Entry e)
{
	if (!IsMinimumAvailable(e)) return false;

	sqlite3_stmt* stm;
	wxString sql_add_entry("INSERT INTO entries VALUES(?,?,?,?)");
	int res = sqlite3_prepare_v2(pdb, sql_add_entry.ToUTF8(), -1, &stm, NULL);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to prepare a SQLite Statement.\n Error code = %04x", res));
		sqlite3_finalize(stm);
		return false;
	}

	res = sqlite3_bind_text64(stm, 1, e.sems.ToUTF8(), -1, SQLITE_STATIC, SQLITE_UTF8);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to prepare a SQLite Statement while binding \"semester\".\n Error code = %04x", res));
		sqlite3_finalize(stm);
		return false;
	}

	res = sqlite3_bind_text64(stm, 2, e.cls.ToUTF8(), -1, SQLITE_STATIC, SQLITE_UTF8);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to prepare a SQLite Statement while binding \"class\".\n Error code = %04x", res));
		sqlite3_finalize(stm);
		return false;
	}

	res = sqlite3_bind_double(stm, 3, e.weight);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to prepare a SQLite Statement while binding \"weight\".\n Error code = %04x", res));
		sqlite3_finalize(stm);
		return false;
	}

	res = sqlite3_bind_double(stm, 4, e.score);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to prepare a SQLite Statement while binding \"score\".\n Error code = %04x", res));
		sqlite3_finalize(stm);
		return false;
	}

	res = sqlite3_step(stm);
	if (res != SQLITE_DONE) {
		wxLogError(wxString::Format("Error occurs while trying to execute a SQLite Statement.\n Error code = %04x", res));
		sqlite3_finalize(stm);
		return false;
	}

	return true;

}

std::vector<Entry*> CalcFrame::RetrieveAllEntries()
{
	
	sqlite3_stmt* stm_all = nullptr;
	wxString sql_all("SELECT * FROM entries");
	int res = sqlite3_prepare_v2(pdb, sql_all.ToUTF8(), -1, &stm_all, nullptr);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to prepare a SQLite Statement.\n Error code = %04x", res));
		sqlite3_finalize(stm_all);
		return std::vector<Entry*>();
	}

	std::vector<Entry*> entries = std::vector<Entry*>();
	res = sqlite3_step(stm_all);
	while (res != SQLITE_DONE) {
		Entry* tmp = nullptr;
		switch (res) {
		case SQLITE_ROW:
			tmp = new Entry{
				wxString::FromUTF8((const char*)(sqlite3_column_text(stm_all, 0))),
				wxString::FromUTF8((const char*)(sqlite3_column_text(stm_all, 1))),
				sqlite3_column_double(stm_all,2),
				sqlite3_column_double(stm_all,3)
			};
			entries.push_back(tmp);
			break;
		default:
			wxLogError(wxString::Format("Error occurs while trying to retrieve data from a SQLite Statement.\n Error code = %04x", res));
			sqlite3_finalize(stm_all);
			return entries;
		}
		res = sqlite3_step(stm_all);
	}

	sqlite3_finalize(stm_all);
	return entries;

}

bool CalcFrame::DropEntry(const Entry e)
{
	// This function always assume Entry exists. Should only be called internally.

	sqlite3_stmt* stm;
	wxString sql_add_entry("DELETE FROM entries WHERE semester==? AND class==? AND weight==? AND score==?");
	int res = sqlite3_prepare_v2(pdb, sql_add_entry.ToUTF8(), -1, &stm, NULL);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to prepare a SQLite Statement.\n Error code = %04x", res));
		sqlite3_finalize(stm);
		return false;
	}

	res = sqlite3_bind_text64(stm, 1, e.sems.ToUTF8(), -1, SQLITE_STATIC, SQLITE_UTF8);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to prepare a SQLite Statement while binding \"semester\".\n Error code = %04x", res));
		sqlite3_finalize(stm);
		return false;
	}

	res = sqlite3_bind_text64(stm, 2, e.cls.ToUTF8(), -1, SQLITE_STATIC, SQLITE_UTF8);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to prepare a SQLite Statement while binding \"class\".\n Error code = %04x", res));
		sqlite3_finalize(stm);
		return false;
	}
	
	res = sqlite3_bind_double(stm, 3, e.weight);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to prepare a SQLite Statement while binding \"weight\".\n Error code = %04x", res));
		sqlite3_finalize(stm);
		return false;
	}
	
	res = sqlite3_bind_double(stm, 4, e.score);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to prepare a SQLite Statement while binding \"score\".\n Error code = %04x", res));
		sqlite3_finalize(stm);
		return false;
	}

	res = sqlite3_step(stm);
	if (res != SQLITE_DONE) {
		wxLogError(wxString::Format("Error occurs while trying to execute a SQLite Statement.\n Error code = %04x", res));
		sqlite3_finalize(stm);
		return false;
	}

	sqlite3_finalize(stm);
	return true;
}



bool CalcFrame::DropEntryByIndex(int index)
{

	sqlite3_stmt* stm_all = nullptr;
	wxString sql_all("SELECT rowid FROM entries LIMIT ?,1");
	int res = sqlite3_prepare_v2(pdb, sql_all.ToUTF8(), -1, &stm_all, nullptr);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to prepare a SQLite Statement.\n Error code = %04x", res));
		sqlite3_finalize(stm_all);
		return false;
	}

	res = sqlite3_bind_int(stm_all, 1, index);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to prepare a SQLite Statement while binding \"index\".\n Error code = %04x", res));
		sqlite3_finalize(stm_all);
		return false;
	}

	res = sqlite3_step(stm_all);
	if (res != SQLITE_ROW and res != SQLITE_DONE) {
		wxLogError(wxString::Format("Error occurs while trying to execute a SQLite Statement.\n Error code = %04x", res));
		sqlite3_finalize(stm_all);
		return false;
	}

	int rid = sqlite3_column_int(stm_all, 0);
	sqlite3_finalize(stm_all);

	sqlite3_stmt* stm = nullptr;
	wxString sql_add_entry("DELETE FROM entries WHERE rowid==?");
	res = sqlite3_prepare_v2(pdb, sql_add_entry.ToUTF8(), -1, &stm, NULL);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to prepare a SQLite Statement.\n Error code = %04x", res));
		sqlite3_finalize(stm);
		return false;
	}

	res = sqlite3_bind_int(stm, 1, rid);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to prepare a SQLite Statement while binding \"rowid\".\n Error code = %04x", res));
		sqlite3_finalize(stm);
		return false;
	}

	res = sqlite3_step(stm);
	if (res != SQLITE_DONE) {
		wxLogError(wxString::Format("Error occurs while trying to execute a SQLite Statement.\n Error code = %04x", res));
		sqlite3_finalize(stm);
		return false;
	}

	sqlite3_finalize(stm);
	return true;
}

double CalcFrame::GetGPA()
{
	sqlite3_stmt* stm_count = nullptr;
	wxString sql_count("SELECT COUNT(*) FROM entries");
	int res = sqlite3_prepare_v2(pdb, sql_count.ToUTF8(), -1, &stm_count, nullptr);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to prepare a SQLite Statement for count.\n Error code = %04x", res));
		sqlite3_finalize(stm_count);
		return 0.0;
	}

	res = sqlite3_step(stm_count);
	if (res != SQLITE_DONE and res != SQLITE_ROW) {
		wxLogError(wxString::Format("Error occurs while trying to execute a SQLite Statement for count.\n Error code = %04x", res));
		sqlite3_finalize(stm_count);
		return 0.0;
	}

	int count = sqlite3_column_int(stm_count, 0);
	if (count == 0) return 0.0f;

	sqlite3_stmt* stm_gpa = nullptr;
	wxString sql_gpa("SELECT SUM(weight * score) / SUM(weight) FROM entries");
	res = sqlite3_prepare_v2(pdb, sql_gpa.ToUTF8(), -1, &stm_gpa, nullptr);
	if (res != SQLITE_OK) {
		wxLogError(wxString::Format("Error occurs while trying to prepare a SQLite Statement.\n Error code = %04x", res));
		sqlite3_finalize(stm_gpa);
		return 0.0;
	}

	res = sqlite3_step(stm_gpa);
	if (res != SQLITE_DONE and res != SQLITE_ROW) {
		wxLogError(wxString::Format("Error occurs while trying to execute a SQLite Statement.\n Error code = %04x", res));
		sqlite3_finalize(stm_gpa);
		return 0.0;
	}

	double gpa_res = sqlite3_column_double(stm_gpa, 0);
	return gpa_res;

}

void CalcFrame::CbNewFile(wxCommandEvent& e)
{

	is_ready = CreateNew();
	if (is_ready) {
		EnableElements();
		this->SetTitle(wxString::Format("GPACalc - [Unsaved File]"));
	}

}

void CalcFrame::CbLoadFile(wxCommandEvent& e)
{

	if (is_ready and p.empty()) { 
		wxMessageDialog wmb(
			this,
			wxT("Do you want to save the datasheet before proceeding?"),
			wxT("Save First?"),
			wxYES_NO | wxYES_DEFAULT);
		int should_save = wmb.ShowModal();
		if (should_save == wxID_YES) {
			while (true) {
				wxFileDialog sv_dialog(
					this, 
					wxT("Save to..."), 
					fs::current_path().generic_string().c_str(), 
					"", 
					wxT("Database File (*.db)|*.db"), 
					wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
				if (sv_dialog.ShowModal() != wxID_CANCEL) {
					bool res = Save(fs::path(sv_dialog.GetPath().ToStdWstring()));
					if (!res) {
						wxLogError(wxString::Format("Error occurs while trying to save the datasheet. Maybe you've picked a read-only place?"));
						continue;
					}
					break;
				}
				else return;
			}
		}
	}

	Close();

	while (true) {
		wxFileDialog ld_dialog(
			this,
			wxT("Load..."),
			fs::current_path().generic_string().c_str(),
			"",
			wxT("Database File (*.db)|*.db"));
		if (ld_dialog.ShowModal() != wxID_CANCEL) {
			bool res = Load(fs::path(ld_dialog.GetPath().ToStdWstring()));
			if (!res) {
				wxLogError(wxString::Format("Error occurs while trying to load the datasheet. Maybe you've picked a wrong file?"));
				break;
			}
			res = Verify();
			if (!res) {
				Close();
				break;
			}
			this->SetTitle(wxString::Format("GPACalc - %s", this->p.generic_wstring().c_str()));
			std::vector<Entry*> vc = RetrieveAllEntries();
			for (auto tmp : vc) {
				int cur = ls->GetItemCount();
				ls->InsertItem(cur, tmp->sems);
				ls->SetItem(cur, 1, tmp->cls);
				ls->SetItem(cur, 2, wxString::Format("%.2f", tmp->weight));
				ls->SetItem(cur, 3, wxString::Format("%.2f", tmp->score));
				delete tmp;
			}
			vc.clear();
			UpdateStatistics();
			break;
		}
		else {
			break;
		}
	}

}

void CalcFrame::CbSaveFile(wxCommandEvent& e)
{
	
	if (!p.empty()) return;
	wxFileDialog sv_dialog(this, wxT("Save to..."), fs::current_path().generic_string().c_str(), "", wxT("Database File (*.db)|*.db"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (sv_dialog.ShowModal() == wxID_CANCEL) return;
	
	fs::path ps = fs::path(sv_dialog.GetPath().ToStdWstring());
	int res = Save(ps);
	return;

}

void CalcFrame::CbSearchEntry(wxCommandEvent& e)
{

	if (!is_ready) return;

	wxMessageDialog dlg(this,
		wxT("Bah, yet to be implemented."),
		wxT("Coming soon..."),
		wxOK);
	dlg.ShowModal();

}

void CalcFrame::CbLicense(wxCommandEvent& e)
{

	wxAboutDialogInfo aboutInfo;
	aboutInfo.SetName("GPACalc");
	aboutInfo.SetVersion("2.0.1");
	aboutInfo.SetDescription(wxT("This application uses WxWidgets.\nVisit https://wxwidgets.org/about/licence/ below to see License.\n"
	"This application uses SQLite. SQLite is in the Public Domain.\nVisit https://sqlite.org/copyright.html below to learn more."));

	wxAboutBox(aboutInfo);
}

void CalcFrame::CbAbout(wxCommandEvent& e)
{

	wxAboutDialogInfo aboutInfo;
	aboutInfo.SetName("GPACalc");
	aboutInfo.SetVersion("2.0.1");
	aboutInfo.SetDescription(_("A simple WxWidgets-based calculator developed with NEU GPA system in mind :D\nGPA calculated based on announcement below."));
	aboutInfo.SetCopyright("2024 (C)");
	aboutInfo.SetWebSite("http://aao.neu.edu.cn/2019/0318/c2340a86970/pagem.htm");
	aboutInfo.AddDeveloper("@Nicholas-Flare");

	wxAboutBox(aboutInfo);

}

void CalcFrame::CbButtonAdd(wxCommandEvent& e)
{

	if (!is_ready) return;

	double w = 0.f, s = 0.f;
	w = tc_weight->GetValue();
	tc_score->GetValue().ToDouble(&s);

	if (s < 0) return;

	Entry* tmp = CreateManualEntry(w, s);

	if (!PushEntry(*tmp)) return;

	int cur = ls->GetItemCount();
	ls->InsertItem(cur, tmp->sems);
	ls->SetItem(cur, 1, tmp->cls);
	ls->SetItem(cur, 2, wxString::Format("%.2f",tmp->weight));
	ls->SetItem(cur, 3, wxString::Format("%.2f",tmp->score));

	UpdateStatistics();

	return;

}

void CalcFrame::CbButtonImport(wxCommandEvent& e)
{
	wxMessageDialog abt(this,
		wxT("This function is designed specifically for NEU students. By copying plain text from EAMS system and import it here, user doesn't need to manually input the data."),
		wxT("Tip"),
		wxOK);
	abt.ShowModal();

	wxTheClipboard->Open();
	wxTextDataObject dt;
	wxTheClipboard->GetData(dt);
	auto a = std::vector<Entry*>();
	ImportFrame* frm = new ImportFrame(this, &a, dt.GetText());
	frm->ShowModal();

	for (auto tmp : a) {

		PushEntry(*tmp);

		int cur = ls->GetItemCount();
		ls->InsertItem(cur, tmp->sems);
		ls->SetItem(cur, 1, tmp->cls);
		ls->SetItem(cur, 2, wxString::Format("%.2f", tmp->weight));
		ls->SetItem(cur, 3, wxString::Format("%.2f", tmp->score));
		delete tmp;

	}

	UpdateStatistics();
	a.clear();

	return;

}

void CalcFrame::CbButtonDelete(wxCommandEvent& e)
{

	if (sel < 0) return;

	if (!DropEntryByIndex(sel)) {
		return;
	}

	ls->DeleteItem(sel);
	UpdateStatistics();
	sel = -1;
	return;

}

void CalcFrame::CbOnClose(wxCloseEvent& e)
{

	wxMessageDialog* dlg = new wxMessageDialog(this, "Really close GPACalc? Any unsaved data will be lost!", "Save Before Exit?", wxYES_NO | wxNO_DEFAULT);
	int choice = dlg->ShowModal();

	if (choice == wxID_YES) {
		Close();
		e.Skip();
	}

}

void CalcFrame::CbOnFocus(wxListEvent& e)
{

	sel = e.GetIndex();

}

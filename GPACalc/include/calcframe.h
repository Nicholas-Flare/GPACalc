#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/spinctrl.h>
#include <wx/aboutdlg.h>
#include <wx/clipbrd.h>
#include <filesystem>
#include <sqlite3.h>

#include "../include/entry.h"
#include "../include/impframe.h"

namespace fs = std::filesystem;

enum {
	wxID_M_LICENSE=100,
	wxID_M_ABOUT,
	wxID_M_DESCRIP,
	wxID_M_NEW,
	wxID_M_LOAD,
	wxID_M_SAVE,
	wxID_M_SEARCH,
	wxID_BTN_IMPORT,
	wxID_BTN_ADD,
	wxID_BTN_DEL,
};

class CalcFrame : public wxFrame 
{
public:
	CalcFrame();
private:
	// Internal flags
	fs::path p;
	bool is_ready;
	sqlite3* pdb;
	int sel;

	// UI Elements
	wxMenu* m_misc, * m_recs;
	wxMenuBar* mb;
	wxListCtrl* ls;
	wxStaticText* gpa, * lab1, * lab2;
	wxButton* btn_add, * btn_clip_impt, * btn_del;
	wxTextCtrl* tc_score;
	wxSpinCtrlDouble* tc_weight;
	wxPanel* pnl;

	//UI-related Functions
	void PopulateMenu();
	void PopulateFrame();
	void PopulateSizers();
	void BindElements();
	void DisableElements();
	void EnableElements();
	void UpdateStatistics();

	//Record Manip
	
	/// <summary>
	/// Create an in-memory file.
	/// </summary>
	/// <return>Whether the operation is successful.</return>
	bool CreateNew();

	/// <summary>
	/// Load a file from storage.
	/// </summary>
	/// <param name="p">Path to the file.</param>
	/// <return>Whether the operation is successful.</return>
	bool Load(const fs::path p);

	/// <summary>
	/// Save a file to storage,
	/// </summary>
	/// <param name="p">Path to the location.</param>
	/// <return>Whether the operation is successful.</return>
	bool Save(const fs::path p);

	/// <summary>
	/// Verify whether the current database is a datasheet.
	/// </summary>
	/// <returns>Result of verification.</returns>
	bool Verify();

	/// <summary>
	/// Close the current file.
	/// </summary>
	/// <returns>Whether the operation is successful.</returns>
	bool Close();

	/// <summary>
	/// Add a entry to the database.
	/// </summary>
	/// <param name="e">Entry structure to be added.</param>
	/// <return>Whether the operation is successful.</return>
	bool PushEntry(const Entry e);

	/// <summary>
	/// Retrieve all entries from the database.
	/// </summary>
	/// <returns>Retrieved entry.</returns>
	std::vector<Entry*> RetrieveAllEntries();

	/// <summary>
	/// Drop an entry.
	/// </summary>
	/// <param name="e">The Entry to be dropped.</param>
	/// <return>Whether the operation is successful.</return>
	bool DropEntry(const Entry e);

	/// <summary>
	/// Drop an entry according to its index.
	/// </summary>
	/// <param name="e">The index of the entry to be dropped.</param>
	/// <return>Whether the operation is successful.</return>
	bool DropEntryByIndex(int index);

	/// <summary>
	/// Return the calculated GPA (Weighted Average). 
	/// </summary>
	/// <returns>Calculated GPA.</returns>
	double GetGPA();

	// Callback for UI Elements

	void CbNewFile(wxCommandEvent& e);
	void CbLoadFile(wxCommandEvent& e);
	void CbSaveFile(wxCommandEvent& e);
	void CbSearchEntry(wxCommandEvent& e);
	void CbLicense(wxCommandEvent& e);
	void CbAbout(wxCommandEvent& e);
	void CbButtonAdd(wxCommandEvent& e);
	void CbButtonImport(wxCommandEvent& e);
	void CbButtonDelete(wxCommandEvent& e);
	void CbOnClose(wxCloseEvent& e);
	void CbOnFocus(wxListEvent& e);

};


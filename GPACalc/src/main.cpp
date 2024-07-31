#include"../include/main.h"

bool Main::OnInit() {

	CalcFrame* frm = new CalcFrame();
	frm->Show(true);
	return true;

}

wxIMPLEMENT_APP(Main);

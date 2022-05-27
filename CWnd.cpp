#include "CWnd.h"

bool CWnd::Save(HWND hWnd,LPSTR lpszFilter, LPSTR lpszTitle,LPSTR lpszExt)
{
	static OPENFILENAME  ofn;   
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = (lpszFilter);
	ofn.lpstrFile = szBufferOut;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = (lpszTitle);
	ofn.Flags = OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY;
	ofn.lpstrDefExt = TEXT(lpszExt);
	if(!GetSaveFileName (&ofn))
	 return false;

	return true;
}
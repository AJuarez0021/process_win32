#include "CListView.h"



void CListView::AddColumn(LPSTR lpszTexto, int nAncho, int nIndice){
	lvCol.cchTextMax = 260;
	lvCol.pszText = lpszTexto;
	lvCol.mask = LVCF_TEXT|LVCF_WIDTH;
	lvCol.cx = nAncho;
	ListView_InsertColumn(hWndListview, nIndice, &lvCol);
}


void CListView::DrawGrid()
{
	SendMessage(hWndListview, LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
}


void CListView::AddItem(LPSTR lpszTexto, int nIndice, int nIndiceImagen)
{
	lstrcpy(szbuffer, lpszTexto);  
	lstrcat(szbuffer, "\0");
	lvItem.pszText = szbuffer;
	lvItem.cchTextMax = sizeof(szbuffer);
	lvItem.mask = LVIF_TEXT|LVIF_IMAGE; 
	lvItem.iItem = nIndice; 
	lvItem.iImage = nIndiceImagen;
	ListView_InsertItem(hWndListview, &lvItem);
	ListView_Update(hWndListview, nIndice);
}


void CListView::AddSubItem(int nIndiceItem, int nIndiceSubItem, LPSTR lpszTexto)
{
	lstrcpy(szbuffer, lpszTexto);  
	lstrcat(szbuffer, "\0");
	LV_ITEM lv;
	lv.iSubItem = nIndiceSubItem;
	lv.pszText = szbuffer;
	SendMessage(hWndListview, LVM_SETITEMTEXT, (WPARAM) (int) nIndiceItem, (LPARAM) (LV_ITEM FAR *) &lv);
}

void CListView::Create(HWND hWnd)
{
 GetClientRect(hWnd, &Posicion);			
 hWndListview = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, "", LV_REPORTE, 15, 30, Posicion.right - 29, Posicion.bottom - 45, hWnd, NULL, hInstanciaC, NULL);
}

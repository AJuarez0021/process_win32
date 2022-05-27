#ifndef __CLISTVIEW_H
#define __CLISTVIEW_H

#include <windows.h>
#include <commctrl.h> 

const LV_NORMAL    = WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_SHOWSELALWAYS|LVS_SINGLESEL;
const LV_ICONO     = LV_NORMAL|LVS_ICON;
const LV_ICONOSM   = LV_NORMAL|LVS_SMALLICON;
const LV_LISTA     = LV_NORMAL|LVS_LIST;
const LV_REPORTE   = LV_NORMAL|LVS_REPORT;

class CListView
{
private:	
 HWND                 hWndListview;  
 HINSTANCE            hInstanciaC;   
 RECT                 Posicion;       
 LV_COLUMN            lvCol;          
 char                 szbuffer[512];
 LV_ITEM              lvItem;         

public:
	CListView()
	{
     hWndListview=NULL;
	 InitCommonControls();
     
	}
	~CListView(){ 
	  
	}
	void DestroyListView(){
     DestroyWindow(hWndListview);
	}
	HWND GetHWND(){ return hWndListview; }
	void Create(HWND hWnd);
	void AddColumn(LPSTR lpszTexto, int nAncho, int nIndice);
	void DrawGrid();
	void AddItem(LPSTR lpszTexto, int nIndice, int nIndiceImagen);
	void AddSubItem(int nIndiceItem, int nIndiceSubItem, LPSTR lpszTexto);
	void DeleteAllItems(){ ListView_DeleteAllItems(hWndListview); }
	int  GetItem(){ return ListView_GetNextItem(hWndListview,-1,LVNI_SELECTED); }
	void DeleteItem(int iItem){
      ListView_DeleteItem(hWndListview,iItem);	
	}
};
#endif
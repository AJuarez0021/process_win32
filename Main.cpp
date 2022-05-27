//Visualizador de procesos
//Programa compilado con Visual C++ 6.0
//http://www.programacioncpp.irandohosting.0lx.net
#include <windows.h>
#include <commctrl.h>
#include "CWnd.h"
#include "resource.h"
#include "hyperlink.h"
#pragma comment(lib,"comctl32.lib")


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AboutProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
BOOL DoNotify(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void CenterWin(HWND hWnd);
CWnd w;



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstPrev, LPSTR lpszArgs, int nModo){

      HWND  hWnd;               
      MSG msg;  
      WNDCLASSEX clase;
       
      
      ZeroMemory(&msg, sizeof(MSG));
      ZeroMemory(&clase, sizeof(WNDCLASSEX));

      clase.hInstance=hInstance;  
      clase.lpszClassName="CLASE DE VENTANA"; 
      clase.lpfnWndProc=WndProc; 
      clase.style=0; 
      clase.cbSize=sizeof(WNDCLASSEX); 
      clase.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));		  
      clase.hIconSm=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1)); 
      clase.hCursor=LoadCursor(NULL,IDC_ARROW);
      clase.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
      clase.cbClsExtra=0; 
      clase.cbWndExtra=0; 
      clase.hbrBackground=(HBRUSH) (COLOR_BTNFACE+1); 
      RegisterClassEx(&clase); 
      
      hWnd=CreateWindow("CLASE DE VENTANA",  
		                TEXT("Process"),                                        
						WS_MINIMIZEBOX | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
                        CW_USEDEFAULT, CW_USEDEFAULT, HWND_DESKTOP, NULL, 
                        hInstance, NULL);
        
 
	  if( !hWnd ){ 
	   MessageBox(NULL,"Error al crear la ventana","Process",MB_OK|MB_ICONERROR);
	   return 0; 
	  }
      CenterWin(hWnd);
      ShowWindow(hWnd, nModo);
      UpdateWindow(hWnd);

	  
    
 
      while(GetMessage(&msg, NULL, 0, 0)){
                TranslateMessage(&msg);   
                DispatchMessage(&msg);  
      }
    
      return msg.wParam;  
}

void CenterWin(HWND hWnd)
{
    RECT r;	
	GetClientRect(hWnd,&r);
	int m_Width		= r.right;
	int m_Height	= r.bottom;

	RECT ScreenRect;
	GetWindowRect (GetDesktopWindow(), &ScreenRect);
	SetWindowPos (hWnd, HWND_TOP, (((ScreenRect.right + ScreenRect.left) / 2) - (m_Width / 2)),
				 (((ScreenRect.bottom + ScreenRect.top) / 2) - (m_Height / 2)),
				  m_Width, m_Height, SWP_NOCOPYBITS | SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int iItem=0;
	DWORD dwPID;


	switch(uMsg){
	  case WM_CREATE:		   
		   w.tb->CreateToolBar(hWnd,NULL);
		   w.tb->AddSeparator();
		   w.tb->AddSeparator();
		   w.tb->AddButton(ID_PROCESS_REFRESH ,STD_FILENEW,NULL);
           w.tb->AddButton(ID_PROCESS_KILL,STD_DELETE,NULL);
		   w.tb->AddSeparator();
		   w.tb->AddButtonExt(IDR_POPUP,STD_FILESAVE,NULL); 
		   w.p.v.Create(hWnd);
		   w.p.v.DrawGrid();
		   w.p.v.AddColumn("Proceso",110,0);
		   w.p.v.AddColumn("PID",75,1);
		   w.p.v.AddColumn("Prioridad Base",100,2);
		   w.p.v.AddColumn("Numero de Hilos",100,3);
		   w.p.v.AddColumn("Ruta completa",300,4);		   
		   w.p.GetProcess();
		   w.p.ViewListProcess();   		   
		   break;
	  case WM_NOTIFY: 
		   DoNotify(hWnd,uMsg,wParam,lParam);
		   switch (((LPNMHDR) lParam)->code){
             case TTN_GETDISPINFO: 
			{
			  LPTOOLTIPTEXT lpttt; 
			  lpttt = (LPTOOLTIPTEXT) lParam; 
			  lpttt->hinst = GetModuleHandle(NULL); 
			  switch(lpttt->hdr.idFrom){		
			    case ID_PROCESS_REFRESH:
					 lpttt->lpszText=TEXT("Refrescar Procesos");
					 break;
				case ID_PROCESS_KILL:
					 lpttt->lpszText=TEXT("Terminar Procesos");
					 break;
				case IDR_POPUP:
					 lpttt->lpszText=TEXT("Guardar");
					 break;
			  }
	 
			}
			break;
		   }

           break;
	  case WM_COMMAND:
		   switch(LOWORD(wParam)){
		    case ID_POPUP_TXT:
				 if(w.Save(hWnd,"Archivo .txt\0 *.txt\0","Guardar","txt")){
                  w.p.ExportToTXT(hWnd,w.GetFileName());
				 }
				 break;
			case ID_POPUP_HTML:
				 if(w.Save(hWnd,"Archivo .html\0 *.html\0","Guardar","html")){
                   w.p.ExportToHTML(hWnd,w.GetFileName());
				 }
				 break;
		    case ID_PROCESS_REFRESH:
				 w.p.UpdateListProcess(hWnd);
				 break;
			case ID_HELP_ABOUT:
				 DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT),hWnd,(DLGPROC)AboutProc);
				 break;
			case ID_PROCESS_KILL:
				{								     
				 iItem=w.p.v.GetItem();				
				 if(iItem!=-1){
				  dwPID=w.p.SearchPID(iItem);
				  if(dwPID!=-1){
				   w.p.KillProcess(dwPID);
				   w.p.v.DeleteItem(iItem);				   				 				 
				  }
				  else
				   MessageBox(hWnd,"Error: no se pudo eliminar el proceso","Process",MB_OK|MB_ICONINFORMATION);
				 }
				 else
				  MessageBox(hWnd,"No ha seleccionado ningun proceso","Process",MB_OK|MB_ICONINFORMATION);
				}
				 break;
			case ID_FILE_TXT:
				 if(w.Save(hWnd,"Archivo .txt\0 *.txt\0","Guardar","txt")){
                  w.p.ExportToTXT(hWnd,w.GetFileName());
				 }
				 break;
			case ID_FILE_HTML:
				 if(w.Save(hWnd,"Archivo .html\0 *.html\0","Guardar","html")){
                   w.p.ExportToHTML(hWnd,w.GetFileName());
				 }
				 break;
			case ID_FILE_EXIT:
				 DestroyWindow(hWnd);
				 break;
		   }
		   break;
      case WM_DESTROY:
		  
           PostQuitMessage(0);
           break;
      default:                 
           return DefWindowProc(hWnd, uMsg,  wParam, lParam);                    
     }
     return 0;
}

LRESULT CALLBACK AboutProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
 HYPCTRL hc;
 switch(uMsg)
 {
   case WM_INITDIALOG:
	    InitHypCtrl(&hc);
		hc.ulStyle	 = ulHover;
		hc.szLink	 = TEXT("http://www.programacioncpp.irandohosting.0lx.net");
		hc.szTooltip = TEXT("Visitar Pagina Web");
		CreateHypCtrl(hWnd, &hc, 20, 85, 0, 0);

	    break;
   case WM_COMMAND:
	    switch(LOWORD(wParam)){
		  case ID_ABOUT_OK:	
			   EndDialog(hWnd,0);
			   break;
		}
		break;
   case WM_DESTROY:
	    return 1;
	   
   default: 
	   return 0;
        
 }
 return DefWindowProc(hWnd, uMsg,  wParam, lParam);     
}

BOOL DoNotify(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#define lpnm   ((LPNMHDR)lParam)
#define lpnmTB ((LPNMTOOLBAR)lParam)

   RECT      rc;
   TPMPARAMS tpm;
   HMENU     hPopupMenu = NULL;
   HMENU     hMenuLoaded;
   BOOL      bRet = FALSE;

   switch(lpnm->code){
      case TBN_DROPDOWN:
         SendMessage(lpnmTB->hdr.hwndFrom, TB_GETRECT,(WPARAM)lpnmTB->iItem, (LPARAM)&rc);
         MapWindowPoints(lpnmTB->hdr.hwndFrom,HWND_DESKTOP, (LPPOINT)&rc, 2);                         
         tpm.cbSize = sizeof(TPMPARAMS);
         tpm.rcExclude = rc;
		 
         hMenuLoaded = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_POPUP)); 
         hPopupMenu = GetSubMenu(LoadMenu(GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_POPUP)),0);

         TrackPopupMenuEx(hPopupMenu,TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL,               
            rc.left, rc.bottom, hwnd, &tpm); 

		
         DestroyMenu(hMenuLoaded);			
      return (FALSE);
   }
   return FALSE;
}
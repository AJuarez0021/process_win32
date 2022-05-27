#ifndef __CWND_H
#define __CWND_H

#include "CProcess.h"
#include "CToolBar.h"

class CWnd{
private:
 char szBufferOut[MAX_PATH];
public:
 CProcess p;
 CToolBar *tb;
 CWnd(){ 
   ZeroMemory(&szBufferOut,sizeof(szBufferOut));
   tb=new CToolBar;
 }
 ~CWnd(){ 
   delete tb;

 }
 bool Save(HWND hWnd,LPSTR lpszFilter, LPSTR lpszTitle,LPSTR lpszExt);
 char *GetFileName(){ return szBufferOut; }
};

#endif
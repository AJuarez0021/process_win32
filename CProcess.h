#ifndef  __CPROCESS_H
#define  __CPROCESS_H

#include "CListView.h"
#include <string>
#include <vector>
using namespace std;

struct ListProcess
{
 string strNameProcess;
 DWORD dwPID;
 DWORD dwClassBase;
 DWORD dwThreads;
 string strFullPath;
};

class CProcess
{
private:
 string Upper(const char *strText);
public:
 CListView v;
 CProcess(){
 }
 ~CProcess(){
 }
 int GetNumProcess();
 DWORD SearchPID(string strExeFileName);
 DWORD SearchPID(int iItem);
 void ClearProcess();
 void GetProcess();
 void ViewListProcess();
 void UpdateListProcess(HWND hWnd);
 void KillProcess(DWORD ProcessID);
 void InsertProcess(const ListProcess& p);
 void ExportToTXT(HWND hWnd,string strFileName);
 void ExportToHTML(HWND hWnd,string strFileName);

};

extern string GetFileName(string strPath);
#endif
#pragma warning(disable:4786) 

#include "CProcess.h"
#include <tlhelp32.h>
#include <fstream>

vector<ListProcess> LProcess;

template <class T>
const vector<T>GetToken(string strCad,string strSeps)
{
 char *token;
 vector<T> v;
 string strTokens;
 token = strtok( (char*)strCad.c_str(), strSeps.c_str() );
 v.clear();
 while( token != NULL ){         
  strTokens=token;
  v.push_back(strTokens);  
  token = strtok( NULL, strSeps.c_str() );
 }    
 return v;
}


string GetFileName(string strPath)
{
 int iMax=0;
 vector<string> v=GetToken<string>(strPath,"\\");
 iMax=(int)v.size()-1;
 string strName=v[iMax];
 return strName;
}


string CProcess::Upper(const char *strText)
{
 string strResult="";

 for(int i=0;i<(int)strlen(strText);i++)
  strResult+=toupper(strText[i]);

 return strResult;
}


int CProcess::GetNumProcess()
{
 return LProcess.size();
}



DWORD CProcess::SearchPID(string strExeFileName)
{
 DWORD PID=-1;

 for(int i=0;i < LProcess.size(); i++){
  if(LProcess[i].strNameProcess == Upper(strExeFileName.c_str())){
   PID=LProcess[i].dwPID;
   break;
  }
 }
 return PID;
}


DWORD CProcess::SearchPID(int iItem)
{
 DWORD PID=-1;
 if(iItem > 0 && iItem < GetNumProcess())
  PID = LProcess[iItem].dwPID;
 return PID;
}
void CProcess::InsertProcess(const ListProcess& p)
{

 LProcess.push_back(p);
}

void CProcess::ClearProcess()
{
 LProcess.clear();
}


void CProcess::GetProcess()
{
 PROCESSENTRY32 pe32;
 HANDLE hSnapProcess;
 HANDLE hProcess;
 DWORD dwPriorityClass;
 ListProcess p;

 hSnapProcess=CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
 if( hSnapProcess == INVALID_HANDLE_VALUE )
  return;

 pe32.dwSize = sizeof( PROCESSENTRY32 );
 if( !Process32First( hSnapProcess, &pe32 )){
  CloseHandle(hSnapProcess);
  return;
 }

 string strExeFile="";
 do{
  hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
  
  if(hProcess!=NULL){
   dwPriorityClass = GetPriorityClass( hProcess );
   if( !dwPriorityClass ){    
    CloseHandle( hProcess );
	return;
   }
   strExeFile=Upper(pe32.szExeFile);
   p.strNameProcess=GetFileName(strExeFile); 
   p.dwPID=pe32.th32ProcessID;
   p.dwClassBase=pe32.pcPriClassBase;
   p.dwThreads=pe32.cntThreads;
   p.strFullPath=pe32.szExeFile;
   InsertProcess(p);   
   
  }
 }while( Process32Next( hSnapProcess, &pe32 ) );
  CloseHandle( hSnapProcess );
}

void CProcess::ViewListProcess()
{
 char strNumThreads[3];
 char strPID[10];
 char strClassBase[3];

 for(int i=0;i<LProcess.size();i++){
	
	sprintf(strPID,"%X",LProcess[i].dwPID);
	sprintf(strNumThreads,"%i",LProcess[i].dwThreads);
	sprintf(strClassBase,"%i",LProcess[i].dwClassBase);

    v.AddItem((char*)LProcess[i].strNameProcess.c_str(),i,0);
	v.AddSubItem(i,1,strPID);
	v.AddSubItem(i,2,strClassBase);
	v.AddSubItem(i,3,strNumThreads);
    v.AddSubItem(i,4,(char*)LProcess[i].strFullPath.c_str());
    
 }

}

void CProcess::UpdateListProcess(HWND hWnd)
{

 v.DeleteAllItems();
 ClearProcess();
 GetProcess();
 ViewListProcess();
}

void CProcess::KillProcess(DWORD ProcessID)
{
 
 HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, ProcessID);
 if(hProcess!=NULL){
  DWORD dwExitCode;
  GetExitCodeProcess(hProcess,&dwExitCode);
  TerminateProcess(hProcess,dwExitCode);
  CloseHandle(hProcess);
  MessageBox(NULL,"Se elimino el proceso","Process Viewer",MB_OK|MB_ICONINFORMATION);
 }
 else
  MessageBox(NULL,"Error no se pudo eliminar el proceso","Process Viewer",MB_OK|MB_ICONERROR);

  
}

void CProcess::ExportToTXT(HWND hWnd,string strFileName)
{
 string strMessage="";
 ofstream out(strFileName.c_str());
 if(out.fail()){
  strMessage="Error el archivo: ";
  strMessage+=GetFileName(strFileName);
  strMessage+=" no pudo abrirse";
  MessageBox(hWnd,strMessage.c_str(),"Process Viewer",MB_OK|MB_ICONERROR);
  return;
 }
 ClearProcess();
 GetProcess();;

 for(int i=0;i<LProcess.size();i++){	 
  out << endl;
  out << "Proceso: "        << LProcess[i].strNameProcess.c_str() << endl;
  out << "PID: "            << hex << LProcess[i].dwPID << endl;
  out << "Prioridad Base: " << dec <<  LProcess[i].dwClassBase << endl;
  out << "Numero de Hilos " << dec <<  LProcess[i].dwThreads << endl;
  out << "Ruta completa: "  << LProcess[i].strFullPath.c_str();
  out << endl;
 }
 out.close();
 strMessage="Se exporto el archivo ";
 strMessage+= GetFileName(strFileName);
 strMessage+=" correctamente";
 MessageBox(hWnd,strMessage.c_str(),"Process Viewer",MB_OK|MB_ICONINFORMATION);

}

void CProcess::ExportToHTML(HWND hWnd,string strFileName)
{

 string strMessage="";
 ofstream out(strFileName.c_str());
 if(out.fail()){
  strMessage="Error el archivo: ";
  strMessage+=GetFileName(strFileName);
  strMessage+=" no pudo abrirse";
  MessageBox(hWnd,strMessage.c_str(),"Process Viewer",MB_OK|MB_ICONERROR);
  return;
 }
 ClearProcess();
 GetProcess();
 out << "<html>" << endl;
 out << "<head>" << endl;
 out << "<title>Process Viewer</title>" << endl;
 out << "</head>" << endl;
 out << "<body text=#C0C0C0 bgcolor=#000000>" << endl;
 out << "<center>" << endl;
 out << "<H1>" << endl;
 out << "<B>Lista de Procesos</B>" << endl;
 out << "</H1>" << endl;
 out << "<TABLE BORDER=1 CELLSPACING=1 CELLPADDING=1><TR>"<<endl;
 out << " <TD align=center> " << endl;
 out << " <font color=#FFFFFF> <b>Proceso</b></font></TD> " << endl;
 out << " <TD align=center>" << endl;
 out << " <font color=#FFFFFF> <b>PID</b></font></TD>" << endl;
 out << " <TD align=center> " << endl;
 out << " <font color=#FFFFFF><b> Prioridad Base </b></font></TD>" << endl;
 out << " <TD align=center> " << endl;
 out << " <font color=#FFFFFF> <b>Numero de Hilos</b></font></TD>" << endl;
 out << " <TD align=center> " << endl;
 out << " <font color=#FFFFFF> <b>Ruta completa</b></font></TD></TR>" << endl;
 for(int i=0;i<LProcess.size();i++){	 
  out << endl;
  out << "<tr><td> "<< LProcess[i].strNameProcess.c_str() << "</td>" << endl;
  out << "<td> "<< hex << LProcess[i].dwPID << "</td>" << endl;
  out << "<td> " << dec <<  LProcess[i].dwClassBase << "</td>" << endl;
  out << "<td> " << dec <<  LProcess[i].dwThreads << "</td>" << endl;
  out << "<td> " << LProcess[i].strFullPath.c_str() << "</td></tr>" << endl;;
  out << endl;
 }
 out << "</TABLE>" << endl;
 out << "</center>" << endl;
 out << "</body>" << endl;
 out << "</html>" << endl;
 out.close();
 strMessage="Se exporto el archivo ";
 strMessage+= GetFileName(strFileName);
 strMessage+=" correctamente";
 MessageBox(hWnd,strMessage.c_str(),"Process Viewer",MB_OK|MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "Registry.hpp"
//---------------------------------------------------------------------------
#include "MainUnit.h"
#include "TesterThreadUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
	CS = new TCriticalSection();
	threadList = new TList();

	sslStaticLock = new TSslStaticLock(NULL);

	loadSettings();
}
//---------------------------------------------------------------------------
__fastcall TMainForm::~TMainForm()
{
	if(RunButton->Caption == "Stop!")
		RunButtonClick(NULL);

	saveSettings();

	delete sslStaticLock;

	delete threadList;
	delete CS;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::loadSettings(void)
{
	TMemIniFile *iniFile = new TMemIniFile("WebStressTester.ini");

	try
	{
		URLLabeledEdit->Text = iniFile->ReadString("Settings", "URL", "http://localhost");
		ThreadCountLabeledEdit->Text = iniFile->ReadString("Settings", "NoOfThreads", "10");
		ClientsPerThreadLabeledEdit->Text = iniFile->ReadString("Settings", "NoOfClientsPerThread", "1");
		KeepAliveComboBox->ItemIndex = iniFile->ReadString("Settings", "KeepAlive", "1").ToIntDef(1);
		MethodComboBox->ItemIndex = iniFile->ReadString("Settings", "Method", "0").ToIntDef(0);
		ProxyIPLabeledEdit->Text = iniFile->ReadString("Settings", "ProxyIP", "localhost");
		ProxyPortLabeledEdit->Text = iniFile->ReadString("Settings", "ProxyPort", "8080");
		Memo->Text = iniFile->ReadString("Settings", "POSTData", "");
	}
	catch(Exception &e)
	{}

	delete iniFile;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::saveSettings(void)
{
	TMemIniFile *iniFile = new TMemIniFile("WebStressTester.ini");

	try
	{
		iniFile->WriteString("Settings", "URL", URLLabeledEdit->Text);
		iniFile->WriteString("Settings", "NoOfThreads", ThreadCountLabeledEdit->Text);
		iniFile->WriteString("Settings", "NoOfClientsPerThread", ClientsPerThreadLabeledEdit->Text);
		iniFile->WriteString("Settings", "KeepAlive", (String)KeepAliveComboBox->ItemIndex);
		iniFile->WriteString("Settings", "Method", (String)MethodComboBox->ItemIndex);
		iniFile->WriteString("Settings", "ProxyIP", ProxyIPLabeledEdit->Text);
		iniFile->WriteString("Settings", "ProxyPort", ProxyPortLabeledEdit->Text);
		iniFile->WriteString("Settings", "POSTData", Memo->Text);

		iniFile->UpdateFile();
	}
	catch(Exception &e)
	{}

	delete iniFile;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ThreadCountLabeledEditKeyPress(TObject *Sender, char &Key)
{
	if(!(((Key >= '0' && Key <= '9') && (ThreadCountLabeledEdit->SelLength || ThreadCountLabeledEdit->Text.Length() <= 4)) || Key == '\b' || Key == '\r'))
		Key = '\0';
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ClientsPerThreadLabeledEditKeyPress(TObject *Sender, char &Key)
{
	if(!(((Key >= '0' && Key <= '9') && (ClientsPerThreadLabeledEdit->SelLength || ClientsPerThreadLabeledEdit->Text.Length() <= 4)) || Key == '\b' || Key == '\r'))
		Key = '\0';
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::RunButtonClick(TObject *Sender)
{
	if(RunButton->Caption == "Run!")
	{
		CS->Acquire();
		response2xx = 0;
		response3xx = 0;
		response4xx = 0;
		response5xx = 0;
		responseDropped = 0;
		lastHits = 0;
		lastBytes = 0;
		Bytes = 0;
		CS->Release();

		secs = 0;
		Timer->Enabled = true;
		RunButton->Caption = "Stop!";
		String method = MethodComboBox->Items->Strings[MethodComboBox->ItemIndex];
		int clientsPerThread = ClientsPerThreadLabeledEdit->Text.ToIntDef(10);
		bool keepAlive = KeepAliveComboBox->ItemIndex == 1;

		for(int i = 0; i < ThreadCountLabeledEdit->Text.ToIntDef(2); ++i)
			threadList->Add(new TesterThread(false, this, URLLabeledEdit->Text, method, clientsPerThread, keepAlive, Memo->Text, ProxyIPLabeledEdit->Text, ProxyPortLabeledEdit->Text));
	}
	else
	{
		Timer->Enabled = false;

		for(int i = 0; i < threadList->Count; ++i)
			stopThread((TesterThread*)threadList->Items[i]);

		for(int i = 0; i < threadList->Count; ++i)
		{
			while(serverActive((TesterThread*)threadList->Items[i]))
				::Sleep(1);

			delete (TesterThread*)threadList->Items[i];
		}

		threadList->Clear();
		RunButton->Caption = "Run!";
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::stopThread(TesterThread *testerThread)
{
	testerThread->Terminate();
	PostThreadMessage(testerThread->ThreadID, WM_QUIT, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::incResponse2xx(void)
{
	CS->Acquire();
	++response2xx;
	CS->Release();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::incResponse3xx(void)
{
	CS->Acquire();
	++response3xx;
	CS->Release();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::incResponse4xx(void)
{
	CS->Acquire();
	++response4xx;
	CS->Release();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::incResponse5xx(void)
{
	CS->Acquire();
	++response5xx;
	CS->Release();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::incResponseDropped(void)
{
	CS->Acquire();
	++responseDropped;
	CS->Release();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::incKB(int dataLen)
{
	CS->Acquire();
	Bytes += (unsigned __int64)dataLen;
	CS->Release();
}
//---------------------------------------------------------------------------
bool __fastcall TMainForm::serverActive(TesterThread *testerThread)
{
	unsigned long exitCode;

	if(testerThread && GetExitCodeThread((HWND)(testerThread->Handle), &exitCode))
		if(exitCode == STILL_ACTIVE)
			return true;

	return false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::TimerTimer(TObject *Sender)
{
	++secs;

	CS->Acquire();
	String buffer2xx = String(response2xx);
	String buffer3xx = String(response3xx);
	String buffer4xx = String(response4xx);
	String buffer5xx = String(response5xx);
	String bufferDropped = String(responseDropped);
	String totalKB = String(Bytes / 1024);
	String KBPerSec = String(((double)(Bytes - lastBytes)) / ((double)1024));
	__int64 hits = response2xx + response3xx + response4xx + response5xx;
	String hitsPerSec = String(hits - lastHits);
	lastHits = hits;
	lastBytes = Bytes;
	CS->Release();

	LabeledEdit2xx->Text = buffer2xx;
	LabeledEdit3xx->Text = buffer3xx;
	LabeledEdit4xx->Text = buffer4xx;
	LabeledEdit5xx->Text = buffer5xx;
	LabeledEditDropped->Text = bufferDropped;
	LabeledEditTotalKB->Text = totalKB;
	LabeledEditKBPerSec->Text = KBPerSec;
	LabeledEditHitsPerSec->Text = hitsPerSec;

	int seconds, minutes, hours;

	seconds = secs % 60;
	minutes = (secs % 3600 - seconds) / 60;
	hours = (secs - minutes * 60 - seconds) / 3600;

	LabeledEditElapsedTime->Text = String(hours) + ":" + (minutes < 10 ? "0" : "") + String(minutes) + ":" + (seconds < 10 ? "0" : "") + String(seconds);
}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <SyncObjs.hpp>
//---------------------------------------------------------------------------
#include "OverbyteIcsSslThrdLock.hpp"
//---------------------------------------------------------------------------
class TesterThread;
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TButton *RunButton;
	TLabeledEdit *ThreadCountLabeledEdit;
	TLabeledEdit *ClientsPerThreadLabeledEdit;
	TLabeledEdit *URLLabeledEdit;
	TComboBox *MethodComboBox;
	TComboBox *KeepAliveComboBox;
	TLabel *Label1;
	TLabel *Label2;
	TLabeledEdit *LabeledEdit2xx;
	TLabeledEdit *LabeledEditDropped;
	TLabeledEdit *LabeledEdit3xx;
	TLabeledEdit *LabeledEdit4xx;
	TLabeledEdit *LabeledEdit5xx;
	TLabeledEdit *LabeledEditHitsPerSec;
	TLabeledEdit *LabeledEditElapsedTime;
	TLabeledEdit *LabeledEditTotalKB;
	TLabeledEdit *LabeledEditKBPerSec;
	TTimer *Timer;
	TMemo *Memo;
	TLabeledEdit *ProxyIPLabeledEdit;
	TLabeledEdit *ProxyPortLabeledEdit;
	TEdit *Edit1;
	void __fastcall ThreadCountLabeledEditKeyPress(TObject *Sender, char &Key);
	void __fastcall ClientsPerThreadLabeledEditKeyPress(TObject *Sender, char &Key);
	void __fastcall RunButtonClick(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);
private:	// User declarations
	unsigned __int64 secs;

	TList *threadList;
	TCriticalSection *CS;
	int response2xx;
	int response3xx;
	int response4xx;
	int response5xx;
	int responseDropped;
	__int64 lastHits;
	unsigned __int64 Bytes;
	unsigned __int64 lastBytes;

	TSslStaticLock *sslStaticLock;

	bool __fastcall serverActive(TesterThread *testerThread);
	void __fastcall stopThread(TesterThread *testerThread);
	void __fastcall loadSettings(void);
	void __fastcall saveSettings(void);
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
	__fastcall ~TMainForm();

	void __fastcall incResponse2xx(void);
	void __fastcall incResponse3xx(void);
	void __fastcall incResponse4xx(void);
	void __fastcall incResponse5xx(void);
	void __fastcall incResponseDropped(void);
	void __fastcall incKB(int dataLen);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif

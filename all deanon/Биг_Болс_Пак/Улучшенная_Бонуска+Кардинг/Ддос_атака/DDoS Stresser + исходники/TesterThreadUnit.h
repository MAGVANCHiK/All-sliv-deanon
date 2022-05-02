//---------------------------------------------------------------------------

#ifndef TesterThreadUnitH
#define TesterThreadUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
#include <OverbyteIcsHttpProt.hpp>
#include <OverbyteIcsSslSessionCache.hpp>
//---------------------------------------------------------------------------
class TMainForm;
class TesterThread : public TThread
{
private:
	String URL;
	String method;
	int clientsPerThread;
	bool keepAlive;
	TList *clientList;
	TMainForm *mainForm;
	TSslContext *SslContext;
	String POSTString;
	String proxyIP;
	String proxyPort;

	void __fastcall startClients(void);
	void __fastcall stopClients(void);
	void __fastcall messagePump(void);
	bool __fastcall isTerminated(void);
	void __fastcall processServerThreadMessage(const MSG &msg);

	void __fastcall HTTPClientHeaderData(TObject *Sender);
	void __fastcall HTTPClientDocData(TObject *Sender, void *data, int len);
	void __fastcall HTTPClientRequestDone(TObject *Sender, THttpRequest RqType, WORD ErrCode);
protected:
	void __fastcall Execute();
public:
	__fastcall TesterThread(bool CreateSuspended,
				      TMainForm *mainForm,
				      const String &URL,
				      const String &method,
				      int clientsPerThread,
				      bool keepAlive,
				      const String &POSTString,
				      const String &proxyIP,
				      const String &proxyPort);
};
//---------------------------------------------------------------------------
#endif

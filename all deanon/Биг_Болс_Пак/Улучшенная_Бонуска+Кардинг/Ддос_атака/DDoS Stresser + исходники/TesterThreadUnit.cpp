//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "TesterThreadUnit.h"
#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OverbyteIcsHttpProt"
#pragma link "OverbyteIcsWndControl"
#pragma link "OverbyteIcsSslSessionCache"
//---------------------------------------------------------------------------
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TesterThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
__fastcall TesterThread::TesterThread(bool CreateSuspended,
				      TMainForm *mainForm,
				      const String &URL,
				      const String &method,
				      int clientsPerThread,
				      bool keepAlive,
				      const String &POSTString,
				      const String &proxyIP,
				      const String &proxyPort)
	: TThread(CreateSuspended)
{
	this->mainForm = mainForm;
	this->URL= URL;
	this->method = method;
	this->clientsPerThread = clientsPerThread;
	this->keepAlive = keepAlive;
	this->POSTString = POSTString;
	this->proxyIP = proxyIP;
	this->proxyPort = proxyPort;
	this->SslContext = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TesterThread::Execute()
{
	//---- Place thread code here ----
	//TSslAvlSessionCache *SslAvlSessionCache = new TSslAvlSessionCache(NULL);

	if(URL.Pos("https://") == 1)
	{
		SslContext = new TSslContext(NULL);
		SslContext->SslVerifyPeer = false;
		SslContext->SslVerifyPeerModes << SslVerifyMode_PEER;
		SslContext->SslOptions << sslOpt_MICROSOFT_SESS_ID_BUG << sslOpt_NETSCAPE_CHALLENGE_BUG << sslOpt_NETSCAPE_REUSE_CIPHER_CHANGE_BUG << sslOpt_SSLREF2_REUSE_CERT_TYPE_BUG << sslOpt_MICROSOFT_BIG_SSLV3_BUFFER << sslOpt_SSLEAY_080_CLIENT_DH_BUG << sslOpt_TLS_D5_BUG << sslOpt_TLS_BLOCK_PADDING_BUG, sslOpt_TLS_ROLLBACK_BUG << sslOpt_NETSCAPE_CA_DN_BUG << sslOpt_NETSCAPE_DEMO_CIPHER_CHANGE_BUG;
		SslContext->InitContext();
	}

	startClients();

	while(!Terminated)
		messagePump();

	stopClients();

	delete SslContext;
	//delete SslAvlSessionCache;
}
//---------------------------------------------------------------------------
void __fastcall TesterThread::messagePump(void)
{
	MSG msg;

	while(GetMessage(&msg, NULL, 0, 0))
	{
		try
		{
			processServerThreadMessage(msg);
		}
		catch(ESocketException &e)
		{
		}
		catch(Exception &e)
		{
		}
	}
}
//---------------------------------------------------------------------------
bool __fastcall TesterThread::isTerminated(void)
{
	return Terminated;
}
//---------------------------------------------------------------------------
void __fastcall TesterThread::processServerThreadMessage(const MSG &msg)
{
	DispatchMessage(&msg);
}
//---------------------------------------------------------------------------
void __fastcall TesterThread::startClients(void)
{
	clientList = new TList();
	TSslHttpCli *bufferClient;

	for(int i = 0; i < clientsPerThread; ++i)
	{
		bufferClient = new TSslHttpCli(NULL);
		bufferClient->FollowRelocation = false;
		bufferClient->OnHeaderData = HTTPClientHeaderData;
		bufferClient->OnDocData = HTTPClientDocData;
		bufferClient->OnRequestDone = HTTPClientRequestDone;
		bufferClient->URL = URL;
		bufferClient->Connection = (keepAlive ? "keep-alive" : "close");
		bufferClient->Proxy = proxyIP;
		bufferClient->ProxyPort = proxyPort;
		bufferClient->SslContext = SslContext;

		clientList->Add(bufferClient);

		if(method == "GET")
			bufferClient->GetASync();
		else if(method == "HEAD")
			bufferClient->HeadASync();
		else if(method == "POST")
		{
			bufferClient->SendStream = new TMemoryStream();
			bufferClient->SendStream->Write(POSTString.c_str(), POSTString.Length());
			bufferClient->SendStream->Seek(0, 0);
			bufferClient->PostASync();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TesterThread::stopClients(void)
{
	TSslHttpCli *bufferClient;

	for(int i = clientsPerThread - 1; i >= 0; --i)
	{
		bufferClient = (TSslHttpCli*)clientList->Items[i];
		bufferClient->Abort();
		bufferClient->Release();
	}

	delete clientList;
}
//---------------------------------------------------------------------------
void __fastcall TesterThread::HTTPClientHeaderData(TObject *Sender)
{
	TSslHttpCli *bufferClient = (TSslHttpCli*)Sender;

	String upperResponse = UpperCase(bufferClient->LastResponse);

	if(upperResponse.Length() >= 12 && upperResponse.SubString(1, 5) == "HTTP/")
	{
		String targetServerProtocolStatus = bufferClient->LastResponse.SubString(10, 3);

		if(targetServerProtocolStatus[1] == '2')
			mainForm->incResponse2xx();
		else if(targetServerProtocolStatus[1] == '3')
			mainForm->incResponse3xx();
		else if(targetServerProtocolStatus[1] == '4')
			mainForm->incResponse4xx();
		else if(targetServerProtocolStatus[1] == '5')
			mainForm->incResponse5xx();
	}
}
//---------------------------------------------------------------------------
void __fastcall TesterThread::HTTPClientDocData(TObject *Sender, void *data, int len)
{
	mainForm->incKB(len);
}
//---------------------------------------------------------------------------
void __fastcall TesterThread::HTTPClientRequestDone(TObject *Sender, THttpRequest RqType, WORD ErrCode)
{
	if(ErrCode)
		mainForm->incResponseDropped();

	TSslHttpCli *bufferClient = (TSslHttpCli*)Sender;

	if(method == "GET")
		bufferClient->GetASync();
	else if(method == "HEAD")
		bufferClient->HeadASync();
	else if(method == "POST")
	{
		bufferClient->SendStream->Seek(0, 0);
		bufferClient->PostASync();
	}
}
//---------------------------------------------------------------------------

#ifndef ARSTSERVER_H_
#define ARSTSERVER_H_

#ifdef _WIN32
#include <Windows.h>
#endif

#include <string>
#include <memory>
#include <iostream>

#include "ARSTCommon/ARSTThread.h"
#include "ARSTCommon/ARSTClientSocketCallback.h"
#include "ARSTCommon/ARSTHeartbeatCallback.h"
#include "ARSTCommon/ARSTRequestCallback.h"
#include "ARSTCommon/ARSTClientSocket.h"
#include "ARSTHeartbeat.h"
#include "ARSTRequest.h"
#include "ARSTServer.h"

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <nlohmann/json.hpp>

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

using json = nlohmann::json;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using namespace std;

#define MAX_DATA_LENGTH 4096 

class ARSTClient;

enum ARSTServerStatus
{
	ssNone,
	ssFree,
	ssInuse,
	ssBreakdown,
	ssReconnecting
};


using namespace std;

class ARSTServer: public ARSTThread, public ARSTClientSocketCallback, public ARSTHeartbeatCallback, public ARSTRequestCallback
{
	private:
		friend void ARSTClient:: TriggerSendRequestEvent(ARSTServer* pServer, unsigned char* pRequestMessage, int nRequestMessageLength);

		client m_cfd;
		char m_caPthread_ID[21];
		shared_ptr<ARSTClient> m_shpClient;

		ARSTClientSocket* m_pClientSocket;
		ARSTHeartbeat* m_pHeartbeat;
		ARSTRequest* m_pRequest;


		ARSTRequestMarket m_rmRequestMarket;
		unsigned char m_uncaSecondByte;
		int m_nReplyMessageLength;

		unsigned char m_uncaRequestMessage[MAX_DATA_LENGTH];
		int m_nRequestMessageLength;

		int m_nOriginalOrderLength;
		int m_nReplyMsgLength;//for reply_msg[78] reply_msg[80]
		int m_nPoolIndex;
		int m_heartbeat_count;
		bool m_FTX_enable;

		pthread_mutex_t m_pmtxServerStatusLock;
		static context_ptr CB_TLS_Init(const char *, websocketpp::connection_hdl);
	protected:
		void* Run();

		void OnConnect();
		void OnDisconnect();

		void OnHeartbeatLost();
		void OnHeartbeatRequest();
		void OnHeartbeatError(int nData, const char* pErrorMessage);

		bool RecvAll(const char* pWhat, unsigned char* pBuf, int nToRecv);
		bool SendAll(const char* pWhat, const unsigned char* pBuf, int nToSend);

		void ReconnectSocket();
		void OnRequest();
		void OnRequestError(int, const char*);
		void OnData(unsigned char*, int);

	public:
		client::connection_ptr m_conn;
		ARSTServerStatus m_ssServerStatus;
		ARSTServer(string strHost, string strPort, string strName, ARSTRequestMarket rmRequestMarket);
		virtual ~ARSTServer();

		void SetCallback(shared_ptr<ARSTClient>& shpClient);
		void SetRequestMessage(unsigned char* pRequestMessage, int nRequestMessageLength);

		void SetStatus(ARSTServerStatus ssServerStatus);
		ARSTServerStatus GetStatus();
		string m_strHost;
		string m_strPara;
		string m_strName;
		string m_strEPID;
};
#endif

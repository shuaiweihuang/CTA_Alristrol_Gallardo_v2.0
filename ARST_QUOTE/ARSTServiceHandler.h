#ifndef ARSTCLIENT_H_
#define ARSTCLIENT_H_

#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>
#include <map>
#include <memory>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>

#include "ARSTCommon/ARSTHeartbeatCallback.h"
#include "ARSTCommon/ARSTThread.h"
#include "ARSTHeartbeat.h"
#include "ARSTGlobal.h"
#include "Include/ARSTLogonFormat.h"

class ARSTServer;

using namespace std;

union L
{
    unsigned char uncaByte[16];
    short value;
};

struct ARSTClientAddrInfo
{
    int nSocket;
	char caIP[IPLEN];
    struct sockaddr_storage ClientAddr;
};

struct ARSTAccountRecvBuf
{
	int nRecved;
	unsigned char uncaRecvBuf[BUFFERSIZE];
};

enum ARSTClientStauts
{
	csNone,
	csLogoning,
	csOnline,
	csOffline
};

enum ARSTRequestMarket
{
	rmBitmex,
	rmNum
};

struct ARSTBranchAccountInfo
{
	const char* pMarket;

	int nBranchPosition;
	int nAccountPosition;
	int nSubAccountPosition;

	int nBranchLength;
	int nAccountLength;
	int nSubAccountLength;
};

class ARSTClient: public ARSTThread, public ARSTHeartbeatCallback, public enable_shared_from_this<ARSTClient>
{
	private:
		struct ARSTClientAddrInfo m_ClientAddrInfo;
		unsigned char m_uncaLogonID[10];


		ARSTClientStauts m_csClientStatus;

		int m_nOriginalOrderLength;
		int m_nTSReplyMsgLength;//78
		int m_nTFReplyMsgLength;//80
		int m_nOFReplyMsgLength;//80
		int m_nOSReplyMsgLength;//78
		map<string, string> m_mMarketBranchAccount;

		vector<struct ARSTBranchAccountInfo*> m_vBranchAccountInfo;

		pthread_mutex_t m_pmtxClientStatusLock;
		string m_strEPID;
	protected:
		void* Run();

		void OnHeartbeatRequest();
		void OnHeartbeatLost();
		void OnHeartbeatError(int nData, const char* pErrorMessage);

		bool Logon(char* pID, char* pPasswd, struct ARSTLogonReply &struLogonReply);
		bool GetAccount(char* pID, char* pAgent, char* pVersion, vector<char*> &vAccountData);

		bool RecvAll(const char* pWhat, unsigned char* pBuf, int nToRecv);

		void InitialBranchAccountInfo();
		bool CheckBranchAccount(ARSTRequestMarket rmRequestMarket, unsigned char* pRequstMessage);

	public:
		ARSTClient(struct ARSTClientAddrInfo &ClientAddrInfo);
		bool SendAll(const char* pWhat, char* pBuf, int nToSend);
		virtual ~ARSTClient();

		void TriggerSendRequestEvent(ARSTServer* pServer, unsigned char* pRequestMessage, int nRequestMessageLength);

		bool SendLogonReply(struct ARSTLogonReply &struLogonReply);
		bool SendAccountCount(short sCount);
		bool SendAccountData(vector<char*> &vAccountData);
		bool SendRequestReply(unsigned char uncaSecondByte, unsigned char* unpRequestReplyMessage, int nRequestReplyMessageLength);
		bool SendRequestErrorReply(unsigned char uncaSecondByte,unsigned char* pOriginalRequstMessage,int nOriginalRequestMessageLength,const char* pErrorMessage,short nErrorCode);

		void SetStatus(ARSTClientStauts csStatus);
		ARSTClientStauts GetStatus();

		int GetClientSocket();
		ARSTHeartbeat* m_pHeartbeat;
};
#endif

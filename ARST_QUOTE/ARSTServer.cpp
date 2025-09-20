#include <fstream>
#include <cstring>
#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <sys/msg.h>

#include "ARSTCommon/ARSTServerSocket.h"
#include "ARSTWebConn.h"
#include "ARSTWebConns.h"
#include "ARSTServiceHandler.h"
#include "ARSTServer.h"


using namespace std;

extern void FprintfStderrLog(const char* pCause, int nError, int nData, const char* pFile = NULL, int nLine = 0,
                             unsigned char* pMessage1 = NULL, int nMessage1Length = 0, unsigned char* pMessage2 = NULL, int nMessage2Length = 0);

ARSTClients* ARSTClients::instance = NULL;
pthread_mutex_t ARSTClients::ms_mtxInstance = PTHREAD_MUTEX_INITIALIZER;

ARSTClients::ARSTClients()
{
	pthread_mutex_init(&m_pmtxClientVectorLock, NULL);
}

ARSTClients::~ARSTClients()
{
	if(m_pServerSocket)
	{
		m_pServerSocket->ShutdownServer();

		delete m_pServerSocket;

		m_pServerSocket = NULL;
	}

	m_vClient.clear();//to check

	pthread_mutex_destroy(&m_pmtxClientVectorLock);
}

void* ARSTClients::Run()
{
	while(m_pServerSocket->GetStatus() == sssListening)
	{
		struct ARSTClientAddrInfo ClientAddrInfo;
		memset(&ClientAddrInfo, 0, sizeof(struct ARSTClientAddrInfo));

		ClientAddrInfo.nSocket = m_pServerSocket->Accept(&ClientAddrInfo.ClientAddr);
		socklen_t addr_size = sizeof(ClientAddrInfo.ClientAddr);

		struct sockaddr_in *sin = (struct sockaddr_in *)&ClientAddrInfo.ClientAddr;
		unsigned char *ip = (unsigned char *)&sin->sin_addr.s_addr;
		sprintf(ClientAddrInfo.caIP,"%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);//to do

		FprintfStderrLog("Quote:ACCEPT_CLIENT_IP", 0, 0, NULL, 0, reinterpret_cast<unsigned char*>(ClientAddrInfo.caIP), strlen(ClientAddrInfo.caIP));
		shared_ptr<ARSTClient> shpClient = make_shared<ARSTClient>(ClientAddrInfo);
		PushBackClientToVector(shpClient);
	}

	return NULL;
}

void ARSTClients::OnListening()
{
	Start();
}

void ARSTClients::OnShutdown()
{
}

ARSTClients* ARSTClients::GetInstance()
{
	if(instance == NULL)
	{
		pthread_mutex_lock(&ms_mtxInstance);//lock

		if(instance == NULL)
		{
			instance = new ARSTClients();
			FprintfStderrLog("CLIENTS_ONE", 0, 0);
		}

		pthread_mutex_unlock(&ms_mtxInstance);//unlock
	}

	return instance;
}

void ARSTClients::SetConfiguration(string& strListenPort, string& strHeartBeatTime, string& strEPIDNum, int& nService)
{
	m_strListenPort = strListenPort;
	m_strHeartBeatTime = strHeartBeatTime;
	m_strEPIDNum = strEPIDNum;
	m_nService = nService;

	try
	{
		m_pServerSocket = new ARSTServerSocket(this);
		m_pServerSocket->Listen(m_strListenPort);
	}
	catch (exception& e)
	{
		FprintfStderrLog("NEW_SERVERSOCKET_ERROR", -1, 0, NULL, 0, (unsigned char*)e.what(), strlen(e.what()));
	}
}


void ARSTClients::CheckClientVector()
{
	vector<shared_ptr<ARSTClient> >::iterator iter = m_vClient.begin();
	while(iter != m_vClient.end())
	{
		ARSTClient* pClient = (*iter).get();
		if(pClient->GetStatus() == csOffline && (*iter).unique())
		{
			ShutdownClient(pClient->GetClientSocket());
			EraseClientFromVector(iter);
		}
		else
		{
			iter++;
		}
	}
}

void ARSTClients::PushBackClientToVector(shared_ptr<ARSTClient>& shpClient)
{
	pthread_mutex_lock(&m_pmtxClientVectorLock);

	m_vClient.push_back(std::move(shpClient));

	pthread_mutex_unlock(&m_pmtxClientVectorLock);
}

void ARSTClients::EraseClientFromVector(vector<shared_ptr<ARSTClient> >::iterator iter)
{
	pthread_mutex_lock(&m_pmtxClientVectorLock);

	m_vClient.erase(iter);

	pthread_mutex_unlock(&m_pmtxClientVectorLock);
}

void ARSTClients::ShutdownClient(int nSocket)
{
	if(m_pServerSocket)
		m_pServerSocket->ShutdownClient(nSocket);
}

bool ARSTClients::IsServiceRunning(enum ARSTRequestMarket& rmRequestMarket)
{
	return (m_nService & 1<<rmRequestMarket) ? true : false;
}

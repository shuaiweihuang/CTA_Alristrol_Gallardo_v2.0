#include <iostream>
#include <unistd.h>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <sys/time.h>
#include <assert.h>
#include <algorithm>

#include "ARSTQueueNode.h"
#include "ARSTServer.h"
#define PRICE_POS	3
#define VOLUME_POS	4
#define TC_POS		5
#define EPOCH_POS	8
#define END_POS		9

#define QUEUELOCALSIZE	1000
#define MERGE_COUNT 1

using namespace std;

vector<string> QueueLocal;
ARSTQueueDAO::ARSTQueueDAO(string strService, key_t kSendKey, key_t kRecvKey)
{
	m_strService = strService;

	m_kSendKey = kSendKey;
	m_kRecvKey = kRecvKey;

	m_pSendQueue = new ARSTQueue;
	m_pSendQueue->Create(m_kSendKey);

	m_pRecvQueue = new ARSTQueue;
	m_pRecvQueue->Create(m_kRecvKey);

	Start();
}

ARSTQueueDAO::~ARSTQueueDAO() 
{
	if(m_pSendQueue != NULL) 
	{
		delete m_pSendQueue;
	}

	if(m_pRecvQueue != NULL) 
	{
		delete m_pRecvQueue;
	}
}

void* ARSTQueueDAO::Run()
{
	ARSTClients* pClients = ARSTClients::GetInstance();
	assert(pClients);

	char uncaRecvBufToken[BUFSIZE];
	char uncaRecvBuf[BUFSIZE];
	char uncaRecvBufPiece[BUFSIZE];

	static string hash_string;
	static string out_string;
	static int tick_count = 0;
	int    readcount = 0;

	memset(uncaRecvBuf, 0, sizeof(uncaRecvBuf));

	while(m_pRecvQueue)
	{
		memset(uncaRecvBufToken, 0, sizeof(uncaRecvBufToken));
		memset(uncaRecvBufPiece, 0, sizeof(uncaRecvBufPiece));
		usleep(100);

		int nGetMessage = m_pRecvQueue->GetMessage(uncaRecvBufToken);
		strcpy(uncaRecvBufPiece, uncaRecvBufToken);
#ifdef DEBUG
		printf("SERVER: queue data read at key %d\n", m_kRecvKey);
#endif
		if(nGetMessage > 0)
		{

			char *token = strtok(uncaRecvBufToken, ",");
			int GTA_index = 0;
			while((token = strtok(NULL, ",")))
			{
				GTA_index++;
				if(GTA_index == PRICE_POS || GTA_index == EPOCH_POS || GTA_index == VOLUME_POS)
				{
					hash_string += token;
				}
#if 0
				if(GTA_index == TC_POS)
				{
					out_string += "TC=";
					out_string += to_string(tick_count);
					tick_count++;
				}
				else
					out_string += token;
				if(GTA_index != END_POS)
					out_string += ",";
#endif
			}
			
			if(find(QueueLocal.begin(), QueueLocal.end(), hash_string.c_str()) == QueueLocal.end())
			{
				//printf("%s\n", uncaRecvBuf);
				//printf("%s\n", hash_string.c_str());
				//printf("%s\n", out_string.c_str());
				QueueLocal.push_back(hash_string.c_str());
				strcpy(uncaRecvBuf+strlen(uncaRecvBuf), uncaRecvBufPiece);
				readcount++;
				if(readcount>=MERGE_COUNT) {
					//printf("\n%d:\n %s\n", strlen(uncaRecvBuf), uncaRecvBuf);
					vector<shared_ptr<ARSTClient> >::iterator iter = pClients->m_vClient.begin();
					while(iter != pClients->m_vClient.end())
					{
						ARSTClient* pClient = (*iter).get();
						if(pClient->GetStatus() == csOffline && (*iter).unique()) {
							iter++;
							continue;
						}
						//if(pClient->SendAll(NULL, (char*)out_string.c_str(), out_string.length()) != false) {

						if(pClient->SendAll(NULL, uncaRecvBuf, strlen(uncaRecvBuf)) != false ) {

							pClient->m_pHeartbeat->TriggerGetReplyEvent();
						}
						iter++;
					}
					memset(uncaRecvBuf, 0, sizeof(uncaRecvBuf));
					readcount = 0;
				}
			}
			else
			{
				//printf("2. %s\n", hash_string.c_str());
				while(QueueLocal.size() > (QUEUELOCALSIZE / 4 * 3))
				{
					QueueLocal.erase(QueueLocal.begin(), QueueLocal.begin()+250);
				}
				//printf("%d\n", QueueLocal.size());
			}
			hash_string.clear();
			out_string.clear();
		}
		if(nGetMessage < 0)
			exit(-1);
	}
	return NULL;
}

int ARSTQueueDAO::SendData(char* pBuf, int nSize, long lType, int nFlag)
{
	int nResult = -1;
#ifdef DEBUG
	printf("SERVER: queue data send at key %d\ndata contents: %s\n", m_kSendKey, pBuf);
#endif
	if(m_pSendQueue)
	{
		nResult= m_pSendQueue->SendMessage(pBuf, nSize, lType, nFlag);
	}
	else
	{
		return nResult;
	}

	return nResult;
}

key_t ARSTQueueDAO::GetRecvKey()
{
	return m_kRecvKey;
}

key_t ARSTQueueDAO::GetSendKey()
{
	return m_kSendKey;
}

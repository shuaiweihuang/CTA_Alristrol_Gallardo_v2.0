#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <stdexcept>

#include "ARSTWebConns.h"
#include "ARSTWebConn.h"
#include "ARSTGlobal.h"
#include "ARSTQueueManager.h"


using namespace std;

extern void FprintfStderrLog(const char* pCause, int nError, int nData, const char* pFile = NULL, int nLine = 0,
				unsigned char* pMessage1 = NULL, int nMessage1Length = 0, unsigned char* pMessage2 = NULL, int nMessage2Length = 0);

ARSTServers* ARSTServers::instance = NULL;
pthread_mutex_t ARSTServers::ms_mtxInstance = PTHREAD_MUTEX_INITIALIZER;
struct MNTRMSGS g_MNTRMSG;

extern void mem_usage(double& vm_usage, double& resident_set);

ARSTServers::ARSTServers()
{
	unsigned char msg[BUFFERSIZE];
	m_pHeartbeat = NULL;
	memset(&g_MNTRMSG, 0, sizeof(struct MNTRMSGS));
	try
	{
		m_pHeartbeat = new ARSTHeartbeat(this);
		m_pHeartbeat->SetTimeInterval(MONITOR_HEARTBEAT_INTERVAL_SEC);
		memset(msg, 0, BUFFERSIZE);
		sprintf((char*)msg, "add heartbeat in quote monitor service.");
		FprintfStderrLog("NEW_HEARTBEAT_CREATE", -1, 0, __FILE__, __LINE__, msg, strlen((char*)msg));
	}
	catch (exception& e)
	{
		FprintfStderrLog("NEW_HEARTBEAT_ERROR", -1, 0, __FILE__, __LINE__, 0, 0, 0, 0);
		return;
	}
}

ARSTServers::~ARSTServers()
{
	if(m_pHeartbeat)
	{
		delete m_pHeartbeat;
		m_pHeartbeat = NULL;
	}
}



void ARSTServers::AddFreeServer(enum ARSTRequestMarket rmRequestMarket, int nServerIndex)
{
	try
	{
		ARSTServer* pServer = new ARSTServer(m_vServerConfig.at(rmRequestMarket)->vServerInfo.at(nServerIndex)->strHost,
						   m_vServerConfig.at(rmRequestMarket)->vServerInfo.at(nServerIndex)->strPara,
						   m_vServerConfig.at(rmRequestMarket)->vServerInfo.at(nServerIndex)->strName,
						   rmRequestMarket);
		printf("[%s] Service: (%s:%s)\n",
		m_vServerConfig.at(rmRequestMarket)->vServerInfo.at(nServerIndex)->strName.c_str(),
		m_vServerConfig.at(rmRequestMarket)->vServerInfo.at(nServerIndex)->strHost.c_str(),
		m_vServerConfig.at(rmRequestMarket)->vServerInfo.at(nServerIndex)->strPara.c_str());
		m_vServerPool.push_back(pServer);

	}	
	catch(const out_of_range& e)
	{
		FprintfStderrLog("OUT_OF_RANGE_ERROR", -1, 0, __FILE__, __LINE__, (unsigned char*)e.what(), strlen(e.what()));
	}
}

ARSTServers* ARSTServers::GetInstance()
{
	if(instance == NULL)
	{
		pthread_mutex_lock(&ms_mtxInstance);//lock

		if(instance == NULL)
		{
			instance = new ARSTServers();
			FprintfStderrLog("SERVERS_ONE", 0, 0);
		}

		pthread_mutex_unlock(&ms_mtxInstance);//unlock
	}

	return instance;
}

void ARSTServers::SetConfiguration(struct ARSTConfig* pstruConfig)
{
	m_vServerConfig.push_back(pstruConfig);
}

void ARSTServers::StartUpServers()
{
	try
	{
		printf("Number of Markets : %d\n", rmNum);

		for(int i=0 ; i<rmNum ; i++)
		{
			printf("Server count/subset: %d/%d\n", m_vServerConfig.at(i)->nServerCount,  m_vServerConfig.at(i)->nServerSet);

			for(int j=0 ; j<m_vServerConfig.at(i)->nServerCount ; j++)
			{
				printf("AddFreeServer %d-%d\n", i, j);
				AddFreeServer((ARSTRequestMarket)i, j);
			}
		}
#ifdef DEBUG
		printf("vector size = %d\n", m_vServerPool.size());
#endif
	}
	catch(const out_of_range& e)
	{
		FprintfStderrLog("OUT_OF_RANGE_ERROR", -1, 0, __FILE__, __LINE__, (unsigned char*)e.what(), strlen(e.what()));
	}

}

ARSTServer* ARSTServers::GetServerByName(string name)
{
	vector<ARSTServer*>::iterator iter = m_vServerPool.begin();
	while(iter != m_vServerPool.end())
	{
		if((*iter)->m_strName == name)
		{
			return *iter;
		}
		iter++;
	}
	return NULL;
}

void ARSTServers::CheckClientVector()
{
	for(int i=0 ; i<m_vServerPool.size() ; i++)
	{
		//printf("[%s] break down polling:%d\n", (m_vServerPool[i])->m_strName.c_str(), (m_vServerPool[i])->m_ssServerStatus);
		if((m_vServerPool[i])->m_ssServerStatus == ssBreakdown)
		{
			sleep(5);
			//printf("[%s]: break down\n", (m_vServerPool[i])->m_strName.c_str());

			for(int j=0 ; j<m_vServerConfig.at(0)->nServerCount ; j++)
			{
				if(m_vServerConfig.at(0)->vServerInfo.at(j)->strName == "BITMEXFUND")
					continue;
				if(m_vServerConfig.at(0)->vServerInfo.at(j)->strName == (m_vServerPool[i])->m_strName)
				{
					ARSTServer* pServer = new ARSTServer(
							m_vServerConfig.at(0)->vServerInfo.at(j)->strHost,
							m_vServerConfig.at(0)->vServerInfo.at(j)->strPara,
							m_vServerConfig.at(0)->vServerInfo.at(j)->strName,
							rmBitmex);
					printf("[Reconnect] %s: Service: %s:%s\n",
					m_vServerConfig.at(0)->vServerInfo.at(j)->strName.c_str(),
					m_vServerConfig.at(0)->vServerInfo.at(j)->strHost.c_str(),
					m_vServerConfig.at(0)->vServerInfo.at(j)->strPara.c_str());
					m_vServerPool[i] = pServer;
				}
			}
		}
	}
}

void ARSTServers::OnHeartbeatLost()
{
	FprintfStderrLog("HEARTBEAT LOST", -1, 0, NULL, 0,  NULL, 0);
#ifdef EXIT_VERSION
	exit(-1);
#endif
}

void ARSTServers::OnHeartbeatRequest()
{
	char caHeartbeatRequestBuf[128];
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	double RSS_size;
	char hostname[128];

	gethostname(hostname, sizeof hostname);

	memset(caHeartbeatRequestBuf, 0, 128);
	mem_usage(g_MNTRMSG.process_vm_mb, RSS_size);
	sprintf(caHeartbeatRequestBuf, "{\"Type\":\"Heartbeat\",\"Component\":\"Quote\",\"Hostname\":\"%s\",\"ServerDate\":\"%d%02d%02d\",\"ServerTime\":\"%02d%02d%02d00\"}",
		hostname, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

//	FprintfStderrLog("HEARTBEAT REQUEST", -1, 0, NULL, 0,  NULL, 0);
	ARSTQueueDAO* pQueueDAO = ARSTQueueDAOManager::GetInstance()->m_QueueDAOMonitor;
	pQueueDAO->SendData(caHeartbeatRequestBuf, strlen(caHeartbeatRequestBuf));

	gethostname(hostname, sizeof hostname);
	snprintf(caHeartbeatRequestBuf, sizeof(caHeartbeatRequestBuf),  "{\"Type\":\"System\",\"Component\":\"Quote\",\"Hostname\":\"%s\",\"CurrentThread\":\"%d\",\"MaxThread\":\"%d\",\"MemoryUsage\":\"%.0f\"}", 			hostname, g_MNTRMSG.num_of_thread_Current, g_MNTRMSG.num_of_thread_Max, g_MNTRMSG.process_vm_mb);

	pQueueDAO->SendData(caHeartbeatRequestBuf, strlen(caHeartbeatRequestBuf));
	m_pHeartbeat->TriggerGetReplyEvent();
}

void ARSTServers::OnHeartbeatError(int nData, const char* pErrorMessage)
{
	FprintfStderrLog("HEARTBEAT ERROR", -1, 0, NULL, 0,  NULL, 0);
#ifdef EXIT_VERSION
	exit(-1);
#endif
}


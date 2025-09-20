#ifndef ARSTSERVERS_H_
#define ARSTSERVERS_H_

#include "ARSTCommon/ARSTPevents.h"
#include "ARSTWebConn.h"
#include <vector>
#include <string>
#include "../ARST_INCLUDE/ARSTMonitor.h"

using namespace neosmart;
using namespace std;

struct ARSTServerInfo
{
	string strHost;
	string strPara;
	string strName;
};

struct ARSTConfig
{
	int nServerCount;
	int nServerSet;
	vector<struct ARSTServerInfo*> vServerInfo;
};

struct MNTRMSGS
{
	int num_of_thread_Current;
	int num_of_thread_Max;
	long network_delay_ms;
	double process_vm_mb;
};

class ARSTServers: public ARSTHeartbeatCallback//public ARSTThread
{
	private:
		ARSTServers();
		virtual ~ARSTServers();
		static ARSTServers* instance;
		static pthread_mutex_t ms_mtxInstance;
		int m_alive_check;
		vector<struct ARSTConfig*> m_vServerConfig;
		//vector<vector<vector<ARSTServer*> > > m_vvvServerPool;
		vector<ARSTServer*> m_vServerPool;
		ARSTHeartbeat* m_pHeartbeat;

	protected:
		void AddFreeServer(enum ARSTRequestMarket rmRequestMarket, int nPoolIndex);
		void OnHeartbeatLost();
		void OnHeartbeatRequest();
		void OnHeartbeatError(int nData, const char* pErrorMessage);
	public:
		ARSTServer* m_ServerBitmex;
		static ARSTServers* GetInstance();
		void SetConfiguration(struct ARSTConfig* pstruConfig);
		void StartUpServers();
		void CheckClientVector();
		ARSTServer* GetServerByName(string name);
};
#endif

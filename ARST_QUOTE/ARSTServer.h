#ifndef ARSTCLIENTS_H_
#define ARSTCLIENTS_H_

#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>
#include <memory>
#include <iostream>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <fstream>

#include "ARSTCommon/ARSTSocketCallback.h"
#include "ARSTCommon/ARSTThread.h"
#include "ARSTCommon/ARSTServerSocket.h"
#include "ARSTCommon/ARSTSharedMemory.h"
#include "ARSTServiceHandler.h"
#include "ARSTGlobal.h"

using namespace std;

class ARSTClients: public ARSTThread, public ARSTSocketCallback
{
	private:
		ARSTClients();
		virtual ~ARSTClients();
		static ARSTClients* instance;
		static pthread_mutex_t ms_mtxInstance;

		ARSTServerSocket* m_pServerSocket;
		string m_strListenPort;


		int m_nService;

		pthread_mutex_t m_pmtxClientVectorLock;

#ifdef MNTRMSG
		fstream m_fileLogon;
#endif

	protected:
		void* Run();
		void OnListening();
		void OnShutdown();

	public:
		vector<shared_ptr<ARSTClient> > m_vClient;
		string m_strHeartBeatTime;
		string m_strEPIDNum;
		static ARSTClients* GetInstance();

		void SetConfiguration(string& strListenPort, string& strHeartBeatTime, string& strEPIDNum, int& nService);

		void CheckClientVector();
		void PushBackClientToVector(shared_ptr<ARSTClient>& shpClient);
		void EraseClientFromVector(vector<shared_ptr<ARSTClient> >::iterator iter);
		void ShutdownClient(int nSocket);
		bool IsServiceRunning(enum ARSTRequestMarket& rmRequestMarket);
#ifdef MNTRMSG
		void FlushLogMessageToFile();
#endif
};
#endif

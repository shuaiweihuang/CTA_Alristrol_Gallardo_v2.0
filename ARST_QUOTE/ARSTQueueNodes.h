#ifndef ARSTQUEUEDAOS_H_
#define ARSTQUEUEDAOS_H_

#include <vector>
#include <string>

#include "ARSTQueueNode.h"

using namespace std;

class ARSTQueueDAOs: public ARSTThread
{
	private:
		ARSTQueueDAOs();
		virtual ~ARSTQueueDAOs();
		static ARSTQueueDAOs* instance;
		static pthread_mutex_t ms_mtxInstance;

		vector<ARSTQueueDAO*> m_vQueueDAO;
		int m_nNumberOfQueueDAO;
		key_t m_kQueueDAOWriteStartKey;
		key_t m_kQueueDAOWriteEndKey;
		key_t m_kQueueDAOReadStartKey;
		key_t m_kQueueDAOReadEndKey;
		key_t m_kQueueDAOMonitorKey;
		key_t m_kRoundRobinIndexOfQueueDAO;

		string m_strService;

		pthread_mutex_t m_MutexLockOnGetDAO;

	protected:
		void* Run();

	public:
		static ARSTQueueDAOs* GetInstance();
		void AddDAO(key_t kSendKey,key_t kRecvKey);
		void RemoveDAO(key_t key);//Sendkey
		ARSTQueueDAO* GetDAO();//SendKey
		ARSTQueueDAO* m_QueueDAOMonitor;
		void SetConfiguration(string strService, int nNumberOfQueueDAO, key_t kQueueDAOWriteStartKey, key_t kQueueDAOWriteEndKey,
						      key_t kQueueDAOReadStartKey, key_t kQueueDAOReadEndKey, key_t kQueueDAOMonitorKey);
		void StartUpDAOs();
};
#endif

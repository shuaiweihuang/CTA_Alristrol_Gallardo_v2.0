#ifndef ARSTQUEUEDAO_H_
#define ARSTQUEUEDAO_H_

#include <map>
#include <string>

#include"ARSTCommon/ARSTQueue.h"
#include"ARSTCommon/ARSTThread.h"

using namespace std;

class ARSTQueueDAO: public ARSTThread
{
	private:
		string m_strService;
		key_t m_kRecvKey;
		key_t m_kSendKey;
		ARSTQueue* m_pSendQueue;
		ARSTQueue* m_pRecvQueue;

	protected:
		void* Run();

	public:
		ARSTQueueDAO(string strService, key_t kSendKey, key_t kRecvKey);
		virtual ~ARSTQueueDAO();

		int SendData(char* pBuf, int nSize, long lType = 1,int nFlag = 0);

		key_t GetSendKey();
		key_t GetRecvKey();
};
#endif

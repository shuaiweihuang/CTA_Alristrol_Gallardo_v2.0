#ifndef ARSTCOMMON_ARSTQUEUE_H_
#define ARSTCOMMON_ARSTQUEUE_H_

#include<sys/msg.h>

#define BUFSIZE 1500

struct ARSTQueueMessage
{
	long lMessageType;
	unsigned char uncaMessageBuf[BUFSIZE];
};

class ARSTQueue
{
	private:
		int  m_nID;

		struct ARSTQueueMessage m_QueueMessage;

	public:
		ARSTQueue();
		virtual ~ARSTQueue();

		int GetMessage(char* pBuf, long lType = 0, int nFlag = 0);
		int SendMessage(char* pBuf, int nSize, long lType = 1, int nFlag = 0);

        int Create(key_t kKey);
        int Remove();
};
#endif

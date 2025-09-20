#ifndef ARSTHEARTBEAT_H_
#define ARSTHEARTBEAT_H_

#include <string>
#include "ARSTCommon/ARSTPevents.h"
#include "ARSTCommon/ARSTThread.h"
#include "ARSTCommon/ARSTHeartbeatCallback.h"

using namespace neosmart;
using namespace std;


class ARSTHeartbeat: public ARSTThread
{
	private:
		int m_nTimeInterval;

		ARSTHeartbeatCallback* m_pHeartbeatCallback;
		neosmart_event_t m_PEvent[2];

	protected:
		void* Run();

	public:
		int m_nIdleTime;
		ARSTHeartbeat(ARSTHeartbeatCallback* pHeartbeatCallback);
		virtual ~ARSTHeartbeat();
		void SetTimeInterval(int);
		void TriggerGetReplyEvent();
		void TriggerTerminateEvent();

};
#endif

#ifndef ARSTINTERFACE_ARSTHEARTBEATCALLBACK_H_
#define ARSTINTERFACE_ARSTHEARTBEATCALLBACK_H_

class ARSTHeartbeatCallback 
{
	public:
		virtual void OnHeartbeatLost()=0;
		virtual void OnHeartbeatRequest()=0;

		virtual void OnHeartbeatError(int nData, const char* pErrorMessage)=0;

		virtual ~ARSTHeartbeatCallback(){};
};

#endif

#ifndef ARSTINTERFACE_ARSTCLIENTSOCKETCALLBACK_H_
#define ARSTINTERFACE_ARSTCLIENTSOCKETCALLBACK_H_

class ARSTClientSocketCallback 
{
	public:
		virtual void OnConnect()=0;
		virtual void OnDisconnect()=0;
		virtual void OnData( unsigned char* pBuf, int nSize)=0;
		virtual ~ARSTClientSocketCallback(){};
};

#endif /* ARSTINTERFACE_ARSTSOCKETCALLBACK_H_ */

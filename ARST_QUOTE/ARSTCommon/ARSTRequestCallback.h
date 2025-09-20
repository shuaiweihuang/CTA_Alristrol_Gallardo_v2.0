#ifndef ARSTINTERFACE_ARSTREQUESTCALLBACK_H_
#define ARSTINTERFACE_ARSTREQUESTCALLBACK_H_

class ARSTRequestCallback 
{
	public:
		//virtual void OnRequest(unsigned char* pRequestMessage, int nRequestMessageLength)=0;
		virtual void OnRequest()=0;

		virtual void OnRequestError(int nData, const char* pErrorMessage)=0;

		virtual ~ARSTRequestCallback(){};
};

#endif

#ifndef ARSTINTERFACE_ARSTSOCKETCALLBACK_H_
#define ARSTINTERFACE_ARSTSOCKETCALLBACK_H_

class ARSTSocketCallback 
{
	public:
		virtual void OnListening()=0;
		virtual void OnShutdown()=0;

		virtual ~ARSTSocketCallback(){};
};

#endif /* ARSTINTERFACE_ARSTSOCKETCALLBACK_H_ */

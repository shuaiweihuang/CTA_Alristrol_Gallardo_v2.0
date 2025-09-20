#ifndef ARSTREQUEST_H_
#define ARSTREQUEST_H_

#include "ARSTCommon/ARSTPevents.h"
#include "ARSTCommon/ARSTThread.h"
#include "ARSTCommon/ARSTRequestCallback.h"

class ARSTRequestCallback;

class ARSTRequest : public ARSTThread
{
private:
    ARSTRequestCallback* m_pRequestCallback;
    neosmart::neosmart_event_t m_PEvent[2];

protected:
    void* Run() override;

public:
    explicit ARSTRequest(ARSTRequestCallback* pRequestCallback) noexcept;
    ~ARSTRequest() override;

    void TriggerWakeUpEvent() noexcept;
    void TriggerTerminateEvent() noexcept;
};

#endif // ARSTREQUEST_H_


/*
Run():
 └── while(callback 存在):
       等待事件 (reply / terminate) 最多 5 秒
       ├─ [錯誤] → callback OnHeartbeatError → break
       ├─ [超時] (5秒沒事件)
       │     idle_time += 5
       │     ├─ idle <= interval → continue 等
       │     ├─ interval < idle <= interval+10 → callback OnHeartbeatRequest (送心跳包)
       │     └─ idle > interval+10 → callback OnHeartbeatLost → break
       └─ [有事件觸發]
             ├─ index == 0 → 收到 reply → idle=0
             ├─ index == 1 → callback OnHeartbeatError("TERMINATE") → break
             └─ 其他 index → callback OnHeartbeatError("UNEXPECTED_INDEX") → break
*/

#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include "ARSTHeartbeat.h"

namespace {
    constexpr int WAIT_INTERVAL_MS = 5000;
    constexpr int IDLE_INCREMENT   = 5;
    constexpr int EXTRA_GRACE      = 10;
}

ARSTHeartbeat::ARSTHeartbeat(ARSTHeartbeatCallback* callback)
    : m_nTimeInterval(0),
      m_nIdleTime(0),
      m_pHeartbeatCallback(callback)
{
    m_PEvent[0] = CreateEvent(0, 0); // reply event
    m_PEvent[1] = CreateEvent(0, 0); // terminate event
}

ARSTHeartbeat::~ARSTHeartbeat() {
    TriggerTerminateEvent();
    DestroyEvent(m_PEvent[0]);
    DestroyEvent(m_PEvent[1]);
}

void* ARSTHeartbeat::Run() {
    while (m_pHeartbeatCallback) {
        int nIndex = -1;
        int nResult = WaitForMultipleEvents(m_PEvent, 2, false, WAIT_INTERVAL_MS, nIndex);

        if (nResult != 0 && nResult != WAIT_TIMEOUT) {
            m_pHeartbeatCallback->OnHeartbeatError(0, "HEARTBEAT_ERROR_IN_WAIT");
            break;
        }

        if (nResult == WAIT_TIMEOUT) {
            m_nIdleTime += IDLE_INCREMENT;

            if (m_nIdleTime < m_nTimeInterval) {
                continue;
            }

            if (m_nIdleTime <= m_nTimeInterval + EXTRA_GRACE) {
                m_pHeartbeatCallback->OnHeartbeatRequest();
                continue;
            }

            m_pHeartbeatCallback->OnHeartbeatLost();
            break;
        }

        // Event triggered
        switch (nIndex) {
            case 0: // client reply
                m_nIdleTime = 0;
                break;
            case 1: // terminate
                m_pHeartbeatCallback->OnHeartbeatError(0, "HEARTBEAT_TERMINATE");
                return nullptr;
            default:
                m_pHeartbeatCallback->OnHeartbeatError(nIndex, "HEARTBEAT_UNEXPECTED_INDEX_ERROR");
                return nullptr;
        }
    }
    return nullptr;
}

void ARSTHeartbeat::SetTimeInterval(int nTimeInterval) {
    m_nTimeInterval = nTimeInterval;
}

void ARSTHeartbeat::TriggerGetReplyEvent() {
    SetEvent(m_PEvent[0]);
}

void ARSTHeartbeat::TriggerTerminateEvent() {
    SetEvent(m_PEvent[1]);
}


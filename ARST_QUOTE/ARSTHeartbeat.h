#ifndef ARSTHEARTBEAT_H_
#define ARSTHEARTBEAT_H_

#include <string>
#include "ARSTCommon/ARSTPevents.h"
#include "ARSTCommon/ARSTThread.h"
#include "ARSTCommon/ARSTHeartbeatCallback.h"

using namespace neosmart;
using namespace std;

/*
 * @class ARSTHeartbeat
 * @brief 心跳檢測模組，繼承自 ARSTThread
 *
 * 負責定期檢查連線或服務是否存活，並透過回呼通知上層事件：
 * - 心跳請求 (OnHeartbeatRequest)
 * - 心跳失敗 (OnHeartbeatLost)
 * - 錯誤事件 (OnHeartbeatError)
 */

class ARSTHeartbeat : public ARSTThread
{
private:
    int m_nTimeInterval;                         ///< 心跳檢查間隔（秒）
    int m_nIdleTime;                             ///< 累積閒置時間（秒）
    ARSTHeartbeatCallback* m_pHeartbeatCallback; ///< 回呼介面
    neosmart_event_t m_PEvent[2];                ///< 事件陣列: 0=收到回覆, 1=終止心跳

protected:
    /**
     * @brief 執行緒主函式，處理心跳邏輯
     */
    void* Run() override;

public:
    explicit ARSTHeartbeat(ARSTHeartbeatCallback* pHeartbeatCallback);
    virtual ~ARSTHeartbeat();

    /**
     * @brief 設定心跳間隔
     * @param nTimeInterval 心跳間隔秒數
     */
    void SetTimeInterval(int nTimeInterval);

    /**
     * @brief 觸發收到回覆事件
     */
    void TriggerGetReplyEvent();

    /**
     * @brief 觸發終止心跳事件
     */
    void TriggerTerminateEvent();
};

#endif // ARSTHEARTBEAT_H_


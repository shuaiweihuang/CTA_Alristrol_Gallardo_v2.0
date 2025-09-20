#include <cstring>
#include <array>
#include <string>
#include <cstdio>
#include <sys/time.h>
#include <time.h>

#include "ARSTGlobal.h"

// === Market Codes ===
const char* TS_MARKET_CODE = "TS";
const char* TF_MARKET_CODE = "TF";
const char* OF_MARKET_CODE = "OF";
const char* OS_MARKET_CODE = "OS";

// === Heartbeat Messages ===
const char* HEARTBEAT_REQUEST_MSG = "HTBT";
const char* HEARTBEAT_REPLY_MSG   = "HTBR";

// Buffers
std::array<unsigned char, 6> g_heartbeatRequestBuf{};
std::array<unsigned char, 6> g_heartbeatReplyBuf{};
unsigned char g_uncaHeartbeatRequestBuf[6] = {0};

// === Logon Errors ===
const char* REPEAT_LOGON_STATUS_CODE = "7160";
const char* REPEAT_LOGON_ERROR_CODE  = "M716";

// === Logging Labels ===
const char* g_caWhat[] = {
    "RECV_KEY",
    "RECV_LOGON",
    "RECV_HEARTBEAT",
    "RECV_TS",
    "RECV_TF",
    "RECV_OF",
    "RECV_OS"
};

const char* g_caWhatError[] = {
    "RECV_KEY_ERROR",
    "RECV_LOGON_ERROR",
    "RECV_HEARTBEAT_ERROR",
    "RECV_TS_ERROR",
    "RECV_TF_ERROR",
    "RECV_OF_ERROR",
    "RECV_OS_ERROR"
};

// === Init Function ===
void InitialGlobal()
{
    g_heartbeatRequestBuf.fill(0);
    g_heartbeatReplyBuf.fill(0);

    g_heartbeatRequestBuf[0] = g_heartbeatReplyBuf[0] = ESCAPE_BYTE;
    g_heartbeatRequestBuf[1] = g_heartbeatReplyBuf[1] = HEARTBEAT_BYTE;

    std::memcpy(g_heartbeatRequestBuf.data() + 2,
                HEARTBEAT_REQUEST_MSG,
                std::strlen(HEARTBEAT_REQUEST_MSG));

    std::memcpy(g_heartbeatReplyBuf.data() + 2,
                HEARTBEAT_REPLY_MSG,
                std::strlen(HEARTBEAT_REPLY_MSG));
}


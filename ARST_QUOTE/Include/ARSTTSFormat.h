#ifndef INCLUDE_CVTSFORMAT_H_
#define INCLUDE_CVTSFORMAT_H_

#include <cstddef>  // for size_t

// =======================================
// 真 1 byte真真真真真
// =======================================
#pragma pack(push, 1)

// 真真真
constexpr size_t BROKER_LEN = 4;
constexpr size_t AGENT_LEN = 2;
constexpr size_t KEY_LEN = 13;
constexpr size_t TRADE_TYPE_LEN = 2;
constexpr size_t STEP_MARK_LEN = 1;
constexpr size_t ORDER_TYPE_LEN = 1;
constexpr size_t TX_DATE_LEN = 8;
constexpr size_t ORDER_TIME_LEN = 9;
constexpr size_t ACNO_LEN = 7;
constexpr size_t SUB_ACNO_LEN = 7;
constexpr size_t COM_ID_LEN = 6;
constexpr size_t QTY_LEN_IPVH = 9;
constexpr size_t ORDER_PRICE_LEN_IPVH = 9;
constexpr size_t QTY_LEN_DEFAULT = 8;
constexpr size_t ORDER_PRICE_LEN_DEFAULT = 8;
constexpr size_t BUYSSELL_LEN = 1;
constexpr size_t SALE_NO_LEN = 4;
constexpr size_t ORDER_NO_LEN = 5;
constexpr size_t SWITCH_DATA_LEN = 40;
constexpr size_t USER_DEF_LEN = 128;
constexpr size_t ORDER_ID_LEN = 10;
constexpr size_t PRICE_MARK_LEN = 1;
constexpr size_t OTA_MARK_LEN = 1;
constexpr size_t OTA_ORDER_ID_LEN = 10;
constexpr size_t KEYIN_ID_LEN = 6;
constexpr size_t PRICE_TYPE_LEN = 1;
constexpr size_t ORDER_COND_LEN = 1;
constexpr size_t RESERVED_LEN = 32;

#ifdef IPVH_331
struct CV_TS_ORDER
{
    char broker[BROKER_LEN];
    char agent[AGENT_LEN];
    char key[KEY_LEN];
    char trade_type[TRADE_TYPE_LEN];
    char step_mark[STEP_MARK_LEN];
    char order_type[ORDER_TYPE_LEN];
    char tx_date[TX_DATE_LEN];
    char order_time[ORDER_TIME_LEN];
    char acno[ACNO_LEN];
    char sub_acno[SUB_ACNO_LEN];
    char com_id[COM_ID_LEN];
    char qty[QTY_LEN_IPVH];
    char order_price[ORDER_PRICE_LEN_IPVH];
    char buysell[BUYSSELL_LEN];
    char sale_no[SALE_NO_LEN];
    char order_no[ORDER_NO_LEN];
    char switch_data[SWITCH_DATA_LEN];
    char user_def[USER_DEF_LEN];
    char order_id[ORDER_ID_LEN];
    char price_mark[PRICE_MARK_LEN];
    char ota_mark[OTA_MARK_LEN];
    char ota_order_id[OTA_ORDER_ID_LEN];
    char keyin_id[KEYIN_ID_LEN];
    char price_type[PRICE_TYPE_LEN];
    char order_cond[ORDER_COND_LEN];
    char reserved[RESERVED_LEN];
};
#else
struct CV_TS_ORDER
{
    char broker[BROKER_LEN];
    char agent[AGENT_LEN];
    char key[KEY_LEN];
    char trade_type[1];
    char step_mark[STEP_MARK_LEN];
    char order_type[ORDER_TYPE_LEN];
    char tx_date[TX_DATE_LEN];
    char order_time[ORDER_TIME_LEN];
    char acno[ACNO_LEN];
    char sub_acno[SUB_ACNO_LEN];
    char com_id[COM_ID_LEN];
    char qty[QTY_LEN_DEFAULT];
    char order_price[ORDER_PRICE_LEN_DEFAULT];
    char buysell[BUYSSELL_LEN];
    char sale_no[SALE_NO_LEN];
    char order_no[ORDER_NO_LEN];
    char switch_data[SWITCH_DATA_LEN];
    char user_def[USER_DEF_LEN];
    char order_id[ORDER_ID_LEN];
    char price_mark[PRICE_MARK_LEN];
    char ota_mark[OTA_MARK_LEN];
    char ota_order_id[OTA_ORDER_ID_LEN];
};
#endif

struct CV_TS_REPLY
{
    CV_TS_ORDER original;
    char reply_msg[78];
    char error_code[2];
};

#pragma pack(pop)

#endif // INCLUDE_CVTSFORMAT_H_


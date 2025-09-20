#pragma pack(push, 1) // 1 byte align

struct TCVLogon
{
    char logon_id[10];
    char password[50];
    char agent[2];
    char version[10];
};

struct TCVLogonReply
{
    char status_code[4];
    char error_code[4];
    char error_message[512];
    char backup_ip[20];
};

#pragma pack(pop)


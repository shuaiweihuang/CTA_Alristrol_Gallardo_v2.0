#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <assert.h>
#include <glib.h>
#include <unistd.h>

#include "ARSTServer.h"
#include "ARSTWebConns.h"
#include "ARSTQueueManager.h"

using namespace std;

void InitialGlobal();

extern void FprintfStderrLog(const char* pCause, int nError, int nData, const char* pFile = NULL, int nLine = 0,
			     unsigned char* pMessage1 = NULL, int nMessage1Length = 0, 
			     unsigned char* pMessage2 = NULL, int nMessage2Length = 0);

bool ReadConfigFile(string strConfigFileName, string strSection, struct ARSTConfig &struConfig);
bool ReadClientConfigFile(string strConfigFileName, string& strListenPort, string& strHeartBeatTime, string &strEPIDNum);
bool ReadQueueDAOConfigFile(string strConfigFileName,string&, int&, key_t&, key_t&, key_t&, key_t&, key_t&);
#define DECLARE_CONFIG_DATA(CONFIG)\
	struct ARSTConfig stru##CONFIG;\
	memset(&stru##CONFIG, 0, sizeof(struct ARSTConfig));\
	;

int main()
{
	int nNumberOfQueueDAO;
	key_t kQueueDAOWriteStartKey;
	key_t kQueueDAOWriteEndKey;
	key_t kQueueDAOReadStartKey;
	key_t kQueueDAOReadEndKey;
	key_t kQueueDAOMonitorKey;
	string strService;

	InitialGlobal();
	setbuf(stdout, NULL);
	signal(SIGPIPE, SIG_IGN );
	srand(time(NULL));

	DECLARE_CONFIG_DATA(TSConfig);

	int nService = 0;
	string strListenPort, strHeartBeatTime, strEPIDNum;
	if (!ReadClientConfigFile(
		"../ini/ARSTQuote.ini",  // 可以直接傳字串字面量
		strListenPort,
		strHeartBeatTime,
		strEPIDNum))
	{
	    std::cerr << "Failed to load client config file" << std::endl;
	    exit(1);
	}

	printf("Quote program start\n");

	if (!ReadConfigFile("../ini/ARSTQuote.ini", "EXCHANGE", struTSConfig)) {
	    std::cerr << "Failed to read EXCHANGE config" << std::endl;
	    exit(1);
	}
	//Web connection service.
	ARSTServers* pServers = NULL;
	try
	{
		pServers = ARSTServers::GetInstance();
		if(pServers == NULL)
			throw "GET_WEB_ERROR";

		pServers->SetConfiguration(&struTSConfig);
		pServers->StartUpServers();
	}
	catch (const char* pErrorMessage)
	{
		FprintfStderrLog(pErrorMessage, -1, 0, __FILE__, __LINE__);
	}
	
	if (!ReadQueueDAOConfigFile(
		"../ini/ARSTQuote.ini",  // 直接使用字串字面量
		strService,
		nNumberOfQueueDAO,
		kQueueDAOWriteStartKey,
		kQueueDAOWriteEndKey,
		kQueueDAOReadStartKey,
		kQueueDAOReadEndKey,
		kQueueDAOMonitorKey))
	{
	    std::cerr << "Failed to load QueueDAO config file" << std::endl;
	    exit(1);
	}

	ARSTQueueDAOManager* pQueueDAOManager = ARSTQueueDAOManager::GetInstance();
	assert(pQueueDAOManager);

	pQueueDAOManager->SetConfiguration(strService, nNumberOfQueueDAO, kQueueDAOWriteStartKey, kQueueDAOWriteEndKey, kQueueDAOReadStartKey, kQueueDAOReadEndKey, kQueueDAOMonitorKey);
	pQueueDAOManager->StartUpDAOs();

	//Server connection service.
	ARSTClients* pClients = NULL;
	try
	{
		pClients = ARSTClients::GetInstance();
		if(pClients == NULL)
			throw "GET_SERVER_MANAGEMENT_ERROR";

		pClients->SetConfiguration(strListenPort, strHeartBeatTime, strEPIDNum, nService);

		while(1)
		{
			pClients->CheckClientVector();
			pServers->CheckClientVector();
			sleep(1);
		}
	}
	catch (const char* pErrorMessage)
	{
		FprintfStderrLog(pErrorMessage, -1, 0, __FILE__, __LINE__);
	}

	return 0;
}

bool ReadConfigFile(string strConfigFileName, string strSection, struct ARSTConfig &struConfig)
{
    GKeyFile* keyfile = g_key_file_new();
    if (!keyfile) {
        std::cerr << "Failed to create GKeyFile object" << std::endl;
        return false;
    }

    GKeyFileFlags flags = static_cast<GKeyFileFlags>(
        G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS
    );

    GError* error = nullptr;
    if (!g_key_file_load_from_file(keyfile, strConfigFileName.c_str(), flags, &error)) {
        std::cerr << "Failed to load config file: " 
                  << strConfigFileName << "\nError: " 
                  << (error ? error->message : "Unknown") << std::endl;
        if (error) g_error_free(error);
        g_key_file_free(keyfile);
        return false;
    }

    // 讀取基本配置
    struConfig.nServerCount = g_key_file_get_integer(keyfile, strSection.c_str(), "ServerCount", &error);
    if (error) { std::cerr << error->message << std::endl; g_error_free(error); error = nullptr; }

    struConfig.nServerSet = g_key_file_get_integer(keyfile, strSection.c_str(), "ServerSet", &error);
    if (error) { std::cerr << error->message << std::endl; g_error_free(error); error = nullptr; }

    for (int i = 0; i < struConfig.nServerCount; i++) {
        char keyIP[16], keyPort[16], keyName[16];
        snprintf(keyIP, sizeof(keyIP), "IP%02d", i+1);
        snprintf(keyPort, sizeof(keyPort), "PORT%02d", i+1);
        snprintf(keyName, sizeof(keyName), "NAME%02d", i+1);

	ARSTServerInfo* serverInfo = new ARSTServerInfo();

        serverInfo->strHost = g_key_file_get_string(keyfile, strSection.c_str(), keyIP, &error);
        if (error) { std::cerr << error->message << std::endl; g_error_free(error); error = nullptr; }

        serverInfo->strPara = g_key_file_get_string(keyfile, strSection.c_str(), keyPort, &error);
        if (error) { std::cerr << error->message << std::endl; g_error_free(error); error = nullptr; }

        serverInfo->strName = g_key_file_get_string(keyfile, strSection.c_str(), keyName, &error);
        if (error) { std::cerr << error->message << std::endl; g_error_free(error); error = nullptr; }

        std::cout << "Server: " << serverInfo->strName
                  << " Connect to: " << serverInfo->strHost
                  << ":" << serverInfo->strPara << std::endl;

        struConfig.vServerInfo.push_back(serverInfo);
    }

    g_key_file_free(keyfile);
    return true;
}


bool ReadClientConfigFile(
    string strConfigFileName, 
    string& strListenPort, 
    string& strHeartBeatTime, 
    string& strEPIDNum
)
{
    GKeyFile* keyfile = g_key_file_new();
    if (!keyfile) {
        std::cerr << "Failed to create GKeyFile object" << std::endl;
        return false;
    }

    GKeyFileFlags flags = static_cast<GKeyFileFlags>(
        G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS
    );

    GError* error = nullptr;
    if (!g_key_file_load_from_file(keyfile, strConfigFileName.c_str(), flags, &error)) {
        std::cerr << "Failed to load client config file: " 
                  << strConfigFileName << "\nError: " 
                  << (error ? error->message : "Unknown") << std::endl;
        if (error) g_error_free(error);
        g_key_file_free(keyfile);
        return false;
    }

    // 讀取配置值
    strEPIDNum = g_key_file_get_string(keyfile, "SERVER", "EpidNum", &error);
    if (error) { std::cerr << error->message << std::endl; g_error_free(error); error = nullptr; }

    strListenPort = g_key_file_get_string(keyfile, "SERVER", "ListenPort", &error);
    if (error) { std::cerr << error->message << std::endl; g_error_free(error); error = nullptr; }

    strHeartBeatTime = g_key_file_get_string(keyfile, "SERVER", "HeartBeatTime", &error);
    if (error) { std::cerr << error->message << std::endl; g_error_free(error); }

    g_key_file_free(keyfile);
    return true;
}

bool ReadQueueDAOConfigFile(
    string strConfigFileName,
    string& strService,
    int& nNumberOfQueueDAO,
    key_t& kQueueDAOWriteStartKey,
    key_t& kQueueDAOWriteEndKey,
    key_t& kQueueDAOReadStartKey,
    key_t& kQueueDAOReadEndKey,
    key_t& kQueueDAOMonitorKey
)
{
    GKeyFile* keyfile = g_key_file_new();
    if (!keyfile) {
        std::cerr << "Failed to create GKeyFile object\n";
        return false;
    }

    GKeyFileFlags flags = static_cast<GKeyFileFlags>(
        G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS
    );

    GError* error = nullptr;
    if (!g_key_file_load_from_file(keyfile, strConfigFileName.c_str(), flags, &error)) {
        std::cerr << "Failed to load config file: " 
                  << strConfigFileName << "\nError: " 
                  << (error ? error->message : "Unknown") << std::endl;
        if (error) g_error_free(error);
        g_key_file_free(keyfile);
        return false;
    }

    // 讀取配置值
    strService = g_key_file_get_string(keyfile, "QUEUE", "Service", &error);
    if (error) { std::cerr << error->message << std::endl; g_error_free(error); error = nullptr; }

    nNumberOfQueueDAO = g_key_file_get_integer(keyfile, "QUEUE", "NumberOfQueueDAO", &error);
    if (error) { std::cerr << error->message << std::endl; g_error_free(error); error = nullptr; }

    kQueueDAOWriteStartKey = g_key_file_get_integer(keyfile, "QUEUE", "QueueNodeWriteStartKey", &error);
    if (error) { std::cerr << error->message << std::endl; g_error_free(error); error = nullptr; }

    kQueueDAOWriteEndKey = g_key_file_get_integer(keyfile, "QUEUE", "QueueNodeWriteEndKey", &error);
    if (error) { std::cerr << error->message << std::endl; g_error_free(error); error = nullptr; }

    kQueueDAOReadStartKey = g_key_file_get_integer(keyfile, "QUEUE", "QueueNodeReadStartKey", &error);
    if (error) { std::cerr << error->message << std::endl; g_error_free(error); error = nullptr; }

    kQueueDAOReadEndKey = g_key_file_get_integer(keyfile, "QUEUE", "QueueNodeReadEndKey", &error);
    if (error) { std::cerr << error->message << std::endl; g_error_free(error); error = nullptr; }

    kQueueDAOMonitorKey = g_key_file_get_integer(keyfile, "QUEUE", "QueueNodeMonitorKey", &error);
    if (error) { std::cerr << error->message << std::endl; g_error_free(error); }

    g_key_file_free(keyfile);

    return true;
}

void mem_usage(double& vm_usage, double& resident_set)
{
	vm_usage = 0.0;
	resident_set = 0.0;
	ifstream stat_stream("/proc/self/stat",ios_base::in);
	string pid, comm, state, ppid, pgrp, session, tty_nr;
	string tpgid, flags, minflt, cminflt, majflt, cmajflt;
	string utime, stime, cutime, cstime, priority, nice;
	string O, itrealvalue, starttime;
	unsigned long vsize;
	long rss;
	stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
	>> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
	>> utime >> stime >> cutime >> cstime >> priority >> nice
	>> O >> itrealvalue >> starttime >> vsize >> rss;
	stat_stream.close();
	long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024;
	vm_usage = vsize / (1024.0*1024.0);
	resident_set = rss * page_size_kb;
}

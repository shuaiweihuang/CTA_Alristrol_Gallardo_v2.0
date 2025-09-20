#include <iostream>
#include <iomanip>
#include <ctime>
#include <sys/time.h>
#include <cctype>

void FprintfStderrLog(
    const char* pCause,
    int nError,
    int nData,
    const char* pFile = nullptr,
    int nLine = 0,
    unsigned char* pMessage1 = nullptr,
    int nMessage1Length = 0,
    unsigned char* pMessage2 = nullptr,
    int nMessage2Length = 0 )
{
    // 取得當前時間
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    struct tm tm = *localtime(&tv.tv_sec);

    // 時間戳輸出
    std::cerr << "[" 
              << std::setw(2) << std::setfill('0') << tm.tm_hour << ":"
              << std::setw(2) << std::setfill('0') << tm.tm_min << ":"
              << std::setw(2) << std::setfill('0') << tm.tm_sec << "'"
              << std::setw(6) << std::setfill('0') << tv.tv_usec << "]";

    // 基本訊息輸出
    std::cerr << "[" << pCause << "][" << nError << "," << nData << "]";

    // 檔案與行號
    if (pFile) {
        std::cerr << pFile << ":" << nLine;
    }

    // 第一段訊息
    std::cerr << "{";
    for (int i = 0; i < nMessage1Length && pMessage1; ++i) {
        unsigned char c = pMessage1[i];
        if (std::isprint(c)) {
            std::cerr << c;
        } else {
            std::cerr << "[" << std::hex << std::showbase
                      << static_cast<int>(c) << std::dec << "]";
        }
    }
    std::cerr << "}";

    // 第二段訊息
    std::cerr << "{";
    for (int i = 0; i < nMessage2Length && pMessage2; ++i) {
        unsigned char c = pMessage2[i];
        if (std::isprint(c)) {
            std::cerr << c;
        } else {
            std::cerr << "[" << std::hex << std::showbase
                      << static_cast<int>(c) << std::dec << "]";
        }
    }
    std::cerr << "}" << std::endl;
}


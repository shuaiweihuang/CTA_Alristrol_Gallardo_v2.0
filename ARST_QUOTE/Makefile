# ========================================
# 全局編譯設定
# ========================================
CC = g++
CFLAGS = -O2 -std=c++17 -DDEBUG_OFF -DEXIT_VERSION_OFF \
         -Wunused-command-line-argument -Wno-deprecated-declarations \
         -D_WEBSOCKETPP_CPP11_STL_

# Boost 路徑
BOOST_ROOT ?= /usr/local/boost_1_72_0

# glib include 路徑（Intel Mac Homebrew 預設）
GLIB_CFLAGS = -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include
GLIB_LIBS   = -lglib-2.0

# include 路徑
INC = -I./Include -IARSTQuoteFR -I$(BOOST_ROOT)/include $(GLIB_CFLAGS)

# 庫設定
LIB = -lssl -lcrypto -pthread \
      -L$(BOOST_ROOT)/lib -lboost_system -lboost_thread -lboost_chrono \
      -L/usr/local/lib $(GLIB_LIBS)

# 輸出路徑
OUTPUT_PATH = ../bin

# ========================================
# 源碼與目標檔案
# ========================================
SRCS = ARSTMain.cpp \
       ARSTGlobal.cpp \
       ARSTDumpErrLog.cpp \
       ARSTHeartbeat.cpp \
       ARSTQueueNode.cpp \
       ARSTQueueNodes.cpp \
       ARSTRequest.cpp \
       ARSTServer.cpp \
       ARSTServiceHandler.cpp \
       ARSTWebConn.cpp \
       ARSTWebConns.cpp \
       ARSTCommon/ARSTQueue.cpp \
       ARSTCommon/ARSTThread.cpp \
       ARSTCommon/ARSTClientSocket.cpp \
       ARSTCommon/ARSTServerSocket.cpp \
       ARSTCommon/ARSTPevents.cpp

OBJS = $(SRCS:.cpp=.o)
TARGET = $(OUTPUT_PATH)/arst_quote

# ========================================
# 編譯規則
# ========================================
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIB)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)


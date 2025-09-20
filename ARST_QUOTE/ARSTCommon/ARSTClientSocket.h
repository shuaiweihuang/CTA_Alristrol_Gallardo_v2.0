#ifndef ARSTCLIENTSOCKET_H_
#define ARSTCLIENTSOCKET_H_

#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <nlohmann/json.hpp>

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

using json = nlohmann::json;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using namespace std;

enum ARSTClientSocketStatus 
{
	cssNone,
	cssConnecting,
	cssConnected,
	cssDisconnect
};

class ARSTClientSocketCallback;

class ARSTClientSocket 
{
	private:

		ARSTClientSocketCallback* m_pClientSocketCallback;

		int m_nSocket;

		ARSTClientSocketStatus m_cssClientSocketStatus;

		struct addrinfo m_AddrInfo;
		struct addrinfo* m_AddrRes;
	protected:
		void Close();

	public:
		ARSTClientSocket();
		ARSTClientSocket(ARSTClientSocketCallback* pClientSocketCallback);
		virtual ~ARSTClientSocket();

		void Connect(string strHost, string strPort, string strName, int type);
		void Disconnect();

		int Send(const unsigned char* pBuf, int nSize);
		int Recv( unsigned char* pBuf, int nSize);

		int Recv();

		ARSTClientSocketStatus GetStatus();
};
#endif /* ARSTSOCKET_H_ */

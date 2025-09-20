#ifndef ARSTSERVERSOCKET_H_
#define ARSTSERVERSOCKET_H_

#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;

enum ARSTServerSocketStatus 
{
	sssNone,
	//ssStartListeningUp,
	sssListening,
	sssShutdown
};

class ARSTSocketCallback;

class ARSTServerSocket 
{
	private:

		ARSTSocketCallback* m_pSocketCallback;

		int m_nSocket;
		socklen_t m_AddrSize;

		ARSTServerSocketStatus m_ServerSocketStatus;

		struct addrinfo m_AddrInfo;
		struct addrinfo* m_AddrRes;

	protected:
		void Close();

	public:
		ARSTServerSocket();
		ARSTServerSocket( ARSTSocketCallback* pSocketCallback);
		virtual ~ARSTServerSocket();

		void Listen(string strPort, int nBacklog = 20);
		int Accept(struct sockaddr_storage* pClientAddr);

		void ShutdownServer();
		void ShutdownClient(int nSocket);

		int Send(int nSocket, const unsigned char* pBuf, int nSize);
		//int Recv( unsigned char* pBuf, int nSize);

		ARSTServerSocketStatus GetStatus();
};
#endif

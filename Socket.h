// Definition of the Socket class

#ifndef EF_SOCKET_H
#define EF_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <sys/un.h>


const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

class Socket
{
public:
    Socket();
    virtual ~Socket();

  // Server initialization
    bool create();
    bool create(int type);
    bool bind (const int port);
    bool listen() const;
    bool accept (Socket&) const;

  // Client initialization
    bool connect ();
    bool connect (const std::string host, const int port);


  // Data Transimission
    bool send (const std::string ) const;
    int recv (std::string& ) const;

    void set_non_blocking (const bool);

    bool is_valid() const {return m_sock_ != -1;}

private:
    int m_sock_;
    sockaddr_in m_addr_;
    sockaddr_un m_addr_un_;
};

#endif

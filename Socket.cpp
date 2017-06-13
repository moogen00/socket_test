#include "Socket.h"
//#include "string.h"
#include <iostream>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define  FILE_SERVER "/tmp/test_server"

Socket::Socket(void)
: m_sock_(-1) {
    memset(&m_addr_, 0, sizeof(m_addr_));
    memset(&m_addr_un_, 0, sizeof(m_addr_un_));
}

Socket::~Socket() {
    if (is_valid()) {
        ::close (m_sock_);
    }
}

bool Socket::create() {
    //m_sock_ = socket(AF_INET, SOCK_STREAM, 0);
    if ( 0 == access( FILE_SERVER, F_OK))
         unlink(FILE_SERVER);

    m_sock_ = socket(PF_FILE, SOCK_STREAM, 0);

    if(!is_valid())
        return false;


    // TIME_WAIT - argh
    int on = 1;
    if (setsockopt(m_sock_, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on)) == -2) {
        return false;
    }

    return true;
}


bool Socket::create(int type) {
    //m_sock_ = socket(AF_INET, SOCK_STREAM, 0);
    if(type == 0) {
        if ( 0 == access( FILE_SERVER, F_OK))
            unlink(FILE_SERVER);
    }

    m_sock_ = socket(PF_FILE, SOCK_STREAM, 0);

    if(!is_valid())
        return false;


    // TIME_WAIT - argh
    int on = 1;
    if (setsockopt(m_sock_, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on)) == -2) {
        return false;
    }

    return true;
}




bool Socket::bind(const int port) {

    if (!is_valid()) {
        return false;
    }

    /*
    m_addr_.sin_family = AF_INET;
    m_addr_.sin_addr.s_addr = INADDR_ANY;
    m_addr_.sin_port = htons(port);
    */

    m_addr_un_.sun_family = AF_UNIX;
    strcpy(m_addr_un_.sun_path, FILE_SERVER);

    //int bind_return = ::bind(m_sock_, (struct sockaddr *)&m_addr_, sizeof(m_addr_));
    int bind_return = ::bind(m_sock_, (struct sockaddr *)&m_addr_un_, sizeof(m_addr_un_));

    if (bind_return == -1) {
        return false;
    }

    return true;
}


bool Socket::listen() const {
    if (!is_valid()) {
        return false;
    }

    int listen_return = ::listen(m_sock_, MAXCONNECTIONS);


    if (listen_return == -1) {
        return false;
    }

    return true;
}


bool Socket::accept(Socket& new_socket) const
{
    //int addr_length = sizeof ( m_addr_ );
    //new_socket.m_sock_ = ::accept (m_sock_, (sockaddr *)&m_addr_, (socklen_t *)&addr_length);

    int addr_length = sizeof(m_addr_un_);
    new_socket.m_sock_ = ::accept (m_sock_, (sockaddr *)&m_addr_un_, (socklen_t *)&addr_length);


    if (new_socket.m_sock_ <= 0 )
        return false;
    else
        return true;
}


bool Socket::send(const std::string s) const
{
    int status = ::send(m_sock_, s.c_str(), s.size(), MSG_NOSIGNAL);
    if (status == -1) {
        return false;
    }
    else {
        return true;
    }
}


int Socket::recv(std::string& s) const {
    char buf [ MAXRECV + 1 ];
    s = "";

    memset(buf, 0, MAXRECV + 1);
    int status = ::recv(m_sock_, buf, MAXRECV, 0 );

    if (status == -1) {
        std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
        return 0;
    } else if ( status == 0 ) {
        return 0;
    } else {
        s = buf;
        return status;
    }
}



bool Socket::connect (const std::string host, const int port) {

    std::cout << "connect 1 \n";
    if ( ! is_valid() ) return false;

    m_addr_.sin_family = AF_INET;
    m_addr_.sin_port = htons ( port );

    //m_addr_un_.sun_family = AF_UNIX;
    //strcpy(m_addr_un_.sun_path, FILE_SERVER);

    int status = inet_pton ( AF_INET, host.c_str(), &m_addr_.sin_addr );

    if ( errno == EAFNOSUPPORT ) return false;

    status = ::connect ( m_sock_, ( sockaddr * ) &m_addr_, sizeof ( m_addr_ ) );
    //status = ::connect ( m_sock_, ( sockaddr * )&m_addr_un_, sizeof (m_addr_un_));

    if ( status == 0 )
        return true;
    else
        return false;
}

bool Socket::connect(void) {
    std::cout << "connect 2 \n";
    if ( ! is_valid() ) {
        std::cout << "connect 2 is_valid false\n";
        return false;
    }

    //m_addr_.sin_family = AF_INET;
    //m_addr_.sin_port = htons ( port );

    m_addr_un_.sun_family = AF_UNIX;
    strcpy(m_addr_un_.sun_path, FILE_SERVER);

    // int status = inet_pton ( AF_INET, host.c_str(), &m_addr_.sin_addr );
    int status = 0;
    if ( errno == EAFNOSUPPORT ) return false;

    //status = ::connect ( m_sock_, ( sockaddr * ) &m_addr_, sizeof ( m_addr_ ) );
    status = ::connect(m_sock_, (sockaddr *)&m_addr_un_, sizeof(m_addr_un_));

    if ( status == 0 )
        return true;
    else
        return false;
}


void Socket::set_non_blocking ( const bool b ) {

    int opts;

    opts = fcntl ( m_sock_, F_GETFL );

    if ( opts < 0 )
    {
        return;
    }

    if ( b )
        opts = ( opts | O_NONBLOCK );
    else
        opts = ( opts & ~O_NONBLOCK );

    fcntl ( m_sock_, F_SETFL,opts );

}

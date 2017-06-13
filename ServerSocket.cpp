// Implementation of the ServerSocket class

#include "ServerSocket.h"
#include "SocketException.h"
#include <iostream>

ServerSocket::ServerSocket ( int port )
{
    std::cout << "ServerSocket start \n";
    //if ( ! Socket::create() )
    if ( ! Socket::create(0) )
    {
      throw SocketException ( "Could not create server socket." );
    }

  if ( ! Socket::bind ( port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }

  if ( ! Socket::listen() )
    {
      throw SocketException ( "Could not listen to socket." );
    }
    std::cout << "ServerSocket end \n";

}

ServerSocket::~ServerSocket()
{
    std::cout << "~ServerSocket \n";
}


const ServerSocket& ServerSocket::operator << ( const std::string& s ) const
{
    std::cout << "operator << send \n";
  if ( ! Socket::send ( s ) )
    {
      throw SocketException ( "Could not write to socket." );
    }

  return *this;

}


const ServerSocket& ServerSocket::operator >> ( std::string& s ) const
{
    std::cout << "operator >> receive \n";
  if ( ! Socket::recv ( s ) )
    {
      throw SocketException ( "Could not read from socket." );
    }

  return *this;
}

void ServerSocket::accept ( ServerSocket& sock )
{
    std::cout << "operator >> accept \n";

  if ( ! Socket::accept ( sock ) )
    {
      throw SocketException ( "Could not accept socket." );
    }
}

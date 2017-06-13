// Implementation of the ClientSocket class

#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>


ClientSocket::ClientSocket ( std::string host, int port )
{
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create client socket." );
    }

  //if ( ! Socket::connect ( host, port ) )
    if ( ! Socket::connect ( host, port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }

}


ClientSocket::ClientSocket ()
{
    //if ( ! Socket::create() )
    if ( ! Socket::create(1) )
    {
      throw SocketException ( "Could not create client socket." );
    }

    std::cout << "Socket::connect() \n";
    if ( ! Socket::connect() )
    {
      throw SocketException ( "Could not bind to port." );
    }

}



const ClientSocket& ClientSocket::operator << ( const std::string& s ) const
{
  if ( ! Socket::send ( s ) )
    {
      throw SocketException ( "Could not write to socket." );
    }

  return *this;

}


const ClientSocket& ClientSocket::operator >> ( std::string& s ) const
{
  if ( ! Socket::recv ( s ) )
    {
      throw SocketException ( "Could not read from socket." );
    }

  return *this;
}

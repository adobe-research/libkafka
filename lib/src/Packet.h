//////////////////////////////////////////////////////////////////
//                                                              //
// libkafka - C/C++ client for Apache Kafka v0.8+               //
//                                                              //
// David Tompkins -- 8/8/2013                                   //
// http://dt.org/                                               //
//                                                              //
// Copyright (c) 2013 by David Tompkins.                        //
//                                                              //
//////////////////////////////////////////////////////////////////
//                                                              //
// This program is free software; you can redistribute it       //
// and/or modify it under the terms of the GNU General Public   //
// License as published by the Free Software Foundation.        //
//                                                              //
// This program is distributed in the hope that it will be      //
// useful, but WITHOUT ANY WARRANTY; without even the implied   //
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      //
// PURPOSE. See the GNU General Public License for more details //
//                                                              //
// You should have received a copy of the GNU General Public    //
// License along with this program; if not, write to the Free   //
// Software Foundation, Inc., 59 Temple Place, Suite 330,       //
// Boston, MA 02111-1307 USA                                    //
//                                                              //
//////////////////////////////////////////////////////////////////

#include <string>
#include <Debug.h>
#include <WireFormatter.h>

using namespace std;

#define DEFAULT_BUFFER_SIZE 1024

class Packet : public WireFormatter
{
  public:

    int size;

    Packet(int bufferSize = DEFAULT_BUFFER_SIZE);
    Packet(unsigned char *buffer);
    ~Packet();

    short int readInt16();
    int readInt32();
    long int readInt64();
    string readString();

    void writeInt16(short int value);
    void writeInt32(int value);
    void writeInt64(int long value);
    void writeString(string value);
    
    void updatePacketSize();

    unsigned char* toWireFormat(bool updateSize = true);

  protected:

    unsigned char *buffer;
    unsigned char *head;

  private:

    bool releaseBuffer;
};

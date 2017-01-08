#include "39dll.h"
/*#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#else
#include <android/log.h>
#endif*/

CList buffers;
CList sockets;
CList files;
int AddSocket(CSocket*);
int AddBuffer(CBuffer*);

//Sockets
double tcpconnect(std::string ip, double port, double mode)
{
	CSocket* sock = new CSocket();

	char *ipAddress = new char[ip.size() + 1];
	ipAddress[ip.size()] = 0;
	memcpy(ipAddress, ip.c_str(), ip.size());

	if (sock->tcpconnect(ipAddress, (int)port, (int)mode))
		return AddSocket(sock);
	delete sock;
	return -1;
}

double tcplisten(double port, double max, double mode)
{
	CSocket* sock = new CSocket();
	if (sock->tcplisten((int)port, (int)max, (int)mode))
		return AddSocket(sock);
	delete sock;
	return -1;
}

double tcpaccept(double sockid, double mode)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if (sock == NULL)return -1;
	CSocket*sock2 = sock->tcpaccept((int)mode);
	if (sock2 != NULL)return AddSocket(sock2);
	return -1;
}

char* tcpip(double sockid)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if (sock == NULL)return NULL;
	return sock->tcpip();
}

double setnagle(double sockid, double value)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if (sock == NULL)return 0;
	if (value)
		sock->setnagle(true);
	else sock->setnagle(false);
	return 1;
}

double tcpconnected(double sockid)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if (sock == NULL)return 0;
	if (sock->tcpconnected())return 1;
	else return 0;
}

double udpconnect(double port, double mode)
{
	CSocket* sock = new CSocket();
	if (sock->udpconnect((int)port, (int)mode))
		return AddSocket(sock);
	delete sock;
	return -1;
}

double sendmessage(double sockid, char*ip, double port, double buffid)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (sock == NULL)return -1;
	if (buff == NULL)return -2;
	int size = sock->sendmessage(ip, (int)port, buff);
	if (size < 0)return -sock->lasterror();
	return size;
}

double receivemessage(double sockid, double len, double buffid)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (sock == NULL)return -1;
	if (buff == NULL)return -2;
	int size = sock->receivemessage((int)len, buff);

	if (size < 0)
	{
		int error = sock->lasterror();
		if (error == 10054)return 0;
#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
		return -error; //windows error are +, so we need to return -
#else
		return error; //because SOCKET_ERROR   -1 ...
		//__android_log_print(ANDROID_LOG_INFO, "debug info", "error 39dll: %d", error);
#endif
		
	}
	return size;
}

double peekmessage(double sockid, double len, double buffid)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (sock == NULL)return -1;
	if (buff == NULL)return -2;
	int size = sock->peekmessage((int)len, buff);
	if (size < 0)
	{
		int error = sock->lasterror();
		if (error == 10054)return 0;
		return -error;
	}
	return size;
}

double setformat(double sockid, double mode, char*sep)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if (sock == NULL)return -1;
	return sock->SetFormat((int)mode, sep);
}

char* lastinIP()
{
	return CSocket::lastinIP();
}

double lastinPort()
{
	return CSocket::lastinPort();
}

double setsync(double sockid, double mode)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if (sock == NULL)return -1;
	sock->setsync((int)mode);
	return 1;
}

double closesock(double sockid)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if (sock == NULL)return -1;
	delete sock;
	sockets.set((int)sockid, NULL);
	return 1;
}

double socklasterror(double sockid)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if (sock == NULL)return -1;
	return -sock->lasterror();
}

char* myhost()
{
	return CSocket::myhost();
}

double compareip(char*ip, char*mask)
{
	return CTools::comparematch(ip, mask);
}

double sockexit()
{
	CSocket::SockExit();
	return 1;
}

double sockstart()
{
	CSocket::SockStart();
	return 1;
}

char* hostip(char*host)
{
	return CSocket::GetIp(host);
}


double getsocketid(double sockid)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if (sock == NULL)return -1;
	return sock->sockid;
}

//Buffers
int writebyte(double val, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->writebyte((unsigned char)val);
}

int writeshort(double val, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->writeshort((short)val);
}

int writeushort(double val, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->writeushort((unsigned short)val);
}

int writeint(double val, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->writeint((int)val);
}

int writeuint(double val, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->writeuint((unsigned int)val);
}

int writefloat(double val, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->writefloat((float)val);
}

int writedouble(double val, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->writedouble(val);
}

int writechars(char*str, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->writechars(str);
}

int writestring(std::string str, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->writestring(str);
}

unsigned char readbyte(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->readbyte();
}

short readshort(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->readshort();
}

unsigned short readushort(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->readushort();
}

int readint(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->readint();
}

unsigned int readuint(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->readuint();
}

float readfloat(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->readfloat();
}

double readdouble(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->readdouble();
}

char* readchars(double len, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return NULL;
	return buff->readchars((int)len);
}

char* readstring(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return NULL;
	return buff->readstring();
}

char* readsep(char*sep, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return NULL;
	return buff->readsep(sep);
}

double getpos(double pos, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	if (pos == 0)return buff->writepos;
	else return buff->readpos;
}

double clearbuffer(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	buff->clear();
	return 1;
}

double buffsize(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->count;
}

double setpos(double pos, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	buff->readpos = (int)pos;
	buff->writepos = (int)pos;
	return pos;
}

double bytesleft(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff->bytesleft();
}

double createbuffer()
{
	CBuffer* buff = new CBuffer();
	return AddBuffer(buff);
}

double freebuffer(double buffid)
{
	if (buffid == 0)return 0; //no deleting default buffer
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	delete buff;
	buffers.set((int)buffid, NULL);
	return 1;
}

double copybuffer(double destinationid, double sourceid)
{
	CBuffer*destination = (CBuffer*)buffers.item((int)destinationid);
	if (destination == NULL)return 0;
	CBuffer*source = (CBuffer*)buffers.item((int)sourceid);
	if (source == NULL)return 0;
	destination->addBuffer(source);
	return 1;

}

double copybuffer2(double destinationid, double start, double len, double sourceid)
{
	CBuffer*destination = (CBuffer*)buffers.item((int)destinationid);
	if (destination == NULL)return 0;
	CBuffer*source = (CBuffer*)buffers.item((int)sourceid);
	if (source == NULL)return 0;
	destination->addBuffer(source->data + (int)start, (int)len);
	return 1;

}

//tools
/*char* getmacaddress()
{
	return CTools::getmacaddress();
}*/

std::string md5string(std::string str)
{
	return CTools::md5string(str);
}

char* md5buffer(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return NULL;
	return CTools::md5buffer(buff);
}

double bufferencrypt(std::string pass, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	CTools::encryptedbuffer(buff, pass);
	return 1;
}

double adler32(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return CTools::adler32(buff);
}

double bufferexists(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return 1;
}

double netconnected()
{
	return CTools::netconnected();
}


double iptouint(char*ip)
{
	return CTools::ip2uint(ip);
}

char* uinttoip(double ip)
{
	return CTools::uint2ip((unsigned int)ip);
}

int AddBuffer(CBuffer*b)
{
	for (int i = 0; i < buffers.count; i++)
	{
		if (buffers[i] == NULL)
		{
			buffers.set(i, b);
			return i;
		}
	}
	buffers.Add(b);
	return buffers.count - 1;
}

int AddSocket(CSocket*b)
{
	for (int i = 0; i < sockets.count; i++)
	{
		if (sockets[i] == NULL)
		{
			sockets.set(i, b);
			return i;
		}
	}
	sockets.Add(b);
	return sockets.count - 1;
}

double dllInit()
{
	CSocket::SockStart();
	buffers.Add(new CBuffer());
	sockets.Add(new CSocket()); //dummy socket
	return 1;
}

CBuffer* getBuffer(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if (buff == NULL)return 0;
	return buff;
}
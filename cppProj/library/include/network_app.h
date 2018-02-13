#ifndef _NETWORK_APP_H_
#define _NETWORK_APP_H_
#include <string>

class NetworkApp
{
public:
	static bool ping(std::string adress);
	NetworkApp(void);
	~NetworkApp(void);
protected:
	static unsigned short check_sum(void *b, int len);
private:
};

#endif //ndef _NETWORK_APP_H_

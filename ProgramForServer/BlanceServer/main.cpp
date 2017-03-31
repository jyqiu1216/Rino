

#include "BalanceServer.h"

int main(int argc, char *argv[])
{
	initLoggingSystem(__argc, __argv);
	CBalanceServer balanceserver;
	balanceserver.StartService();
	balanceserver.StopService();
	return 0;
}


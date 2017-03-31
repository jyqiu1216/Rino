#pragma  once

namespace WJ
{
	//e_hs_mess_apply = 102,//报名
	enum E_BALANCER_MAINID
	{
		E_balancer_mainid_req_loginserver_addr = 1
	};


	//balancer 返回loginserver 地址
	struct struct_req_loginserver_addr
	{
		char ip[16];
		int  port;

		struct_req_loginserver_addr()
		{
			std::memset(this, 0, sizeof(struct_req_loginserver_addr));
		}
	};
}

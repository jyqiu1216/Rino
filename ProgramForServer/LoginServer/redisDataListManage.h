#ifndef WJ_RedisDataListManage_H
#define WJ_RedisDataListManage_H

#include "Redis/WJRedis.h"

namespace WJ
{
	//redis 事务函数，注意：这些静态函数运行在dbworker 线程中
	//redis命令 用户登录大厅
	void onRedis_Order_h_messid_load_hall(CRedisWorkerThread *pDbWorker, void *pArg);
	//redis命令 玩家报名
	void onRedis_Order_h_messid_apply(CRedisWorkerThread *pDbWorker, void *pArg);
	//redis命令 玩家退费
	void onRedis_Order_h_messid_apply_restitution(CRedisWorkerThread *pDbWorker, void *pArg);
	//redis命令 玩家创建团队
	void onRedis_Order_h_messid_team_create(CRedisWorkerThread *pDbWorker, void *pArg);
	//redis命令 玩家加入团队
	void onRedis_Order_h_messid_team_join(CRedisWorkerThread *pDbWorker, void *pArg);
	//redis命令 玩家退出团队
	void onRedis_Order_h_messid_team_leave(CRedisWorkerThread *pDbWorker, void *pArg);
	//redis命令 玩家解散团队
	void onRedis_Order_h_messid_team_dissolve(CRedisWorkerThread *pDbWorker, void *pArg);
	//redis命令 更新用户数据(娱币、积分)
	void onRedis_Order_h_messid_update_userinfo(CRedisWorkerThread *pDbWorker, void *pArg);
}

#endif//WJ_RedisDataListManage_H

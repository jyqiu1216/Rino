#ifndef WJ_ApplyManage_H
#define WJ_ApplyManage_H

#include <map>
#include <mutex>
#include "common/WJBaseType.h"
namespace WJ
{
	//报名节点数据
	struct struct_applynode
	{
		UINT userid;//用户id
		UINT ucontestid;//比赛id
		bool bAllot;//是否被分配到房间去了
		UINT uroomid;//被分配的房间id
		struct_applynode()
		{
			std::memset(this, 0, sizeof(struct_applynode));
		}
	};

	class CApplayManage
	{
	public:
		CApplayManage();
		~CApplayManage();

		//清除所有记录
		void clear();
		//添加
		bool add(struct_applynode &data);
		//删除
		bool del(UINT userid);
		//是否存在
		bool bExist(UINT userid);
		//设置玩家被分配到房间
		void AllotRoom(UINT userid, UINT uroomid);
		//设置玩家未被分配到房间
		void unAllotRoom(UINT userid);
		//获得该比赛有多少人报名还没有处理
		UINT getusercount(UINT ucontestid);
		//获得个人赛有多少人报名还没有处理
		UINT getusercount_unplaying();
		//获得个人赛有多少人已经在比赛了
		UINT getusercount_playing();
		//获得玩家报名数据
		bool at(UINT userid, struct_applynode& data);
	private:
		std::map<long long, struct_applynode> m_datalist;
	};
}

#endif//WJ_ApplyManage_H


#include "ApplyManage.h"
#include "common/commonfunction.h"
namespace WJ
{
	CApplayManage::CApplayManage()
	{
		clear();
	}

	CApplayManage::~CApplayManage()
	{
		clear();
	}

	//清除所有记录
	void CApplayManage::clear()
	{
		m_datalist.clear();
	}

	//添加
	bool CApplayManage::add(struct_applynode &data)
	{
		if (bExist(data.userid))
		{
			return false;
		}
		m_datalist.insert(std::map<long long, struct_applynode>::value_type(data.userid,data));
		return true;
	}

	//删除
	bool CApplayManage::del(UINT userid)
	{
		auto iter = m_datalist.find(userid);
		if(iter != m_datalist.end())
		{
			m_datalist.erase(iter);
			return true;
		}

		return false;
	}

	//是否存在
	bool CApplayManage::bExist(UINT userid)
	{
		auto iter = m_datalist.find(userid);
		if(iter != m_datalist.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//设置玩家被分配到房间
	void CApplayManage::AllotRoom(UINT userid, UINT uroomid)
	{
		auto iter = m_datalist.find(userid);
		if(iter != m_datalist.end())
		{
			(*iter).second.bAllot = true;
			(*iter).second.uroomid = uroomid;
		}
	}

	//设置玩家未被分配到房间
	void CApplayManage::unAllotRoom(UINT userid)
	{
		auto iter = m_datalist.find(userid);
		if (iter != m_datalist.end())
		{
			
			(*iter).second.bAllot = false;
			(*iter).second.uroomid = 0;
		}
	}

	//获得该比赛有多少人报名还没有处理
	UINT CApplayManage::getusercount(UINT ucontestid)
	{
		UINT ucount = 0;
		auto iter = m_datalist.begin();
		while (iter != m_datalist.end())
		{
			if (ucontestid == (*iter).second.ucontestid
				&& !(*iter).second.bAllot)
			{
				++ucount;
			}
			++iter;
		}

		return ucount;
	}

	//获得个人赛有多少人报名还没有处理
	UINT CApplayManage::getusercount_unplaying()
	{
		UINT ucount = 0;
		auto iter = m_datalist.begin();
		while (iter != m_datalist.end())
		{
			if (!(*iter).second.bAllot)
			{
				++ucount;
			}
			++iter;
		}

		return ucount;
	}

	//获得个人赛有多少人已经在比赛了
	UINT CApplayManage::getusercount_playing()
	{
		UINT ucount = 0;
		auto iter = m_datalist.begin();
		while (iter != m_datalist.end())
		{
			if ((*iter).second.bAllot)
			{
				++ucount;
			}
			++iter;
		}

		return ucount;
	}

	//获得玩家报名数据
	bool CApplayManage::at(UINT userid, struct_applynode& data)
	{
		auto iter = m_datalist.find(userid);
		if(iter != m_datalist.end())
		{
			//QK_InfoLog("sdp_test", "hall CApplayManage::at(userid=%d,ucontestid=%d,bAllot=%d)", (*iter).userid, (*iter).ucontestid, (*iter).bAllot, (*iter).uroomid);
			data = (*iter).second;
			return true;
		}
		else
		{
			return false;
		}
	}

}
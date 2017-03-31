#ifndef WJ_GameDesk_H
#define WJ_GameDesk_H
#include <mutex>
#include "common/WJBaseType.h"
#include "UserInfo.h"
#include "message/WJNetMessageHead.h"
#include "server/WJTimerMgr.h"


namespace WJ
{
	//游戏计时器id 所有桌子系统计时器一律按照 TIMER_GAME_TIMERID_MASK_SYS 格式设计
#define GAME_TIME_GAMEFINISH_WAIT		0x00000010 //当收到一个玩家发送结束消息的时候启动，防止有些玩家断线不发
#define GAME_TIME_AGREE_OUTTIME			0x00000020 //游戏搓桌成功则启动该计时器直到游戏开始的时候这个计时器就无用了，防止桌子收不到客户端通知的同意消息而导致桌子卡住
#define GAME_TIME_GAMEFINISH_OUTTIME	0x00000030 //游戏一旦开始则启动该计时器，防止桌子收不到客户端通知的结束从而无法结束游戏
	class CGameRoom;
	struct Room_ManageInfoStruct;
	class ISysDeskInterface
	{
	public://这个接口是给服务端用的，游戏端不能继承，所以要用到关键字 final
		//清理桌子,强行踢人(是否把玩家重新放回排队机中)
		virtual void ClearDesk(bool bInqueue = false) = 0;
		///判断游戏桌上的某个玩家是否开始游戏了
		virtual bool IsPlayGame() = 0;
		///用户同意开始
		virtual bool UserAgreeGame(UINT uDeskStation) = 0;
		///处理用户发送结束消息
		virtual bool Sys_UserEndGame(UINT uDeskStation, void* pData, UINT DataSize) = 0;
		/// 获得桌子中第一个没有人的座位索引
		virtual bool GetDeskNoPlayerStation(UINT &uDeskStation) = 0;
		/// 获得桌子中有多少人
		virtual UINT GetDeskPlayerNum() = 0;
		///用户在线状态离开游戏
		virtual bool UserOnlineStationNetCut(UINT uDeskStation, CGameUserInfo * pLostUserInfo) = 0;
		///用户在线状态重回游戏
		virtual bool UserOnlineStationReCome(UINT uDeskStation, CGameUserInfo * pNewUserInfo) = 0;
		///是否锁定
		virtual bool IsLock() = 0;
		///上锁
		virtual void LockDesk() = 0;
		///解锁
		virtual void UnlockDesk() = 0;
		//是否存在该玩家
		virtual bool IsExistPlayer(CGameUserInfo * pUserInfo) = 0;
		///初始化函数
		virtual bool Init(UINT uDeskStation, UINT bMaxPeople, void* pDataManage) = 0;
		//判断这个座位是否有人坐下了
		virtual bool bExistPlayerForDeskStation(UINT uDeskStation) = 0;
		/// 设置桌子中的玩家信息
		virtual void SetDeskUserInfo(CGameUserInfo * pUserInfo, UINT uDeskStation) = 0;
		//获取用户信息指针
		virtual /*const*/ CGameUserInfo* GetUserInfo(UINT uDeskStation) = 0;
		//获取最大人数
		virtual const UINT GetMaxPeople() = 0;
		//获取桌子id
		virtual const UINT GetDeskIndex() = 0;
		//发送桌子数据
		virtual bool SendDeskData(void * pData, UINT uSize, UINT uMainID, UINT uAssID, UINT uHandleCode) = 0;
		virtual bool SendDeskData(UINT uMainID, UINT uAssID, UINT uHandleCode) = 0;
		//获取同桌玩家 
		virtual UINT FillDeskUserInfo(char * pBuffer, UINT uBufferSize, UINT& uindex, bool & bFinish) = 0;
		///游戏结束
		/// @param bDeskStation 异常结束时，引起结束的玩家在桌子中的位置号，正常结束时无意义
		/// @param bCloseFlag  异常结束游戏时的标志，正常结束时为0
		virtual bool GameFinish(UINT uCloseFlag, UINT uDeskStation = 0) = 0;
		///用户断线离开
		virtual bool Sys_UserNetCut(UINT uDeskStation, CGameUserInfo * pLostUserInfo) = 0;
		///用户断线重来
		virtual bool Sys_UserReCome(UINT uDeskStation, CGameUserInfo * pNewUserInfo) = 0;
		///游戏开始
		virtual bool Sys_GameBegin(UINT uBeginFlag) = 0;
		///用户离开游戏桌
		virtual UINT Sys_UserLeftDesk(UINT uDeskStation, CGameUserInfo * pUserInfo, UINT uLeftCause = 0) = 0;
		///框架消息处理函数
		virtual bool Sys_HandleFrameMessage(UINT uDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID) = 0;
		///游戏数据包处理函数
		virtual bool Sys_HandleGameMessage(UINT uDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID) = 0;
		///定时器消息
		virtual bool Sys_OnTimer(UINT uTimerID) = 0;
		///获取游戏状态信息
		virtual bool Sys_OnGetGameStation(UINT uDeskStation, UINT uSocketID) = 0;
		//设置房主
		virtual void SetdwOwnerUserID(UINT uuserid) = 0;
		//获取房主
		virtual UINT GetdwOwnerUserID() = 0;
		//搓桌分配完成
		virtual void QueueOver() = 0;
		//玩家是否任何时候都可以离开（意味着一离开游戏就结束了，即游戏状态只有一个）
		virtual bool SysCanLeaveEveryTime() = 0;
	};

	class /*WJSERVERGAMELIB_API*/ CGameDesk : public ISysDeskInterface
	{
	private://固有属性一旦确定不会更改
		UINT							m_uDeskIndex;							///< 桌子号码
		UINT							m_uMaxPeople;							///< 游戏人数
		CGameRoom						* m_pGameRoom;							///< 数据管理指针
	private:
		long int						m_dwBeginTime;							///< 开始时间
		bool							m_bLock;								///< 是否上锁
		char							m_szLockPass[61];						///< 上锁密码
		UINT							m_dwOwnerUserID;						///< 房主 ID
		bool							m_bPlayGame;							///< 游戏是否开始标志
		CGameUserInfo					* m_pUserInfo[MAX_PEOPLE];				///< 用户信息指针，此数组索引一定是玩家的座位索引。
		bool							m_UserEndGame[MAX_PEOPLE];				///< 客户端通知服务器，游戏可以结束了

	public://允许游戏修改的数据
		UINT							m_uGameStation;							///< 游戏状态

	public:
		CGameDesk();
		~CGameDesk();

	public:///游戏必须实现的函数/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///是否可以开始游戏
		virtual bool CanBeginGame() = 0;
		//游戏内部初始化
		virtual bool InitDeskGameStation() = 0;
		///获取游戏状态信息
		virtual bool OnGetGameStation(UINT uDeskStation, UINT uSocketID) = 0;
		///重置游戏状态
		virtual bool ReSetGameState(UINT uLastStation) = 0;
		///用户断线离开
		virtual bool UserNetCut(UINT uDeskStation, CGameUserInfo * pLostUserInfo) = 0;
		///用户断线重来
		virtual bool UserReCome(UINT uDeskStation, CGameUserInfo * pNewUserInfo) = 0;
		///游戏开始
		virtual bool GameBegin(UINT uBeginFlag) = 0;
		///用户离开游戏桌
		virtual UINT UserLeftDesk(UINT uDeskStation, CGameUserInfo * pUserInfo, UINT uLeftCause = 0) = 0;
		///框架消息处理函数
		virtual bool HandleFrameMessage(UINT uDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID) = 0;
		///游戏数据包处理函数
		virtual bool HandleGameMessage(UINT uDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID) = 0;
		///定时器消息
		virtual bool OnTimer(UINT uTimerID) = 0;
		//游戏结束的时候处理游戏内部数据，结算
		//void* pData:返回结束的消息包内容
		//UINT &DataSize:返回结束的消息包大小
		virtual bool GameOverSettlement(UINT uCloseFlag, std::string &pData) = 0;
		///处理用户发送结束消息
		virtual bool UserEndGame(UINT uDeskStation, void* pData, UINT DataSize) = 0;
		//玩家是否任何时候都可以离开（意味着一离开游戏就结束了，即游戏状态只有一个）
		virtual bool CanLeaveEveryTime() = 0;
	public://游戏和服务端都可使用的接口///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///发送游戏状态
		bool SendGameStation(UINT uDeskStation, UINT uSocketID, void* pStationData, UINT uSize);
		///发送开始消息
		bool SendBeginData(void * pData, UINT uSize);
		///发送结束消息
		bool SendFinishData(void * pData, UINT uSize);
		///发送数据函数
		bool SendGameData(UINT uDeskStation, UINT uMainID, UINT uAssID, UINT uHandleCode);
		///发送数据函数
		bool SendGameData(UINT uDeskStation, void * pData, UINT uSize, UINT uMainID, UINT uAssID, UINT uHandleCode);
		//批量发送函数
		void SendDataBatch(void * pData, UINT uSize, UINT uMainID, UINT uAssistantID, UINT uHandleCode);
		///设置定时器
		bool SetTimer(UINT uTimerID, int uElapse);
		///删除定时器
		bool KillTimer(UINT uTimerID);
		//获取房间指针
		const Room_ManageInfoStruct* GetRoom_ManageInfoStruct();
		//获得随机数字
		void GetRandText(std::string &str, int strleng);

		//获取用户信息指针
		virtual CGameUserInfo* GetUserInfo(UINT uDeskStation) override final;
		//获取最大人数
		virtual const UINT GetMaxPeople() override final;
		//获取桌子id
		virtual const UINT GetDeskIndex() override final;
		///判断游戏桌是否开始游戏了
		virtual bool IsPlayGame() override final;
		/// 获得桌子中有多少人
		virtual UINT GetDeskPlayerNum() override final;
		//发送桌子数据
		virtual bool SendDeskData(void * pData, UINT uSize, UINT uMainID, UINT uAssID, UINT uHandleCode) override final;
		virtual bool SendDeskData(UINT uMainID, UINT uAssID, UINT uHandleCode) override final;
		///游戏结束
		/// @param bCloseFlag  异常结束游戏时的标志，正常结束时为0
		virtual bool GameFinish(UINT uCloseFlag, UINT uDeskStation = 0) override final;
	private://仅服务端使用接口///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//清理桌子,强行踢人(是否把玩家重新放回排队机中)
		virtual void ClearDesk(bool bInqueue = false) override final;
		///用户同意开始
		virtual bool UserAgreeGame(UINT uDeskStation) override final;
		///处理用户发送结束消息
		virtual bool Sys_UserEndGame(UINT uDeskStation, void* pData, UINT DataSize) override final;
		/// 获得桌子中第一个没有人的座位索引
		virtual bool GetDeskNoPlayerStation(UINT &uDeskStation) override final;
		///用户在线状态离开游戏
		virtual bool UserOnlineStationNetCut(UINT uDeskStation, CGameUserInfo * pLostUserInfo) override final;
		///用户在线状态重回游戏
		virtual bool UserOnlineStationReCome(UINT uDeskStation, CGameUserInfo * pNewUserInfo) override final;
		///是否锁定
		virtual bool IsLock() override final;
		///上锁
		virtual void LockDesk() override final;
		///解锁
		virtual void UnlockDesk() override final;
		//是否存在该玩家
		virtual bool IsExistPlayer(CGameUserInfo * pUserInfo) override final;
		///初始化函数
		virtual bool Init(UINT uDeskStation, UINT bMaxPeople, void* pDataManage) override final;
		//判断这个座位是否有人坐下了
		virtual bool bExistPlayerForDeskStation(UINT uDeskStation) override final;
		/// 设置桌子中的玩家信息
		virtual void SetDeskUserInfo(CGameUserInfo * pUserInfo, UINT uDeskStation) override final;
		//获取同桌玩家 
		virtual UINT FillDeskUserInfo(char * pBuffer, UINT uBufferSize, UINT& uindex, bool & bFinish) override final;
		///用户断线离开
		virtual bool Sys_UserNetCut(UINT uDeskStation, CGameUserInfo * pLostUserInfo) override final;
		///用户断线重来
		virtual bool Sys_UserReCome(UINT uDeskStation, CGameUserInfo * pNewUserInfo) override final;
		///游戏开始
		virtual bool Sys_GameBegin(UINT bBeginFlag) override final;
		///用户离开游戏桌
		virtual UINT Sys_UserLeftDesk(UINT uDeskStation, CGameUserInfo * pUserInfo, UINT uLeftCause = 0) override final;
		///框架消息处理函数
		virtual bool Sys_HandleFrameMessage(UINT uDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID) override final;
		///游戏数据包处理函数
		virtual bool Sys_HandleGameMessage(UINT uDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID) override final;
		///定时器消息
		virtual bool Sys_OnTimer(UINT uTimerID) override final;
		///获取游戏状态信息
		virtual bool Sys_OnGetGameStation(UINT uDeskStation, UINT uSocketID) override final;
		//设置房主
		virtual void SetdwOwnerUserID(UINT uuserid) override final;
		//获取房主
		virtual UINT GetdwOwnerUserID() override final;
		//搓桌分配完成
		virtual void QueueOver() override final;
		//玩家是否任何时候都可以离开（意味着一离开游戏就结束了，即游戏状态只有一个）
		virtual bool SysCanLeaveEveryTime() override final;
	private://仅类自己使用的方法/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///是否可以开始游戏
		bool Sys_CanBeginGame();
		///清理所有断线玩家
		bool CleanCutGameInfoAll();
		///清理指定断线玩家
		bool CleanCutGameInfo(UINT bDeskStation);
		//生成随机数
		int MyRand();

		//获取一个桌子内所有玩家列表,除自己信息外
		UINT FillDeskUsers(char * pBuffer, UINT uBufferSize, UINT deskid, UINT myuserid, UINT uBeginPos, bool & bFinish, UINT &uCopyCount);

		//向指定玩家更新该玩家所在桌子内所有除自己数据外的用户列表
		void SendUserListForDesk(UINT deskid, UINT socketindex, UINT myuserid);
	};
}

#endif//WJ_GameDesk_H
/*=================
创建时间2016-4-23
最后改动时间2016-4-25 15:29
=====================*/

#ifndef _FXQ_GAMELOGICMANAGER_H_
#define _FXQ_GAMELOGICMANAGER_H_

#include "GameLogicManager.h"

#define WGS_GL_ORDER_MUTEX_NAME gl_order_inner_mutex
#define WGS_GL_ORDER_INNER_MUTEX mutable boost::mutex WGS_GL_ORDER_MUTEX_NAME;
#define WGS_GL_ORDER_INNER_LOCK		boost::lock_guard<boost::mutex> wgsChnInnerMutexLock(WGS_GL_ORDER_MUTEX_NAME);

#define WGS_WC_ORDER_MUTEX_NAME wc_order_inner_mutex
#define WGS_WC_ORDER_INNER_MUTEX mutable boost::mutex WGS_WC_ORDER_MUTEX_NAME;
#define WGS_WC_ORDER_INNER_LOCK		boost::lock_guard<boost::mutex> wgsWCMutexLock(WGS_WC_ORDER_MUTEX_NAME);

#define WGS_WC1_ORDER_MUTEX_NAME wc1_order_inner_mutex
#define WGS_WC1_ORDER_INNER_MUTEX mutable boost::mutex WGS_WC1_ORDER_MUTEX_NAME;
#define WGS_WC1_ORDER_INNER_LOCK		boost::lock_guard<boost::mutex> wgsWC1MutexLock(WGS_WC1_ORDER_MUTEX_NAME);

#define WGS_WC2_ORDER_MUTEX_NAME wc2_order_inner_mutex
#define WGS_WC2_ORDER_INNER_MUTEX mutable boost::mutex WGS_WC2_ORDER_MUTEX_NAME;
#define WGS_WC2_ORDER_INNER_LOCK		boost::lock_guard<boost::mutex> wgsWC2MutexLock(WGS_WC2_ORDER_MUTEX_NAME);

#define WGS_WC3_ORDER_MUTEX_NAME wc3_order_inner_mutex
#define WGS_WC3_ORDER_INNER_MUTEX mutable boost::mutex WGS_WC3_ORDER_MUTEX_NAME;
#define WGS_WC3_ORDER_INNER_LOCK		boost::lock_guard<boost::mutex> wgsWC3MutexLock(WGS_WC3_ORDER_MUTEX_NAME);

class FXQGameLogicManager :public GameLogicManager, public Singleton<FXQGameLogicManager>
{
	public:
	WGS_GL_ORDER_INNER_MUTEX
	WGS_WC_ORDER_INNER_MUTEX
	WGS_WC1_ORDER_INNER_MUTEX
	WGS_WC2_ORDER_INNER_MUTEX
	WGS_WC3_ORDER_INNER_MUTEX
	
	FXQGameLogicManager();
	~FXQGameLogicManager();
	//	创建角色
	CharPtr CreateCreature(UserPtr & user,bool isCenter = true);
	
	// 创建战场
	ChannelPtr CreateNewChannel(ResourceProxy town);
	
	//更新游戏排行
	void UpdateGameOrderList(void);
	
	// 得到游戏排行列表
	std::list<ResourcePtr> GetGameOrderList(const uint32 & game_id,const uint8 & type);
	
	// //重写添加物品数量函数
	// ItemPtr addItemNum(CharPtr & chr, const uint32 & model_id, const uint32 & addnum, bool bUpdate = true);
	
	//增加游戏次数
	void addBattleTimes(CharPtr & pChr,int winChips);
	
	//按类型增加数值
	void addNumberByType(CharPtr &pChr,const uint8 & type,const uint32 & num,bool ab = true);
	
	//检测等级变化
	uint32 checkLevelChange(CharPtr &pChr);
	//检测勋章变化
	uint32 checkXZChange(CharPtr &pChr, bool up_d = true);
	
	//std::vector<CharPtr> robotList;
	
	//获取平均等待时间
	uint32 get_acgWaitTime();
	
	//玩家换服务器时做投降处理
	virtual void DoSomethingBeforeExitGameServer(CharPtr & pChr);
	
	//更新平均等待时间
	void upDataAcgWaitTime(uint32 ti);
	
	//std::vector<CharPtr> waitPlayerList;					//等待队列
	
	protected:
	std::list<ResourcePtr>  m_downOrder;					//击坠排行
	std::list<ResourcePtr>  m_treasureOrder;				//财富排行
	std::list<ResourcePtr>  m_winRiteOrder;					//胜率排行
	std::list<ResourcePtr>  m_medalOrder;					//勋章排行
	
	uint32 acgWaitTime;										//等待平均时间
	uint32 startTimes;										//局数
};


#endif



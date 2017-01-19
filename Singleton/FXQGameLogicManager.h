/*=================
����ʱ��2016-4-23
���Ķ�ʱ��2016-4-25 15:29
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
	//	������ɫ
	CharPtr CreateCreature(UserPtr & user,bool isCenter = true);
	
	// ����ս��
	ChannelPtr CreateNewChannel(ResourceProxy town);
	
	//������Ϸ����
	void UpdateGameOrderList(void);
	
	// �õ���Ϸ�����б�
	std::list<ResourcePtr> GetGameOrderList(const uint32 & game_id,const uint8 & type);
	
	// //��д�����Ʒ��������
	// ItemPtr addItemNum(CharPtr & chr, const uint32 & model_id, const uint32 & addnum, bool bUpdate = true);
	
	//������Ϸ����
	void addBattleTimes(CharPtr & pChr,int winChips);
	
	//������������ֵ
	void addNumberByType(CharPtr &pChr,const uint8 & type,const uint32 & num,bool ab = true);
	
	//���ȼ��仯
	uint32 checkLevelChange(CharPtr &pChr);
	//���ѫ�±仯
	uint32 checkXZChange(CharPtr &pChr, bool up_d = true);
	
	//std::vector<CharPtr> robotList;
	
	//��ȡƽ���ȴ�ʱ��
	uint32 get_acgWaitTime();
	
	//��һ�������ʱ��Ͷ������
	virtual void DoSomethingBeforeExitGameServer(CharPtr & pChr);
	
	//����ƽ���ȴ�ʱ��
	void upDataAcgWaitTime(uint32 ti);
	
	//std::vector<CharPtr> waitPlayerList;					//�ȴ�����
	
	protected:
	std::list<ResourcePtr>  m_downOrder;					//��׹����
	std::list<ResourcePtr>  m_treasureOrder;				//�Ƹ�����
	std::list<ResourcePtr>  m_winRiteOrder;					//ʤ������
	std::list<ResourcePtr>  m_medalOrder;					//ѫ������
	
	uint32 acgWaitTime;										//�ȴ�ƽ��ʱ��
	uint32 startTimes;										//����
};


#endif



#pragma once

#include "constDef.h"
#include <QList>
#include <QMap>
#include <QSharedPointer>
#include <QMutex>
#include <QTime>


class ITObject;
class ITGameBaseData;
class ITGameAttributeData;
class ITGameItem;
class ITGamePet;
class ITGameMap;
class ITGameGateMap;
class ITAccountIdentity;
class ITAccount;
class ITAccountGid;
class ITGameCharacter;
class ITCGPetPictorialBook;
class ITGameSkill;
class ITCharcterServer;
class ITRouteNode;
class ITGameServerType;
class ITHost;


typedef QSharedPointer<ITObject> ITObjectPtr;
typedef QList<ITObjectPtr> ITObjectList;

typedef QSharedPointer<ITGameBaseData> ITGameBaseDataPtr;
typedef QList<ITGameBaseDataPtr> ITGameBaseDataList; 

typedef QSharedPointer<ITGameCharacter> ITGameCharacterPtr;
typedef QList<ITGameCharacterPtr> ITGameCharacterList;

typedef QSharedPointer<ITGameAttributeData> ITGameAttributeDataPtr;
typedef QList<ITGameAttributeDataPtr> ITGameAttributeDataList;

typedef QSharedPointer<ITGameItem> ITGameItemPtr;
typedef QList<ITGameItemPtr> ITGameItemList;

typedef QSharedPointer<ITGamePet> ITGamePetPtr;
typedef QList<ITGamePetPtr> ITGamePetList;

typedef QSharedPointer<ITCGPetPictorialBook> ITCGPetPictorialBookPtr;
typedef QList<ITCGPetPictorialBookPtr> ITCGPetPictorialBookList;

typedef QSharedPointer<ITGameMap> ITGameMapPtr;
typedef QList<ITGameMapPtr> ITGameMapList;

typedef QSharedPointer<ITGameGateMap> ITGameGateMapPtr;
typedef QList<ITGameGateMapPtr> ITGameGateMapList;

typedef QSharedPointer<ITAccountIdentity> ITAccountIdentityPtr;
typedef QList<ITAccountIdentityPtr> ITAccountIdentityList;

typedef QSharedPointer<ITAccount> ITAccountPtr;
typedef QList<ITAccountPtr> ITAccountList;

typedef QSharedPointer<ITAccountGid> ITAccountGidPtr;
typedef QList<ITAccountGidPtr> ITAccountGidList; 

typedef QSharedPointer<ITHost> ITHostPtr;
typedef QList<ITHostPtr> ITHostList;

typedef QSharedPointer<ITGameSkill> ITGameSkillPtr;
typedef QList<ITGameSkillPtr> ITGameSkillList;

typedef QSharedPointer<ITCharcterServer> ITCharcterServerPtr;
typedef QList<ITCharcterServerPtr> ITCharcterServerList;

typedef QSharedPointer<ITGameServerType> ITGameServerTypePtr;
typedef QList<ITGameServerTypePtr> ITGameServerTypeList;

typedef QSharedPointer<ITRouteNode> ITRouteNodePtr;
typedef QList<ITRouteNodePtr> ITRouteNodeList;

Q_DECLARE_METATYPE(ITObjectPtr)
Q_DECLARE_METATYPE(ITGameAttributeDataPtr)
Q_DECLARE_METATYPE(ITGameBaseDataPtr)
Q_DECLARE_METATYPE(ITGameItemPtr)
Q_DECLARE_METATYPE(ITGamePetPtr)
Q_DECLARE_METATYPE(ITGameMapPtr)
Q_DECLARE_METATYPE(ITGameGateMapPtr)
Q_DECLARE_METATYPE(ITAccountPtr)
Q_DECLARE_METATYPE(ITAccountGidPtr)
Q_DECLARE_METATYPE(ITGameCharacterPtr)
Q_DECLARE_METATYPE(ITCGPetPictorialBook)
Q_DECLARE_METATYPE(ITGameSkillPtr)
Q_DECLARE_METATYPE(ITCharcterServerPtr)
Q_DECLARE_METATYPE(ITGameServerTypePtr)
Q_DECLARE_METATYPE(ITHostPtr)
enum TObjStatus //设备状态
{
	TStatus_Normal = 0, //正常
	TStatus_Add = 1,	//增加
	TStatus_Edit = 2,	//修改
	TStatus_Delete = 3, //删除
};

/*!
* @brief 工程模块工厂
*/
class ObjectModuleFactory
{
public:
	virtual ~ObjectModuleFactory(void) {}
	virtual ITObjectPtr Create() = 0;
};

#define DECLARE_OBJECT_MODULE_FACTORY(ModuleName)          \
	class ModuleName##Factory : public ObjectModuleFactory \
	{                                                      \
	public:                                                \
		virtual ITObjectPtr Create()                       \
		{                                                  \
			return ITObjectPtr(new ModuleName());          \
		}                                                  \
	};
#ifndef NEW_MODULE_FACTORY
#define NEW_MODULE_FACTORY(ModuleName) \
	(new ModuleName##Factory())
#endif // !NEW_MODULE_FACTORY

//! 对象基类
class ITObject : public QEnableSharedFromThis<ITObject>
{
public:
	ITObject();
	ITObject(QString szName, int nObjType, quint64 ullID);
	virtual ~ITObject();
	//设置状态值
	int getStatus(void) { return m_nStatus; };
	void setAddStatus(void) { m_nStatus = TStatus_Add; }
	virtual void setDelStatus(void) { m_nStatus = TStatus_Delete; }
	void setEditStatus(void)
	{
		if (m_nStatus == TStatus_Normal) m_nStatus = TStatus_Edit;
	}
	void setNomalStatus(void) { m_nStatus = TStatus_Normal; }

	//设置属性值
	void setObjectType(int nType);
	void setObjectID(quint64 nCode);
	void setObjectName(const QString& sName);
	void setObjectCode(int code);
	void setObjectOrder(int order) { m_nOrder = order; }
	void setObjectDsec(QString strDesc) { m_sObjDesc = strDesc; }

	//获取属性值
	const int& getObjectCode()const;
	const int& getObjectType(void)const;
	const quint64 getObjectID(void)const;
	const QString getObjectName(void)const;
	const QString getObjectDesc(void)const { return m_sObjDesc; }
	int getObjectOrder( ) { return m_nOrder; }

	quint64 id64() { return m_ullID; }
	void setObjectParent(ITObjectPtr parent) { m_parent = parent; }
	ITObjectPtr getObjectParent() { return m_parent; }

	ITObjectList GetAllChildObj() { return m_childList; }
	void addChildObj(ITObjectPtr childObj);
	void removeChildObj(ITObjectPtr childDev);
	void clearAllChildObj();
	bool _bUpdate = false; //是否更新

private:
	QMutex* m_nameMutex;			//不能定义，子类继承或赋值函数赋值时候，因为此锁不能赋值，所以报错
	int m_nType = TObject_None;		//Obj类型		状态值为enum类型ObjectType
	int m_nStatus = TStatus_Normal; //Obj修改状态
	quint64 m_ullID = 0;			//id
	int m_gameCode = 0;				//游戏代码
	QString m_szName;				//名称
	ITObjectPtr m_parent;			//父设备
	ITObjectList m_childList;		//子列表
	QString m_sObjDesc;				//描述
	int m_nOrder = 0;				//!< 排序用
};
DECLARE_OBJECT_MODULE_FACTORY(ITObject)
//! 游戏基础数据-人物、宠物共有数据
class ITGameBaseData : public ITObject
{
public:
	ITGameBaseData();
	ITGameBaseData(QString szName, int nType, quint64 ullID);
	virtual ~ITGameBaseData();
	ITGameBaseData& operator=(const ITGameBaseData &o);
	bool operator!=(const ITGameBaseData& o)const;
	bool operator==(const ITGameBaseData& o)const;
	quint64 char_id=0;
	int _level = 0;					//!< 等级
	int	_xp = 0;					//!< 经验
	int _maxxp = 0;					//!< 最大经验
	int _hp = 0;					//!< 血
	int _maxhp = 0;					//!< 最大血
	int _mp = 0;					//!< 魔
	int _maxmp = 0;					//!< 最大魔
	int _health = 0;				//!< 健康
	int _skillslots = 0;			//!< 技能格
	int _imageid = 0;				//!< 图片id

};
DECLARE_OBJECT_MODULE_FACTORY(ITGameBaseData)
//! 游戏基础数据-人物、宠物共有数据
class ITGameAttributeData : public ITObject
{
public:
	ITGameAttributeData();
	ITGameAttributeData(QString szName, int nType, quint64 ullID);
	virtual ~ITGameAttributeData();
	ITGameAttributeData &operator=(const ITGameAttributeData &o);
	bool operator!=(const ITGameAttributeData& o)const;
	bool operator==(const ITGameAttributeData& o)const;
	quint64 _char_id=0;
	int _points_remain = 0;			//!< 未加点
	int _points_endurance = 0;		//!< 体力
	int _points_strength = 0;		//!< 力量
	int _points_defense = 0;		//!< 防御
	int _points_agility = 0;		//!< 敏捷
	int _points_magical = 0;		//!< 魔法
	int _value_attack = 0;			//!< 攻击力
	int _value_defensive = 0;		//!< 防御力
	int _value_agility = 0;			//!< 敏捷度
	int _value_spirit = 0;			//!< 精神
	int _value_recovery = 0;		//!< 恢复
	int _resist_poison = 0;			//!< 抵抗-毒
	int _resist_sleep = 0;			//!< 抵抗-睡
	int _resist_medusa = 0;			//!< 抵抗-石
	int _resist_drunk = 0;			//!< 抵抗-醉
	int _resist_chaos = 0;			//!< 抵抗-混
	int _resist_forget = 0;			//!< 抵抗-遗忘
	int _fix_critical = 0;			//!< 必杀
	int _fix_strikeback = 0;		//!< 反击
	int _fix_accurancy = 0;			//!< 命中
	int _fix_dodge = 0;				//!< 闪躲
	int _element_earth = 0;			//!< 属性-地
	int _element_water = 0;			//!< 属性-水
	int _element_fire = 0;			//!< 属性-火
	int _element_wind = 0;			//!< 属性-风
	int _manu_endurance = 0;		//!< 耐力
	int _manu_skillful = 0;			//!< 灵巧
	int _manu_intelligence = 0;		//!< 智力
};
DECLARE_OBJECT_MODULE_FACTORY(ITGameAttributeData)
// 用管理类生成，否则自己管理指针对象
class ITGamePet : public ITObject
{
public:
	ITGamePet();
	ITGamePet(QString szName, int nType, quint64 ullID);
	virtual ~ITGamePet();

	quint64 _character_id = 0;//角色绑定关系
	QString _realName;		//宠物真实名称
	int _loyality = 0;		//忠诚
	int _petNumber = 0;		//宠物编号
	int _state = 0;			//当前状态 战斗 待命 休息
	int _race = 0;			//种族
	int _grade = 0;			//档次
	int _lossMinGrade = 0;	//最少掉档
	int _lossMaxGrade = 0;	//最多掉档
	int _pos = 0;			//!< 宠物位置
	bool _bExist = false;	//是否存在

	ITGameBaseDataPtr _baseData = nullptr;		//!< 宠物相关信息
	ITGameAttributeDataPtr _attrData = nullptr;	//!< 宠物属性
	ITGameSkillList _skillList;
	QMap<int, ITGameSkillPtr> _skillPosForSkill;
};
DECLARE_OBJECT_MODULE_FACTORY(ITGamePet)

class ITGameItem : public ITObject
{
public:
	ITGameItem();
	ITGameItem(QString szName, int nType, quint64 ullID);
	virtual ~ITGameItem();
	ITGameItem& operator=(const ITGameItem& o);
	bool operator!=(const ITGameItem& o)const;
	bool operator==(const ITGameItem& o)const;

	int _itemCount = 0;		//物品数量
	int _itemType = 0;	   //道具类型
	int _itemPrice = 0;	   //贩店价格
	int _itemPile = 0;	   //最大叠加数
	int _itemLevel = 0;	   //物品等级
	int _sellMinCount = 0; //卖店最少数量
	int _itemPos = 0;		//物品位置
	QString _itemAttr;	   //物品描述
	bool _bExist = false;	//是否存在
};
DECLARE_OBJECT_MODULE_FACTORY(ITGameItem)

//! 游戏地图信息
class ITGameMap : public ITObject
{
public:
	ITGameMap() {}
	ITGameMap(QString szName, int nType, quint64 ullID);
	virtual ~ITGameMap();

	int _mapNumber = 0; //地图编号
	int _oftenMap = 0;	//是否常用地图

	ITGameGateMapList m_pGateMapList;
};
DECLARE_OBJECT_MODULE_FACTORY(ITGameMap)

//! 传送点位信息
class ITGameGateMap : public ITObject
{
public:
	ITGameGateMap();
	ITGameGateMap(QString szName, int nType, quint64 ullID);
	virtual ~ITGameGateMap();

	int _x = 0;				  //传送点
	int _y = 0;				  //传送点
	int _tx = 0;			  //传送到达点
	int _ty = 0;			  //传送到达点
	QString _mapName;		  //当前地图名称
	int _mapNumber = 0;		  //当前地图编号
	QString _targetMapName;	  //目标地图名称
	int _targetMapNumber = 0; //目标地图编号
	int _warpType = 0;		  //0踩点传送 1点击传送 2对话选择传送
	QStringList _npcSelect;	  //对话选择传送内容 n个对话选项

	ITGameGateMapPtr _parent = nullptr;		//回溯用
	ITGameGateMapPtr _subGateMap = nullptr; //单条用
	ITGameGateMapList _navigationList;		//寻路用 临时数据
	int _nCost = 0;							//可达目标的距离成本
	int _nTotalCost = 0;					//如果为起始路线节点 存放总耗费
};

DECLARE_OBJECT_MODULE_FACTORY(ITGameGateMap)


struct ITRouteNode
{
	ITRouteNodePtr parent = nullptr;
	ITRouteNodePtr next = nullptr;
	ITGameGateMapPtr cur = nullptr;
	int nTotalCost = 0;
};
//! 游戏类型
class ITLoginGameType : public ITObject
{
public:
	ITLoginGameType();
	ITLoginGameType(QString szName, int nType, quint64 ullID);
	virtual ~ITLoginGameType();
};
//账号集合 按某个人设置
class ITAccountIdentity : public ITObject
{
public:
	ITAccountIdentity();
	ITAccountIdentity(QString szName, int nType, quint64 ullID);
	virtual ~ITAccountIdentity();

	QString _identity;	//身份证号
	int _sex;
};
DECLARE_OBJECT_MODULE_FACTORY(ITAccountIdentity)
//登录账号 电信-网通-时长-怀旧可能公用同一个
class ITAccount : public ITObject
{
public:
	ITAccount();
	ITAccount(QString szName, int nType, quint64 ullID);
	virtual ~ITAccount();

	QString _userName; //登录账号
	QString _passwd;   //登录密码
	QString _identity;	//身份证号
};
DECLARE_OBJECT_MODULE_FACTORY(ITAccount)

//游戏id
class ITAccountGid : public ITObject
{
public:
	ITAccountGid();
	ITAccountGid(QString szName, int nType, quint64 ullID);
	virtual ~ITAccountGid();

	QString _userGid; //gid账号
	quint64 _serverTypeID;//所在游戏大区id
	ITGameCharacterList _roleList;
};

DECLARE_OBJECT_MODULE_FACTORY(ITAccountGid)

class ITGameSysTime
{
public:
	int years;
	int month;
	int days;
	int hours;
	int mins;
	int secs;
	int local_time;
	int server_time;
};
//游戏角色
class ITGameCharacter : public ITObject
{
public:
	ITGameCharacter();
	ITGameCharacter(QString szName, int nType, quint64 ullID);
	virtual ~ITGameCharacter();
	ITGameCharacter &operator=(const ITGameCharacter &o);

	void setTargetGameServerType(ITGameServerTypePtr pServer) { _gameServerType = pServer; }
	ITGameServerTypePtr getTargetGameServerType() { return _gameServerType; }

	QString _gid;				//!< 游戏gid
	int _type = 0;				//!< 左右角色
	int	_sex = 0;				//!< 男女人物
	int	_gold = 0;				//!< 金币
	int	_bankgold = 0;			//!< 银行金币
	int _souls = 0;				//!< 灵魂
	int _score = 0;				//!< 战绩
	QString	_job;				//!< 职业
	QString _nickName;			//!< 自定称号
	int _useTitle = 0;			//!< 当前使用的称号index
	QStringList	_titles;		//!< 称号列表
	int _avatar_id = 0;			//!< 公共聊天id?
	int _unitid = 0;			//!< 聊天单位id？
	int _petid = 0;				//!< 当前宠物id
	bool _petriding = false;	//!< 是否骑乘中
	int _direction = 0;			//!< 人物朝向
	int _punchclock = 0;		//!< 卡时
	bool _usingpunchclock = false;//!< 是否已打卡

	int _value_charisma = 0;	//!< 魅力
	int _x = 0;					//!< 当前x坐标
	int _y = 0;					//!< 当前y坐标
	int _battle_position = 0;	//!< 战斗站位
	QString _map_name = "";		//!< 当前地图名称
	int _map_number = 0;		//!< 当前地图编号
	int _server_line = 0;		//!< 当前游戏线路
	int _big_line = 0;			//!< 当前服务器-电信13|网通14
	int _game_status = 0;		//!< 游戏状态
	int _world_status = 0;		//!< 游戏世界状态
	int _game_pid = 0;			//!< 游戏进程id
	int _game_port = 0;			//!< 游戏端口
	QString _prestige = "";		//!< 声望称号
	ITGameSysTime _gameSysTime;	//!< 当前游戏时间
	ITGameBaseDataPtr _baseData=nullptr;		//!< 角色相关信息
	ITGameAttributeDataPtr _attrData=nullptr;	//!< 角色属性
	QMap<int, ITGamePetPtr> _petPosForPet;		//!< 宠物位置和指针映射
	QMap<int, ITGameSkillPtr> _skillPosForSkill;//!< 技能位置和技能映射
	QMap<int, ITGameItemPtr> _itemPosForPtr;	//!< 物品id和物品指针映射
	QMap<QString, ITGameItemPtr> _bankItemNameForPtr;//银行物品名称和物品指针映射
	QMutex _mutex;				//!< 加锁
	QTime _lastUploadTime;		//!< 最后一次上传时间
	QDateTime _lastUpdateDateTime;	//!< 最后一次上传时间
	int _connectState = 0;		//!< 0离线 1在线
	ITGameServerTypePtr _gameServerType = nullptr;//!< 游戏大区
	quint64 _localHostId = 0;	//!< 所在电脑信息
};

DECLARE_OBJECT_MODULE_FACTORY(ITGameCharacter)

class ITGameSkill :public ITObject
{
public:
	ITGameSkill();
	ITGameSkill(QString szName, int nType, quint64 ullID);
	virtual ~ITGameSkill();
	ITGameSkill& operator=(const ITGameSkill& o);
	bool operator!=(const ITGameSkill& o)const;
	bool operator==(const ITGameSkill& o)const;

	QString _info;			//技能介绍
	int _id = 0;			//技能id
	int _cost = 0;			//耗魔
	int _flags = 0;			//
	int _index = 0;			//技能位置
	int _level = 0;			//等级
	int _maxLevel = 0;		//最高等级
	bool _available = 0;	//是否可用
	int _xp = 0;			//!< 技能经验
	int _maxxp = 0;			//!< 技能下一级经验
	bool _bExist = false;	//是否存在

	ITGameSkillList _subskills; //技能子项
};
DECLARE_OBJECT_MODULE_FACTORY(ITGameSkill)



//游戏子id配置
class ITAccountGidRoleRunConfig : public ITObject
{
public:
	ITAccountGidRoleRunConfig();
	ITAccountGidRoleRunConfig(QString szName, int nType, quint64 ullID);
	virtual ~ITAccountGidRoleRunConfig();

	int _serverLine;	 //几线 1-10
	QString _configPath; //配置路径名称
	QString _scriptPath; //脚本路径
};
DECLARE_OBJECT_MODULE_FACTORY(ITAccountGidRoleRunConfig)

//! 宠物图鉴信息和算档信息
class ITCGPetPictorialBook : public ITObject
{
public:
	ITCGPetPictorialBook();
	ITCGPetPictorialBook(QString szName, int nType, quint64 ullID);
	virtual ~ITCGPetPictorialBook();
	int _petNumber = 0;	   //宠物编号
	int _petRaceType = 0;  //宠物种族
	QString _petRace;	   //宠物种族
	int _petBaseBp = 0;	   //成长系数
	int _growVigor = 0;	   //体力成长
	int _growStrength = 0; //力量成长
	int _growDefense = 0;  //防御成长
	int _growAgility = 0;  //敏捷成长
	int _growMagic = 0;	   //魔法成长
	int _canCatch = 0;		//0=不可捕捉，1=可以捕捉
	int _cardType = 0;		// 0 = 无，1 = 银卡，2 = 金卡
	int _imageId = 0;		//图片id
	int _skillCount = 0;	//技能格
	int _elementEarth = 0;	//地
	int _elementWater = 0;	//水
	int _elementFire = 0;	//火
	int _elementWind = 0;	//风

	int rate_endurance; //0x000C 0=半颗星，3=2颗星，5=3颗星，9=5颗星
	int rate_strength;	//0x0010 力量
	int rate_defense;	//0x0014 防御
	int rate_agility;	//0x0018 敏捷
	int rate_magical;	//0x001C 魔法
};
DECLARE_OBJECT_MODULE_FACTORY(ITCGPetPictorialBook)

//! 游戏大区-道具电信-道具网通-时长-怀旧双鱼
class ITGameServerType :virtual public ITObject
{
public:
	int _server_type;		//!< 对应类型值 13 14  1 23 24
	//! 获取人物角色对象指针
	ITObjectPtr getRoleObjFromRoleName(const QString& sCharName);
	ITObjectPtr getGidObjFromGidName(const QString &sGid);

	void appendObject(ITObjectPtr pObj);
	void removeObject(ITObjectPtr pObj);

	QHash<QString, ITObjectPtr> _nameForObj;	//!< 游戏人物名称映射 汇总 
	QHash<QString, ITAccountGidPtr> _gidForObj;//!< 游戏gid和所辖人物角色列表映射 一般来说一个大区下有唯一gid  唯一gid下有左右两个角色
};
DECLARE_OBJECT_MODULE_FACTORY(ITGameServerType)

//! 角色建立的服务
class ITCharcterServer : public ITObject
{
public:
	QString _ip;
	int _port;
	int _big_line;
	int online;
	bool is_open;					//是否监听中 true监听中 false服务已关闭
	bool is_multicast ;
	QString multicast_ip ;			//组播ip 预留
	int multicast_port ;			//组播端口
};
DECLARE_OBJECT_MODULE_FACTORY(ITCharcterServer)
class ITHost : public ITObject
{
public:
	QString _sPcName;			//!< PC名称
	QString _sPcUserName;		//!< 当前登录用户名称
	QString _sPcMacAddr;		//!< 当前网卡MAC地址
	QString _sPcLocalIp;		//!< 当前网卡ip地址 注意：不等于公网ip
};
DECLARE_OBJECT_MODULE_FACTORY(ITHost)

#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>

typedef std::vector<std::string> stringList;
struct GamePetSolution
{
	int gr;
	double cl[5];
	double BP[5];
	double stat[5];
	double loss; 
	double cfLoss[5];
	double maxLoss[5];
	double spi; //���� = spi(this.cl) * solBase + 100
	double rev; //�ظ� = rev(this.cl) * solBase + 100
};
class GamePetCalcData
{
public:
	int number = 0;							   //ͼ�����
	std::string petName;					   //��������
	int petRaceType = -1;					   //������
	int level = 1;							   //����Ĭ�ϵȼ�
	double ability[5] = { 0, 0, 0, 0, 0 };	   //����ֵ ��ǰ���� Ѫħ������ ��ȥ20��ʼֵ����
	double curShowBp[5] = { 0, 0, 0, 0, 0 };   //bp ��ǰ������ʾ��bp
	double calcBp[5] = { 0, 0, 0, 0, 0 };	   //�����bp
	bool bInputBp = false;					   //�Ƿ�������bp
	double bpGrowing[5] = { 0, 0, 0, 0, 0 };   //bp�ɳ���
	double bestBPGrade[5] = { 0, 0, 0, 0, 0 }; //bp�����ɳ���
	double bpMax[5] = { 0, 0, 0, 0, 0 };	   //bp��ǰ�����ɳ���
	double bpMin[5] = { 0, 0, 0, 0, 0 };	   //bp��ǰ����С�ɳ���
	int totalGrade = 0;						   //�����ɳ��ۺϵ���
	double cfLoss[5] = { 0, 0, 0, 0, 0 };
	double maxLoss[5] = { 0, 0, 0, 0, 0 };
	double baseBP;								  //��������
	double spirit = 0;							  //����
	double recovery = 0;						  //�ָ�
	double lossMin = -1;						  //���ٵ�����
	double lossMax = -1;						  //��������
	double cfLossSummary[5] = { 0, 0, 0, 0, 0 };  //ÿһ�����ٵ�������
	double maxLossSummary[5] = { 0, 0, 0, 0, 0 }; //ÿһ������������
	std::vector<std::shared_ptr<GamePetSolution>> sols;
};
struct ParsePetCalcData
{
	double lossMin = -1;				//���ٵ�����
	double lossMax = -1;				//��������
};
class CGPetPictorialBook
{
public:
	int number;			   //���
	std::string name;		   //��������
	std::string raceTypeName;  //��������
	double bestBPGrade[5]; //�����ɳ�����
	double baseBP;		   //��������
	int totalGrade;		   //�ۺϵ���

	int can_catch;		//0x0001 0=���ɲ�׽��1=���Բ�׽
	int card_type;		//0x0002 0=�ޣ�1=������2=��
	int raceType;		//0x0003 ����
	int index;			//0x0004
	int image_id;		//0x0008
	int rate_endurance; //0x000C 0=����ǣ�3=2���ǣ�5=3���ǣ�9=5����
	int rate_strength;	//0x0010 ����
	int rate_defense;	//0x0014 ����
	int rate_agility;	//0x0018 ����
	int rate_magical;	//0x001C ħ��
	int element_earth;	//0x0020 ��
	int element_water;	//0x0024 ˮ
	int element_fire;	//0x0028 ��
	int element_wind;	//0x002C ��
	int skill_slots;	//0x0030 ������
};
class GPCalc 
{	
public:
	static GPCalc *GetInstance();

	double CalcBaseBP(int petNumber,int level=1);
	double CalcBaseBP(const std::string& petName,int level=1);
	std::shared_ptr<GamePetSolution> CreateSol(double solution[5], int grIndex, int level, int base);
	double CalcStat(int type, double dV[5]);
	double CalcSpi(double spiVals[5]);
	double CalcRev(double revVals[5]);
	std::shared_ptr<GamePetCalcData> CreatePetData(stringList inputData);
	std::shared_ptr<GamePetCalcData> ParseLine(stringList inputData);
	bool ParsePetGrade(stringList inputData, ParsePetCalcData& retData);
	std::string ReadIniValString(std::string strSection, std::string strKeyword, std::string strDefault, std::string strFileName);
	
	void initCaclPetData(stringList petData);// { m_cgPetDef = petData; }
	std::vector<std::string> string_split(std::string str, std::string pattern);

private:
	GPCalc();
	std::map<std::string, std::shared_ptr<CGPetPictorialBook> > m_cgPetDef;

	stringList args;
	double baseBP = 0;
	std::map<std::string, double> m_specialPetBase;
};
#define g_pGamePetCalc GPCalc::GetInstance()
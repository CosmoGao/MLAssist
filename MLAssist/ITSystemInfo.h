#pragma once
#include <memory>
#include <string>
#include <vector>

class ITSystemInfo : public std::enable_shared_from_this<ITSystemInfo>
{
public:
	//����������Ϣ
	struct LocalNetworkInfo
	{
		std::string strIP;
		std::string strMask;
		std::string strMac;
		std::string strGateway;
		std::string strGatewayMask;
		bool isDHCP;
	};
	//��ȡ���������
	static std::string GetPcName();

	//��ȡ�û�����
	static std::string GetPcUserName();

	//��ȡ������Ϣ
	std::vector<LocalNetworkInfo> GetNetworkInfo();
	static std::vector<LocalNetworkInfo> GetNetworkInfoFromQt();
};

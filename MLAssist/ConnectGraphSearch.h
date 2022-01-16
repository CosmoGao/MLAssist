//#pragma once
//#include <QList>
//#include <QSharedPointer>
//class GraphNode;
//typedef QSharedPointer<GraphNode> GraphNodePtr;
//typedef QList<GraphNodePtr> GraphNodeList;
//
//class GraphNode
//{
//public:
//	int id=0;			//��ͼid
//	int num = 0;				 //������ö������ڵĶ������
//	QList<int> nei;	 //��ö������ڵĶ������
//	QList<int> flag; //��ö������ڵĶ����Ƿ���ʹ�
//	bool isIn=false;		//�ö����Ƿ���ջ
//
//
//	GraphNodePtr parent = nullptr;
//
//	GraphNode(int id, GraphNodePtr parent)
//	{
//		this->id = id;
//		this->parent = parent;
//	}
//	int getOne()
//	{
//		int i = 0;
//		for (i = 0; i < num; i++)
//		{
//			if (flag[i] == 0) //�ж��Ƿ���ʹ�
//			{
//				flag[i] = 1; //��ʾ��������Ѿ������ʣ������䷵��
//				return nei[i];
//			}
//		}
//		return -1; //���ж��㶼�ѷ��ʹ��򷵻�-1
//	}
//	void resetFlag()
//	{
//		for (int i = 0; i < num; i++)
//		{
//			flag[i] = 0;
//		}
//	}
//};
//
//
//class ConnectGraphSearch
//{
//public:
//	ConnectGraphSearch(QHash<int, QList<int> > warpHash);
//	~ConnectGraphSearch();
//	int GetWeight(int i, int j);
//	int GetVertices();
//	GraphNodePtr GetValue(int i);
//	GraphNodePtr FindTarget(int startID, int targetID, QHash<int, QList<int> > warpHash);
//
//	QHash<int, GraphNodePtr> m_mapIndexForNode;
//};

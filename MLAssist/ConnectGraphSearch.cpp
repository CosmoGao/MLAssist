//#include "ConnectGraphSearch.h"
//
// ConnectGraphSearch::ConnectGraphSearch(QHash<int, QList<int> > warpHash)
//{
//	 for (auto it=warpHash.begin();it!=warpHash.end();++it)
//	 {
//		 GraphNodePtr tmpNode(new GraphNode(it.key(), nullptr));
//		 tmpNode->nei = it.value();
//		 tmpNode->flag.reserve(tmpNode->nei.size());
//		 tmpNode->num = tmpNode->nei.size();
//		 m_mapIndexForNode.insert(it.key(),tmpNode);
//	 }
//
//}
//
//GraphNodePtr ConnectGraphSearch::FindTarget(int startID, int targetID, QHash<int, QList<int> > warpHash)
//{
//	QList<int> hasSearchList;
//	GraphNodeList queue;
//
//	GraphNodePtr startNode = m_mapIndexForNode.value(startID);
//	queue.push_back(startNode);	//�����ѹջ
//
//	startNode->isIn = (true); //���Ϊ����ջ
//	int path_num = 0;
//	while (!queue.isEmpty()) //�ж�ջ�Ƿ��
//	{
//		GraphNodePtr node = queue.front();
//		queue.pop_front();
//		int flag = node->getOne(); //�õ����ڵĶ���
//		if (flag == -1)							  //������ڶ���ȫ�����ʹ�
//		{
//			node = queue.front();//ջ����һ��Ԫ��
//			queue.pop_front();					
//			node->resetFlag();			//�ö������ڵĶ�����Ϊδ����
//			node->isIn = (false);	    //�ö�����Ϊδ��ջ
//			continue;					//ȡջ�������ڽڵ�
//		}
//		auto neiNode = m_mapIndexForNode.value(flag);
//		if (neiNode && neiNode->isIn) //���Ѿ���ջ�У�ȡ��һ������
//		{
//			continue;
//		}
//		if (stack.getSize() > maxver - 1) //�ж�ջ�и����Ƿ񳬹����û�Ҫ��� ��������������һ��·���ڵ��������
//		{
//			int pop = stack.pop();
//			vertex[pop].resetFlag();
//			vertex[pop].setIsin(false);
//			continue;
//		}
//		stack.push(flag); //���ö�����ջ
//
//		vertex[flag].setIsin(true); //��Ϊ����ջ
//
//		if (stack.peek() == selection2) //���ջ���Ѿ�Ϊ���󣬽���·����¼
//		{
//			int *path = stack.getPath();
//			//����·���Ĵ���ʡ��
//			int pop = stack.pop();		//���䵯��������̽��
//			vertex[pop].setIsin(false); //�����ջ�ı�־λ
//		}
//	}
//}

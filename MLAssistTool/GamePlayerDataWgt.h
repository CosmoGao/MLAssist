#pragma once

#include <QWidget>
#include "PlayerDataTreeModel.h"
#include "ui_GamePlayerDataWgt.h"

class GamePlayerDataWgt : public QWidget
{
	Q_OBJECT

public:
	GamePlayerDataWgt(QWidget* parent = Q_NULLPTR);
	~GamePlayerDataWgt();

	void init();
	void initTable(QTableWidget* pTable,int nCount=20, int height=40);
protected:
	void resetModel(ITObjectList pObjList);
	QString DisplayInputDlg(const QString& title, const QString& labelName);
	void GetRowColFromItemPos(int itempos, int& row, int& col);
	void setItemText(QTableWidget* pTable, int row, int col, const QString& szText, const QColor& szColor = QColor("black"), const QColor& backColor = QColor("white"));
	void setItemText(int row, int col, const QString& szText, const QColor& szColor = QColor("black"));
	QString getGameState(ITGameCharacterPtr pRole);
	QString FormaClockIn(int val);
	QString getSystemTime(ITGameCharacterPtr pRole);
public slots:
	void on_treeView_customContextMenuRequested(const QPoint&);
	void doAddAccountAssemble();
	void doAddAccount();
	void doAddAccountGid();
	void doUpdateCharacterTable(ITGameCharacterPtr pRole);
	void doTreeViewClicked(const QModelIndex& index);
	void doUpdateBagItemTableWidget(ITGameCharacterPtr pRole);
	void doUpdateBankItemTableWidget(ITGameCharacterPtr pRole);
	void doUpdatePetTableWidget(QTableWidget *pTable, ITGameCharacterPtr pRole);
	void on_pushButton_refreshModel_clicked();
	void on_pushButton_search_clicked();
	void on_lineEdit_search_returnPressed();
	void on_pushButton_filterGid_clicked();
	void on_lineEdit_filter_returnPressed();
	void filterGid();
private:
	Ui::GamePlayerDataWgt ui;
	PlayerDataTreeModel* m_pPlayerDataTreeModel;
	ITObjectPtr m_curSelectObj;
	QMap<int,QString> m_petState;
};

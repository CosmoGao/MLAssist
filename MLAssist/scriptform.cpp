#include "scriptform.h"
#include "GameCtrl.h"
#include "ui_scriptform.h"
#include <intrin.h>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QMimeData>
#include <QPlainTextEdit>
#include <QTimer>

#include "MINT.h"

#include "../CGALib/gameinterface.h"

extern CGA::CGAInterface *g_CGAInterface;

ScriptForm::ScriptForm(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::ScriptForm)
{
	ui->setupUi(this);
	connect(g_pGameCtrl, &GameCtrl::NotifyAttachProcessOk, this, &ScriptForm::OnNotifyAttachProcessOk, Qt::QueuedConnection);
	connect(g_pGameCtrl, &GameCtrl::HttpLoadScript, this, &ScriptForm::OnHttpLoadScript);
	connect(g_pGameCtrl, &GameCtrl::NotifyFillLoadScript, this, &ScriptForm::OnNotifyFillLoadScript, Qt::QueuedConnection);

	m_output = new QPlainTextEdit(this);
	ui->verticalLayout_debug->addWidget(m_output);
	m_output->setReadOnly(true);
	m_output->show();

	m_bDebugging = false;
	m_bListening = false;
	m_bNavigating = false;
	m_bPathBegin = false;
	m_bSuspending = false;
	m_port = 0;
	m_ConsoleMaxLines = 100;
	m_output->setMaximumBlockCount(m_ConsoleMaxLines);
	m_node = new QProcess(this);

	connect(m_node, &QProcess::started, this, &ScriptForm::OnNodeStarted);
	connect(m_node, &QProcess::errorOccurred, this, &ScriptForm::OnNodeStartError);
	connect(m_node, SIGNAL(readyRead()), this, SLOT(OnNodeReadyRead()));
	connect(m_node, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(OnNodeFinish(int, QProcess::ExitStatus)));
	connect(ui->horizontalSlider_freezeDuration, SIGNAL(valueChanged(int)), this, SLOT(OnSetFreezeDuration(int)));

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(OnAutoRestart()));
	timer->start(1000);
	ui->label_freezeDuration->setText(tr("静止超时时间: %1 s").arg(ui->horizontalSlider_freezeDuration->value()));
}

void ScriptForm::OnSetFreezeDuration(int value)
{
	ui->label_freezeDuration->setText(tr("静止超时时间: %1 s").arg(value));
}
void ScriptForm::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("text/uri-list"))
	{
		event->acceptProposedAction();
	}
}

// 拖拽释放处理函数
void ScriptForm::dropEvent(QDropEvent *event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty())
	{
		return;
	}

	QString filePath = urls.first().toLocalFile();
	if (filePath.isEmpty())
	{
		return;
	}

	if (!ui->pushButton_term->isEnabled())
	{
		QFile file(filePath);
		if (file.exists())
		{
			m_scriptPath = filePath;
			m_output->appendPlainText(tr("Ready to launch"));
			ui->pushButton_run->setEnabled(true);
			ui->pushButton_debug->setEnabled(true);
			ui->pushButton_term->setEnabled(false);
			ui->lineEdit_scriptPath->setText(filePath);
		}
		else
		{
			QMessageBox::critical(this, tr("Error"), tr("Failed to load script file.\nerror: %1").arg(file.errorString()), QMessageBox::Ok, 0);
		}
	}
}

void ScriptForm::OnNodeStarted()
{
	if (m_bDebugging)
	{
		m_output->appendPlainText(tr("Debugging with pid %1...").arg(m_node->processId()));
	}
	else
	{
		m_output->appendPlainText(tr("Running with pid %1...").arg(m_node->processId()));
	}
	ui->pushButton_term->setEnabled(true);
}

void ScriptForm::OnNodeStartError(QProcess::ProcessError error)
{
	m_output->appendPlainText(tr("Failed to start node, error: %1").arg(m_node->errorString()));
	ui->pushButton_run->setEnabled(true);
	ui->pushButton_debug->setEnabled(true);
	ui->pushButton_term->setEnabled(false);
}

void ScriptForm::OnNodeReadyRead()
{
	if (!m_bListening)
		return;

	if (!m_bDebugging)
	{
		QString data = m_node->readAll();

		m_output->appendPlainText(data);

		if (m_bNavigating)
		{
			if (!m_bPathBegin)
			{
				QLatin1String patternbegin("[PATH BEGIN]");
				int findStart = data.indexOf(patternbegin);
				if (findStart != -1)
				{
					findStart += patternbegin.size();
					m_bPathBegin = true;
					QLatin1String patternend("[PATH END]");
					int findEnd = data.indexOf(patternend, findStart);
					if (findEnd != -1)
					{
						m_PathString += data.mid(findStart, findEnd - findStart);
						m_bPathBegin = false;
						ReportNavigatorPath(m_PathString);
					}
					else
					{
						m_PathString += data.mid(findStart);
					}
				}
			}
			else
			{
				QLatin1String patternend("[PATH END]");
				int findEnd = data.indexOf(patternend);
				if (findEnd != -1)
				{
					m_PathString += data.mid(0, findEnd);
					m_bPathBegin = false;
					ReportNavigatorPath(m_PathString);
				}
				else
				{
					m_PathString += data;
				}
			}
		}
	}
	else
	{
		QByteArray data = m_node->readAll();
		QLatin1String pattern("Debugger listening on ws://");

		int findStart = data.indexOf(pattern);
		if (findStart != -1)
		{
			m_output->appendPlainText(data);
			m_output->appendPlainText(tr("\nCheck \"chrome://inspect\" in chrome to debug the node process."));

			m_bListening = false;
		}
	}
}

void ScriptForm::OnNodeFinish(int exitCode, QProcess::ExitStatus exitStatus)
{
	if (m_bNavigating)
	{
		ReportNavigatorFinish(exitCode);
	}

	m_bListening = false;
	m_bNavigating = false;
	m_bPathBegin = false;

	m_output->appendPlainText(tr("Node finished with exitCode %1").arg(exitCode));

	ui->pushButton_load->setEnabled(true);
	ui->pushButton_run->setEnabled(true);
	ui->pushButton_debug->setEnabled(true);
	ui->pushButton_term->setEnabled(false);
	ui->pushButton_suspend->setEnabled(false);

	UpdateGameTextUI(false);

	//restore everything...
	g_CGAInterface->FixMapWarpStuck(2);
}

void ScriptForm::OnCloseWindow()
{
	if (m_node->state() == QProcess::Running)
	{
		m_node->kill();
		m_node->waitForFinished();
	}
}

void ScriptForm::OnAutoRestart()
{
	if (g_CGAInterface->IsConnected())
	{
		int ingame = 0;
		if (g_CGAInterface->IsInGame(ingame) && ingame)
		{
			int worldStatus = 0, gameStatus = 0;
			if (g_CGAInterface->GetWorldStatus(worldStatus) && g_CGAInterface->GetGameStatus(gameStatus) && gameStatus == 3)
			{
				bool bIsInBattleState = (worldStatus == 10) ? true : false;
				bool bIsInNormalState = (worldStatus == 9) ? true : false;

				CGA::cga_player_info_t playerinfo;
				if (g_CGAInterface->GetPlayerInfo(playerinfo))
				{
					if (playerinfo.health != 0 && ui->checkBox_injuryProt->isChecked())
					{
						if (m_node->state() == QProcess::Running)
						{
							on_pushButton_term_clicked();
							return;
						}
						else
						{
							return;
						}
					}
					if (playerinfo.souls != 0 && ui->checkBox_soulProt->isChecked())
					{
						if (m_node->state() == QProcess::Running)
						{
							on_pushButton_term_clicked();
							return;
						}
						else
						{
							return;
						}
					}
				}
				if (m_node->state() == QProcess::Running && ((ui->checkBox_freezestop->checkState() == Qt::CheckState::PartiallyChecked && !bIsInBattleState) || ui->checkBox_freezestop->checkState() == Qt::CheckState::Checked))
				{
					int x, y, index1, index2, index3;
					std::string filename;
					if (g_CGAInterface->GetMapXY(x, y) && g_CGAInterface->GetMapIndex(index1, index2, index3, filename))
					{
						if (x != m_LastMapX || y != m_LastMapY || index3 != m_LastMapIndex)
						{
							m_LastMapChange = QTime::currentTime();
							m_LastMapX = x;
							m_LastMapY = y;
							m_LastMapIndex = index3;
						}
						else
						{
							int freezeDuration = ui->horizontalSlider_freezeDuration->value();
							if (m_LastMapChange.elapsed() > freezeDuration * 1000)
							{
								on_pushButton_term_clicked();
								g_CGAInterface->LogBack();
								return;
							}
						}
					}
				}

				if (m_node->state() != QProcess::Running && !m_scriptPath.isEmpty())
				{
					if (ui->checkBox_autorestart->checkState() == Qt::CheckState::PartiallyChecked && bIsInNormalState)
					{
						on_pushButton_run_clicked();
						return;
					}
					if (ui->checkBox_autorestart->checkState() == Qt::CheckState::Checked && (bIsInNormalState || bIsInBattleState))
					{
						on_pushButton_run_clicked();
						return;
					}
				}
			}			
		}
		else
		{
			if (/*ui->checkBox_autoterm->isChecked() && */ m_node->state() == QProcess::Running)
			{
				on_pushButton_term_clicked();
				return;
			}
		}
	}
}

ScriptForm::~ScriptForm()
{
	delete ui;
}

void ScriptForm::on_pushButton_load_clicked()
{
	QFileDialog *fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(tr("Load Script"));
	fileDialog->setDirectory(".");
	fileDialog->setNameFilter(tr("JavaScript Files(*.js)"));
	fileDialog->setFileMode(QFileDialog::ExistingFile);
	if (fileDialog->exec() == QDialog::Accepted)
	{
		QString filePath = fileDialog->selectedFiles()[0];
		QFile file(filePath);
		if (file.exists())
		{
			m_scriptPath = filePath;
			m_output->appendPlainText(tr("Ready to launch"));
			ui->pushButton_run->setEnabled(true);
			ui->pushButton_debug->setEnabled(true);
			ui->pushButton_term->setEnabled(false);
			ui->lineEdit_scriptPath->setText(filePath);
		}
		else
		{
			QMessageBox::critical(this, tr("Error"), tr("Failed to load script file.\nerror: %1").arg(file.errorString()), QMessageBox::Ok, 0);
		}
	}
}

void ScriptForm::on_pushButton_run_clicked()
{
	int ingame = 0;
	if (m_port && ui->pushButton_run->isEnabled() && m_node->state() != QProcess::Running && g_CGAInterface->IsInGame(ingame) && ingame)
	{
		ui->pushButton_run->setEnabled(false);
		ui->pushButton_debug->setEnabled(false);
		ui->pushButton_term->setEnabled(false);
		ui->pushButton_load->setEnabled(false);
		ui->pushButton_suspend->setEnabled(true);
		ui->pushButton_suspend->setText(tr("暂停"));

		m_output->appendPlainText(tr("Starting node..."));
		m_LastMapChange = QTime::currentTime();
		m_bDebugging = false;
		m_bListening = true;
		m_bNavigating = false;
		m_bPathBegin = false;

		m_output->clear();

		QStringList args;
		QFileInfo fileInfo(m_scriptPath);

		args.append(fileInfo.fileName());
		args.append(QString::number(m_port));

		QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

		env.insert("CGA_GAME_PORT", qgetenv("CGA_GAME_PORT"));
		env.insert("CGA_GUI_PORT", qgetenv("CGA_GUI_PORT"));
		env.insert("CGA_DIR_PATH", qgetenv("CGA_DIR_PATH"));
		env.insert("CGA_DIR_PATH_UTF8", qgetenv("CGA_DIR_PATH_UTF8"));
		env.insert("CGA_GUI_PID", QString("%1").arg(GetCurrentProcessId()));
		env.insert("NODE_SKIP_PLATFORM_CHECK", "1");
		//env.insert("PATH", qgetenv("PATH")+";"+QDir::currentPath());
		m_node->setProcessEnvironment(env);

		m_node->setWorkingDirectory(fileInfo.dir().absolutePath());
		m_node->setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);
		m_node->start("node.exe", args);

		UpdateGameTextUI(true);
	}
}

void ScriptForm::StopNavigatorScript()
{
	if (m_bNavigating)
	{
		on_pushButton_term_clicked();
	}
}

void ScriptForm::UpdateGameTextUI(bool show)
{
	if (show && m_scriptPath.size())
	{
		QFileInfo fileInfo(m_scriptPath);
		QString fileName = tr("Executing: %1").arg(fileInfo.fileName());
		std::string path = fileName.toUtf8().toStdString();

		g_CGAInterface->SetGameTextUICurrentScript(path);
	}
	else
	{
		g_CGAInterface->SetGameTextUICurrentScript("");
	}
}

bool ScriptForm::IsRuningScript()
{
	return ui->pushButton_run->isEnabled();
}

QString ScriptForm::GetLoginScriptData()
{
	QString sLoginArg;
	sLoginArg = QString(" -loadscript=\"%2\" -scriptautorestart=%3 -scriptfreezestop=%4 -scriptautoterm=%5"
						" -injuryprotect=%6 -soulprotect=%7 -logbackrestart=%8 -transuserinput=%9 ")
						.arg(m_scriptPath)
						.arg(ui->checkBox_autorestart->isChecked())
						.arg(ui->checkBox_freezestop->isChecked())
						.arg(false)
						.arg(ui->checkBox_injuryProt->isChecked())
						.arg(ui->checkBox_soulProt->isChecked())
						.arg(false)
						.arg(false);
	return sLoginArg;
}

void ScriptForm::RunNavigatorScript(int x, int y, int enter, QString *result)
{
	if (!m_bNavigating && !m_bListening && !m_bDebugging && m_node->state() != QProcess::Running)
	{
		QString filePath = QCoreApplication::applicationDirPath() + "/navigator.js";
		m_scriptPath = filePath;
		m_output->appendPlainText(tr("Ready to launch"));
		ui->lineEdit_scriptPath->setText(filePath);

		ui->pushButton_run->setEnabled(false);
		ui->pushButton_debug->setEnabled(false);
		ui->pushButton_term->setEnabled(false);
		ui->pushButton_load->setEnabled(false);
		ui->pushButton_suspend->setEnabled(false);
		ui->checkBox_autorestart->setChecked(false);

		m_output->appendPlainText(tr("Starting node..."));

		m_bDebugging = false;
		m_bListening = true;
		m_bNavigating = true;
		m_bPathBegin = false;
		m_PathString = QString();

		m_output->clear();

		QStringList args;
		QFileInfo fileInfo(m_scriptPath);

		args.append(fileInfo.fileName());
		args.append(QString::number(m_port));
		args.append(QString::number(x));
		args.append(QString::number(y));
		args.append(QString::number(enter));

		QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

		env.insert("CGA_GAME_PORT", qgetenv("CGA_GAME_PORT"));
		env.insert("CGA_GUI_PORT", qgetenv("CGA_GUI_PORT"));
		env.insert("CGA_DIR_PATH", qgetenv("CGA_DIR_PATH"));
		env.insert("CGA_DIR_PATH_UTF8", qgetenv("CGA_DIR_PATH_UTF8"));
		env.insert("CGA_GUI_PID", QString("%1").arg(GetCurrentProcessId()));
		env.insert("NODE_SKIP_PLATFORM_CHECK", "1");
		m_node->setProcessEnvironment(env);

		m_node->setWorkingDirectory(fileInfo.dir().absolutePath());
		m_node->setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);
		m_node->start("node.exe", args);

		UpdateGameTextUI(true);
	}
	else
	{
		if (result)
			*result = tr("Navigation is unavailable while running other scripts.");
	}
}

void ScriptForm::on_pushButton_debug_clicked()
{
	if (m_port && ui->pushButton_debug->isEnabled() && m_node->state() != QProcess::Running)
	{
		ui->pushButton_run->setEnabled(false);
		ui->pushButton_debug->setEnabled(false);
		ui->pushButton_term->setEnabled(false);
		ui->pushButton_load->setEnabled(false);
		ui->pushButton_suspend->setEnabled(false);

		m_output->appendPlainText(tr("Starting node in debug mode..."));

		m_bDebugging = true;
		m_bListening = true;
		m_bNavigating = false;
		m_bPathBegin = false;

		QStringList args;
		QFileInfo fileInfo(m_scriptPath);

		args.append("--inspect");
		args.append(fileInfo.fileName());
		args.append(QString::number(m_port));

		QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

		env.insert("CGA_GAME_PORT", qgetenv("CGA_GAME_PORT"));
		env.insert("CGA_GUI_PORT", qgetenv("CGA_GUI_PORT"));
		env.insert("CGA_DIR_PATH", qgetenv("CGA_DIR_PATH"));
		env.insert("CGA_DIR_PATH_UTF8", qgetenv("CGA_DIR_PATH_UTF8"));
		env.insert("CGA_GUI_PID", QString("%1").arg(GetCurrentProcessId()));
		env.insert("NODE_SKIP_PLATFORM_CHECK", "1");
		m_node->setProcessEnvironment(env);

		m_node->setWorkingDirectory(fileInfo.dir().absolutePath());
		m_node->setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);
		m_node->start("node.exe", args);

		UpdateGameTextUI(true);
	}
}

void ScriptForm::on_pushButton_term_clicked()
{
	if (ui->pushButton_term->isEnabled() && m_node->state() == QProcess::Running)
	{

		ui->pushButton_term->setEnabled(false);

		m_node->kill();

		if (m_bDebugging)
			m_output->clear();
	}
}

void ScriptForm::OnNotifyAttachProcessOk(quint32 ProcessId, quint32 ThreadId, quint32 port, quint32 hWnd)
{
	m_port = port;

	QByteArray qportString = QString("%1").arg(port).toLocal8Bit();
	qputenv("CGA_GAME_PORT", qportString);
}

void ScriptForm::OnNotifyFillLoadScript(QString path, bool autorestart, bool freezestop,
	bool injuryprot, bool soulprot, int consolemaxlines, int logBackRestart, int transInput, int scriptfreezeduration)
{
	if (!path.endsWith("js"))
		return;
	if (!path.isEmpty())
	{
		if (QFile::exists(path) == false)
		{
			path = QApplication::applicationDirPath() + "//" + path;
		}
		QFile file(path);
		if (file.exists())
		{
			m_scriptPath = path;
			m_output->appendPlainText(tr("Ready to launch"));
			ui->pushButton_run->setEnabled(true);
			ui->pushButton_debug->setEnabled(true);
			ui->pushButton_term->setEnabled(false);
			ui->lineEdit_scriptPath->setText(path);
			on_pushButton_run_clicked();
		}
	}

	if (autorestart == 1)
		ui->checkBox_autorestart->setCheckState(Qt::CheckState::PartiallyChecked);
	else if (autorestart >= 2)
		ui->checkBox_autorestart->setCheckState(Qt::CheckState::Checked);
	if (injuryprot)
		ui->checkBox_injuryProt->setChecked(true);
	if (soulprot)
		ui->checkBox_soulProt->setChecked(true);
	if (freezestop)
		ui->checkBox_freezestop->setChecked(true);
	ui->horizontalSlider_freezeDuration->setValue(scriptfreezeduration);
	m_ConsoleMaxLines = consolemaxlines;
	m_output->setMaximumBlockCount(m_ConsoleMaxLines);
}

void ScriptForm::on_pushButton_suspend_clicked()
{
	if (ui->pushButton_suspend->isEnabled() && m_node->state() == QProcess::Running)
	{

		ui->pushButton_suspend->setEnabled(false);

		bool bSuccess = false;

		if (!m_bSuspending)
		{
			HANDLE ProcessHandle = OpenProcess(PROCESS_SUSPEND_RESUME, FALSE, (DWORD)m_node->processId());
			if (ProcessHandle)
			{
				if (STATUS_SUCCESS == NtSuspendProcess(ProcessHandle))
				{
					UpdateGameTextUI(false);
					m_bSuspending = true;
					ui->pushButton_suspend->setText(tr("继续"));
					ui->pushButton_suspend->setEnabled(true);
					bSuccess = true;

					g_CGAInterface->FixMapWarpStuck(2);
				}
				CloseHandle(ProcessHandle);
			}
		}
		else
		{
			HANDLE ProcessHandle = OpenProcess(PROCESS_SUSPEND_RESUME, FALSE, (DWORD)m_node->processId());
			if (ProcessHandle)
			{
				if (STATUS_SUCCESS == NtResumeProcess(ProcessHandle))
				{
					UpdateGameTextUI(true);
					m_bSuspending = false;
					ui->pushButton_suspend->setText(tr("暂停"));
					ui->pushButton_suspend->setEnabled(true);
					bSuccess = true;
				}
				CloseHandle(ProcessHandle);
			}
		}

		if (!bSuccess)
		{
			ui->pushButton_suspend->setEnabled(true);
		}
	}
}

void ScriptForm::OnHttpLoadScript(QString query, QByteArray postdata, QJsonDocument *doc)
{
	QJsonObject obj;

	QJsonParseError err;
	auto newdoc = QJsonDocument::fromJson(postdata, &err);
	if (err.error == QJsonParseError::NoError && newdoc.isObject())
	{
		obj.insert("errcode", 0);
		auto newobj = newdoc.object();
		if (newobj.contains("path"))
		{
			auto qpath = newobj.take("path");
			if (qpath.isString())
			{
				auto path = qpath.toString();
				if (!path.isEmpty())
				{
					QFile file(path);
					if (file.exists())
					{
						m_scriptPath = path;
						ui->lineEdit_scriptPath->setText(path);
					}
					else
					{
						obj.insert("errcode", 3);
						obj.insert("message", tr("script file not exists."));
						goto end;
					}
				}
				else
				{
					obj.insert("errcode", 4);
					obj.insert("message", tr("script path cannot be empty."));
					goto end;
				}
			}
		}
		if (newobj.contains("autorestart"))
		{
			auto qautorestart = newobj.take("autorestart");
			if (qautorestart.isBool())
			{
				ui->checkBox_autorestart->setChecked(qautorestart.toBool());
			}
			else if (qautorestart.toInt() == 2)
			{
				ui->checkBox_autorestart->setChecked(Qt::CheckState::Checked);
			}
			else if (qautorestart.toInt() == 1)
			{
				ui->checkBox_autorestart->setCheckState(Qt::CheckState::PartiallyChecked);
			}
			else if (qautorestart.toInt() == 0)
			{
				ui->checkBox_autorestart->setCheckState(Qt::CheckState::Unchecked);
			}
		}
		if (newobj.contains("injuryprot"))
		{
			auto qinjuryprot = newobj.take("injuryprot");
			if (qinjuryprot.isBool())
			{
				ui->checkBox_injuryProt->setChecked(qinjuryprot.toBool());
			}
		}
		if (newobj.contains("soulprot"))
		{
			auto qsoulprot = newobj.take("soulprot");
			if (qsoulprot.isBool())
			{
				ui->checkBox_soulProt->setChecked(qsoulprot.toBool());
			}
		}
		if (newobj.contains("freezestop"))
		{
			auto freezestop = newobj.take("freezestop");
			if (freezestop.isBool())
			{
				ui->checkBox_freezestop->setChecked(freezestop.toBool());
			}
		}
		if (newobj.contains("freezeduration"))
		{
			auto scriptfreezeduration = newobj.take("scriptfreezeduration");
			if (scriptfreezeduration.toInt() >= 10 && scriptfreezeduration.toInt() <= 600)
			{
				ui->horizontalSlider_freezeDuration->setValue(scriptfreezeduration.toInt());
			}
		}
	}
	else
	{
		obj.insert("errcode", 1);
		obj.insert("message", tr("json parse error"));
	}
end:
	doc->setObject(obj);
}

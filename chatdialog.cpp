#include "chatdialog.h"
#include "ui_chatdialog.h"
#include "netmanager.h"
#include "chatinputtextedit.h"

ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog)
{
    ui->setupUi(this);
}

ChatDialog::ChatDialog(QString myid, QString friendid, QString firstMsg,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog)
{
    ui->setupUi(this);
    this->myid = myid;
    this->friendid = friendid;
    this->initWithThisMsg = firstMsg;

    qDebug("this->myid[%s]", this->myid.toUtf8().constData() );
    qDebug("this->friendid[%s]", this->friendid.toUtf8().constData() );

    QObject::connect(ui->btnSendMsg, SIGNAL(clicked()), this, SLOT( SendMsg()));
    QObject::connect(ui->MyMsgTE, SIGNAL(sigEnterKeyed()), this, SLOT( SendMsg()));

    QTimer::singleShot(0, this, SLOT(Initialize()));
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

bool ChatDialog::event ( QEvent * event )
{
    int returnValue = QWidget::event(event);

    if (event->type() == QEvent::Close)
    {
        qDebug("Close Event");
        emit ChatDlgClosing(friendid);
    }

    return returnValue;
}


void ChatDialog::Initialize()
{
    qDebug("Initialize");

    this->setWindowTitle("From: "+ myid + " To: "+friendid);

    if(this->initWithThisMsg.length() > 0 )
    {
        ui->ChatMsgsTE->append(initWithThisMsg);
    }
}


void ChatDialog::SendMsg()
{
    NetManager::GetInstance().RequestChat( myid,  friendid,  ui->MyMsgTE->toPlainText () );
    ui->ChatMsgsTE->append(ui->MyMsgTE->toPlainText ());
    ui->MyMsgTE->clear();
}

void ChatDialog::SetInputEnabled (bool bEnable)
{
    ui->MyMsgTE->setEnabled(bEnable);

}

void ChatDialog::AppendMsg(QString newMsg)
{
    qDebug("newMsg[%s]", newMsg.toUtf8().constData() );

    ui->ChatMsgsTE->append(newMsg);
}

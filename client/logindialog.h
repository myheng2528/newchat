#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "httpmgr.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT


public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    void initHttpHandlers();

private:
    Ui::LoginDialog *ui;
    int _uid;
    QString _token;

private:
    bool checkUserValid();
    bool checkPwdValid();
    void showTip(QString s,bool isTrue);
    bool enableBtn(bool enabled);
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
signals:
    void switchRegister();
    void switchReset();
    void sig_connect_tcp(ServerInfo si);
public slots:
    void slot_login_mod_finish(ReqId id, QString res, ErrorCodes err);
    void on_login_btn_clicked();
    void slot_tcp_con_finish(bool bsuccess);
};

#endif // LOGINDIALOG_H

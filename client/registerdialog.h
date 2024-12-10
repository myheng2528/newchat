#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H
#include "global.h"
#include <QDialog>
#include "timerbtn.h"
namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_get_btn_clicked();
    void showTip(QString s,bool isTrue);

private:
    Ui::RegisterDialog *ui;
    QMap<ReqId,std::function<void(const QJsonObject& jsonObj)>> _handlers;
    QMap<TipErr, QString> _tip_errs;
    QTimer *_countdown_timer;
    int _countdown;
private:
    void initHttpHandlers();
    bool checkUserValid();
    bool checkEmailValid();
    bool checkPassValid();
    bool checkConfirmValid();
    bool checkVarifyValid();
    void AddTipErr(TipErr te, QString tips);
    void DelTipErr(TipErr te);
    void ChangeTipPage();
private slots:
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
    void on_sure_btn_clicked();
    void on_return_btn_clicked();
    void on_cancel_btn_clicked();
signals:
    void sigSwitchLogin();

};

#endif // REGISTERDIALOG_H

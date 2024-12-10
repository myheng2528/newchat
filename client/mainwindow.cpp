#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _login_dlg = std::make_shared<LoginDialog>();
    setCentralWidget(_login_dlg.get());
    _login_dlg->show();
    connect(_login_dlg.get(),&LoginDialog::switchRegister,this,&MainWindow::SoltSwitchReg);
    connect(_login_dlg.get(),&LoginDialog::switchReset,this,&MainWindow::SlotSwitchReset);

    _reg_dlg = std::make_shared<RegisterDialog>();
    connect(_reg_dlg.get(),&RegisterDialog::sigSwitchLogin,this,&MainWindow::SlotSwitchLogin);

    _reset_dlg = std::make_shared<ResetDialog>();
    connect(_reset_dlg.get(),&ResetDialog::switchLogin,this,&MainWindow::SlotSwitchLogin2);

    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    _reg_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    _reset_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SoltSwitchReg()
{
    takeCentralWidget();
    setCentralWidget(_reg_dlg.get());
    //_login_dlg->hide();

}

void MainWindow::SlotSwitchLogin(){
    takeCentralWidget();
    setCentralWidget(_login_dlg.get());
}

void MainWindow::SlotSwitchLogin2()
{
    takeCentralWidget();
    setCentralWidget(_login_dlg.get());
}

void MainWindow::SlotSwitchReset()
{
    takeCentralWidget();
    setCentralWidget(_reset_dlg.get());
}

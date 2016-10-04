#include "mymessage.h"
#include "ui_mymessage.h"

myMessage::myMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::myMessage)
{
    ui->setupUi(this);
}

myMessage::~myMessage()
{
    delete ui;
}

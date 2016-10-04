#ifndef MYMESSAGE_H
#define MYMESSAGE_H

#include <QDialog>

namespace Ui {
class myMessage;
}

class myMessage : public QDialog
{
    Q_OBJECT

public:
    explicit myMessage(QWidget *parent = 0);
    ~myMessage();

private:
    Ui::myMessage *ui;
};

#endif // MYMESSAGE_H

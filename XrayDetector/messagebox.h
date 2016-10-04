#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QDialog>

namespace Ui {
class messageBox;
}

class messageBox : public QDialog
{
    Q_OBJECT

public:
    explicit messageBox(QWidget *parent = 0);
    ~messageBox();

private:
    Ui::messageBox *ui;
};

#endif // MESSAGEBOX_H

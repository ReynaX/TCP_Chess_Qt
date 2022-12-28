#ifndef JOINGAMEDIALOG_H
#define JOINGAMEDIALOG_H

#include <QDialog>

namespace Ui {
class JoinGameDialog;
}

class JoinGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit JoinGameDialog(QWidget *parent = nullptr);
    ~JoinGameDialog();
    QString getGameID() const;
private:
    Ui::JoinGameDialog *ui;
};

#endif // JOINGAMEDIALOG_H

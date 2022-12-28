#include "joingamedialog.h"
#include "ui_joingamedialog.h"

#include <QIntValidator>

JoinGameDialog::JoinGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JoinGameDialog)
{
    ui->setupUi(this);
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    ui->gameID->setValidator(new QIntValidator(0, std::numeric_limits<int>::max(), this));
}

JoinGameDialog::~JoinGameDialog(){
    delete ui;
}

QString JoinGameDialog::getGameID() const{
    return ui->gameID->text();
}

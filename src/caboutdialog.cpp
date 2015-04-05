// Copyright 2014, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
#include "caboutdialog.h"
#include "ui_caboutdialog.h"
#include "settings.h"
#include <QFile>

CAboutDialog::CAboutDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::CAboutDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("About %1 - %2").arg(appName(), appVersion()));

    ui->label_programmTitle->setText(tr("About %1 - %2").arg(appName(), appVersion()));

    ui->plainTextEdit_about->setPlainText
            (fromResource(":/about/ABOUT"));
    ui->plainTextEdit_authors->setPlainText
            (fromResource(":/about/AUTHORS"));
    ui->plainTextEdit_changelog->setPlainText
            (fromResource(":/about/CHANGELOG"));
    ui->plainTextEdit_license->setPlainText
            (fromResource(":/about/LICENSE"));
    ui->plainTextEdit_libraries->setPlainText
            (fromResource(":/about/LIBRARIES"));
}

CAboutDialog::~CAboutDialog()
{
    delete ui;
}

QString CAboutDialog::fromResource(const QString &resourceName)
{
    QFile text(resourceName);
    text.open(QIODevice::ReadOnly);
    return QString::fromUtf8(text.readAll());
}

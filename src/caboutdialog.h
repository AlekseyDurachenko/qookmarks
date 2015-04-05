// Copyright 2014, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
#ifndef CABOUTDIALOG_H
#define CABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class CAboutDialog;
}

class CAboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CAboutDialog(QWidget *parent = 0);
    virtual ~CAboutDialog();
private:
    QString fromResource(const QString& resourceName);
private:
    Ui::CAboutDialog *ui;
};

#endif // CABOUTDIALOG_H

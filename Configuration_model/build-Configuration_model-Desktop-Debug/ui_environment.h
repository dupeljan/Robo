/********************************************************************************
** Form generated from reading UI file 'environment.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENVIRONMENT_H
#define UI_ENVIRONMENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Environment
{
public:

    void setupUi(QWidget *Environment)
    {
        if (Environment->objectName().isEmpty())
            Environment->setObjectName(QStringLiteral("Environment"));
        Environment->resize(400, 400);
        Environment->setMinimumSize(QSize(400, 400));
        Environment->setMaximumSize(QSize(400, 400));

        retranslateUi(Environment);

        QMetaObject::connectSlotsByName(Environment);
    } // setupUi

    void retranslateUi(QWidget *Environment)
    {
        Environment->setWindowTitle(QApplication::translate("Environment", "Environment", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Environment: public Ui_Environment {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENVIRONMENT_H

/********************************************************************************
** Form generated from reading UI file 'VSettings.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSETTINGS_H
#define UI_VSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VSettingsClass
{
public:

    void setupUi(QWidget *VSettingsClass)
    {
        if (VSettingsClass->objectName().isEmpty())
            VSettingsClass->setObjectName("VSettingsClass");
        VSettingsClass->resize(600, 400);

        retranslateUi(VSettingsClass);

        QMetaObject::connectSlotsByName(VSettingsClass);
    } // setupUi

    void retranslateUi(QWidget *VSettingsClass)
    {
        VSettingsClass->setWindowTitle(QCoreApplication::translate("VSettingsClass", "VSettings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VSettingsClass: public Ui_VSettingsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSETTINGS_H

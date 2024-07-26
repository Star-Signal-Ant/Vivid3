/********************************************************************************
** Form generated from reading UI file 'VCreateScript.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VCREATESCRIPT_H
#define UI_VCREATESCRIPT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_VCreateScriptClass
{
public:

    void setupUi(QDialog *VCreateScriptClass)
    {
        if (VCreateScriptClass->objectName().isEmpty())
            VCreateScriptClass->setObjectName("VCreateScriptClass");
        VCreateScriptClass->resize(600, 400);

        retranslateUi(VCreateScriptClass);

        QMetaObject::connectSlotsByName(VCreateScriptClass);
    } // setupUi

    void retranslateUi(QDialog *VCreateScriptClass)
    {
        VCreateScriptClass->setWindowTitle(QCoreApplication::translate("VCreateScriptClass", "VCreateScript", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VCreateScriptClass: public Ui_VCreateScriptClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VCREATESCRIPT_H

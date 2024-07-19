/********************************************************************************
** Form generated from reading UI file 'VScriptClass.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCRIPTCLASS_H
#define UI_VSCRIPTCLASS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VScriptClassClass
{
public:

    void setupUi(QWidget *VScriptClassClass)
    {
        if (VScriptClassClass->objectName().isEmpty())
            VScriptClassClass->setObjectName("VScriptClassClass");
        VScriptClassClass->resize(600, 400);

        retranslateUi(VScriptClassClass);

        QMetaObject::connectSlotsByName(VScriptClassClass);
    } // setupUi

    void retranslateUi(QWidget *VScriptClassClass)
    {
        VScriptClassClass->setWindowTitle(QCoreApplication::translate("VScriptClassClass", "VScriptClass", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VScriptClassClass: public Ui_VScriptClassClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCRIPTCLASS_H

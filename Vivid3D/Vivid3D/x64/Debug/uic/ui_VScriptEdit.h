/********************************************************************************
** Form generated from reading UI file 'VScriptEdit.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCRIPTEDIT_H
#define UI_VSCRIPTEDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VScriptEditClass
{
public:

    void setupUi(QWidget *VScriptEditClass)
    {
        if (VScriptEditClass->objectName().isEmpty())
            VScriptEditClass->setObjectName("VScriptEditClass");
        VScriptEditClass->resize(600, 400);

        retranslateUi(VScriptEditClass);

        QMetaObject::connectSlotsByName(VScriptEditClass);
    } // setupUi

    void retranslateUi(QWidget *VScriptEditClass)
    {
        VScriptEditClass->setWindowTitle(QCoreApplication::translate("VScriptEditClass", "VScriptEdit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VScriptEditClass: public Ui_VScriptEditClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCRIPTEDIT_H

/********************************************************************************
** Form generated from reading UI file 'VTextEditor.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VTEXTEDITOR_H
#define UI_VTEXTEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VTextEditorClass
{
public:

    void setupUi(QWidget *VTextEditorClass)
    {
        if (VTextEditorClass->objectName().isEmpty())
            VTextEditorClass->setObjectName("VTextEditorClass");
        VTextEditorClass->resize(600, 400);

        retranslateUi(VTextEditorClass);

        QMetaObject::connectSlotsByName(VTextEditorClass);
    } // setupUi

    void retranslateUi(QWidget *VTextEditorClass)
    {
        VTextEditorClass->setWindowTitle(QCoreApplication::translate("VTextEditorClass", "VTextEditor", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VTextEditorClass: public Ui_VTextEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VTEXTEDITOR_H

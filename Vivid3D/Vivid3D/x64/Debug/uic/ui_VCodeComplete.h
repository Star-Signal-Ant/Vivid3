/********************************************************************************
** Form generated from reading UI file 'VCodeComplete.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VCODECOMPLETE_H
#define UI_VCODECOMPLETE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VCodeCompleteClass
{
public:

    void setupUi(QWidget *VCodeCompleteClass)
    {
        if (VCodeCompleteClass->objectName().isEmpty())
            VCodeCompleteClass->setObjectName("VCodeCompleteClass");
        VCodeCompleteClass->resize(600, 400);

        retranslateUi(VCodeCompleteClass);

        QMetaObject::connectSlotsByName(VCodeCompleteClass);
    } // setupUi

    void retranslateUi(QWidget *VCodeCompleteClass)
    {
        VCodeCompleteClass->setWindowTitle(QCoreApplication::translate("VCodeCompleteClass", "VCodeComplete", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VCodeCompleteClass: public Ui_VCodeCompleteClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VCODECOMPLETE_H

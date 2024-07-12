/********************************************************************************
** Form generated from reading UI file 'VAnimationList.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VANIMATIONLIST_H
#define UI_VANIMATIONLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VAnimationListClass
{
public:

    void setupUi(QWidget *VAnimationListClass)
    {
        if (VAnimationListClass->objectName().isEmpty())
            VAnimationListClass->setObjectName("VAnimationListClass");
        VAnimationListClass->resize(600, 400);

        retranslateUi(VAnimationListClass);

        QMetaObject::connectSlotsByName(VAnimationListClass);
    } // setupUi

    void retranslateUi(QWidget *VAnimationListClass)
    {
        VAnimationListClass->setWindowTitle(QCoreApplication::translate("VAnimationListClass", "VAnimationList", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VAnimationListClass: public Ui_VAnimationListClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VANIMATIONLIST_H

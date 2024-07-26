#include "VCreateScript.h"
#include <QVBoxLayout>
#include <QSplitter>
#include <QTreeWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QFormLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <qlineedit.h>
#include <QPushButton>
#include "TemplateManager.h"
#include "VContentView.h"

VCreateScript::VCreateScript(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

    setFixedSize(400, 150);

    QVBoxLayout* bl = new QVBoxLayout;

    QLineEdit* m_Name = new QLineEdit(this);
    QFormLayout* form = new QFormLayout;
    form->addRow("Script Name:", m_Name);
    QComboBox* m_ScriptType = new QComboBox(this);
    m_ScriptType->addItem("GameScript");
    m_ScriptType->addItem("UIScript");
    m_ScriptType->addItem("Editor Plugin");
    form->addRow("Script Type:", m_ScriptType);

    auto cr = new QPushButton("Create");
    auto cancel = new QPushButton("Cancel");

    bl->addLayout(form);

    auto el = new QHBoxLayout;
    el->addWidget(cr);
    el->addWidget(cancel);
    bl->addLayout(el);

    bl->setAlignment(Qt::AlignTop);

    setLayout(bl);

    QObject::connect(cr, &QPushButton::clicked, [m_Name,m_ScriptType,this]() {
    //    QMessageBox::information(&window, "Lambda", "Button clicked!");
        std::string name = m_Name->text().toStdString();
        if (m_ScriptType->currentIndex() == 0)
        {
            TemplateManager* tmp = new TemplateManager;
            tmp->LoadTemplate("v/template/gamescript.v");
            tmp->SwitchText("#NAME#", name);
            tmp->SaveTemplate(m_Path + "\\" + name + ".v");
            VContentView::m_This->Browse(m_Path);
        }
        close();
        
        });

    QObject::connect(cancel, &QPushButton::clicked, [this]() {
        close();
        });

}

VCreateScript::~VCreateScript()
{}

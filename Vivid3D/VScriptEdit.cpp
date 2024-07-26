#include "VScriptEdit.h"
#include "qsplitter.h"
#include <QTextDocument>
#include <QTextEdit>
#include <QTextCursor>
#include <QListWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QPoint>
#include <QRect>
#include <qabstracttextdocumentlayout.h>
#include <qtextbrowser.h>
#include <QTextBlock>
#include <QTextDocument>
#include "VVarGroup.h"
#include "ScriptHost.h"
#include <unordered_set>
#include <algorithm>
VScriptEdit::VScriptEdit(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

    //setBaseSize(700, 700);
    resize(800, 700);


    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Create menu bar
    QMenuBar* menuBar = new QMenuBar(this);

    // Create file menu and add actions
    QMenu* fileMenu = menuBar->addMenu("File");
    QAction* newAction = fileMenu->addAction("New");
    QAction* openAction = fileMenu->addAction("Open");
    QAction* saveAction = fileMenu->addAction("Save");
    fileMenu->addSeparator();
    QAction* exitAction = fileMenu->addAction("Exit");

    // Create edit menu and add actions
    QMenu* editMenu = menuBar->addMenu("Edit");
    QAction* cutAction = editMenu->addAction("Cut");
    QAction* copyAction = editMenu->addAction("Copy");
    QAction* pasteAction = editMenu->addAction("Paste");

    // Create toolbar
    QToolBar* toolBar = new QToolBar(this);
    toolBar->addAction(newAction);
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addSeparator();
    toolBar->addAction(cutAction);
    toolBar->addAction(copyAction);
    toolBar->addAction(pasteAction);

    // Add menu bar and toolbar to the layout
    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(toolBar);

    QSplitter* sp = new QSplitter(Qt::Orientation::Vertical);

    // Create a central widget (e.g., QTextEdit)
    VTextEditor* textEdit = new VTextEditor(this);
   // mainLayout->addWidget(textEdit);
    m_Console = new QTextEdit(this);
    //mainLayout->addWidget(m_Console);
    sp->addWidget(textEdit);
    sp->addWidget(m_Console);
    mainLayout->addWidget(sp);
    QList<int> sizes;
    sizes << 500 << 200;  // Set the desired sizes for the top and bottom widgets
    sp->setSizes(sizes);

 //   m_Console->setMaximumHeight(250);
    setLayout(mainLayout);
    m_This = this;
    m_Edit = textEdit;
    m_MainTimer = new QTimer(this);

    connect(m_MainTimer, &QTimer::timeout, this, &VScriptEdit::on_Timer);

    m_MainTimer->setInterval(1000);
    m_MainTimer->start();




    // Connect the textChanged signal of the QTextEdit to the onTextChanged slot
    connect(m_Edit, &QTextEdit::textChanged, this, &VScriptEdit::onCodeChanged);
    //Offer AI enabled responses to compile problems/errors.

    m_Codes["class"] = KeyColor(42, 104, 189);
    m_Codes["func"] = KeyColor(209, 151, 187);
    m_Codes["end"] = KeyColor(209, 151, 187);
    m_Codes["module"] = KeyColor(200, 0, 0);
    m_Codes["if"] =  KeyColor(209, 151, 187);
    m_Codes["else"] = KeyColor(209, 151, 187);
    m_Codes["elseif"] = KeyColor(209, 151, 187);
    m_Codes["switch"] = KeyColor(209, 151, 187);
    m_Codes["break"] = KeyColor(209, 151, 187);
    m_Codes["return"] = KeyColor(209, 151, 187);
    m_Codes["for"] = KeyColor(209, 151, 187);
    m_Codes["int"] = KeyColor(42, 104, 189);
    m_Codes["float"] = KeyColor(42, 104, 189);
    m_Codes["string"] = KeyColor(42, 104, 189);
    m_Codes["enum"] = KeyColor(42, 104, 189);

  
    

    m_Edit->setTabStopDistance(10);

    QObject::connect(m_Edit, &QTextEdit::cursorPositionChanged, [this]() {
    
        UpdateCodeComplete();

        });



}

void VScriptEdit::LoadScript(std::string path) {

    QFile file(path.c_str());
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        m_Edit->setPlainText(in.readAll());
        file.close();
    }

}

VScriptEdit* VScriptEdit::m_This = nullptr;

VScriptEdit::~VScriptEdit()
{}


std::string VScriptEdit::getText()
{
    // Get the content of the QTextEdit as a QString
    QString qText = m_Edit->toPlainText();

    // Convert QString to std::string
    std::string stdText = qText.toStdString();

    return stdText;
}

void VScriptEdit::on_Timer() {

    if (!m_CodeChanged) return;
    m_CodeChanged = false;

    m_Console->clear();
    auto code = getText();

    VSource* src = new VSource();
    src->SetSource(code);
    VTokenizer* toker = new VTokenizer();

    auto tokes = toker->Tokenize(src);

    VParser* parser = new VParser;

    parser->SetOutput(VScriptEdit::compile_Output);

    auto mod = parser->ParseModule(tokes);

    if (mod == nullptr) {

    }else
    if (m_CodeModule == nullptr) {
        m_CodeModule = mod;

    }else
    if (mod->GetClasses().size() < m_CodeModule->GetClasses().size()) {

    }
    else {
        m_CodeModule = mod;
    }
 //   m_CodeChanged = false;

    UpdateCodeComplete();

    auto word = GetCurrentWord();

    AppendConsole("Current Word:" + word);

    Highlight();


}

std::string VScriptEdit::GetCurrentWord() {

    QTextCursor cursor = m_Edit->textCursor();
    if (!cursor.hasSelection()) {
        cursor.select(QTextCursor::WordUnderCursor);
    }

    // Get the selected text (the word under the cursor)
    QString selectedText = cursor.selectedText();

    // If there's no text selected, return an empty string
    if (selectedText.isEmpty()) {
        return "";
    }

    // Convert the selected text to a std::string and return it
    return selectedText.toStdString();

}


std::vector<std::string> removeDuplicates(std::vector<std::string>& vec) {
    std::unordered_set<std::string> seen;
    auto it = std::remove_if(vec.begin(), vec.end(),
        [&seen](const std::string& str) {
            if (seen.find(str) != seen.end()) {
                return true;  // Remove this element
            }
            seen.insert(str);
            return false;  // Keep this element
        });

    vec.erase(it, vec.end());
    return vec;
}

void VScriptEdit::UpdateCodeComplete() {

    m_ComList.clear();

    if (m_CodeModule == nullptr) return;
    if (m_CodeModule->GetClasses().size() == 0) return;

    for (auto cls : m_CodeModule->GetClasses()) {

        if (cls == nullptr) continue;
        m_ComList.push_back(cls->GetName().GetNames()[0]);
        for (auto func : cls->GetFunctions()) {
            m_ComList.push_back(func->GetName().GetNames()[0].c_str());
        }
        for (auto v : cls->GetGroups()) {
            for (auto av : v->GetNames()) {

                m_ComList.push_back(av.GetNames()[0].c_str());

            }
        }


    }

    for (auto m : ScriptHost::m_This->GetContext()->GetModules()) {

        for (auto cls : m->GetClasses()) {

            if (cls == nullptr) continue;
            m_ComList.push_back(cls->GetName().GetNames()[0]);
            for (auto func : cls->GetFunctions()) {
                m_ComList.push_back(func->GetName().GetNames()[0].c_str());
            }
            for (auto v : cls->GetGroups()) {
                for (auto av : v->GetNames()) {

                    m_ComList.push_back(av.GetNames()[0].c_str());

                }
            }


        }
    }

    m_ComList.push_back("class");
    m_ComList.push_back("if");
    m_ComList.push_back("else");
    m_ComList.push_back("end");
    m_ComList.push_back("class");
    m_ComList.push_back("elseif");
    m_ComList.push_back("func");
    m_ComList.push_back("return");
    m_ComList.push_back("lambda");
    m_ComList.push_back("switch");
    m_ComList.push_back("break");
    m_ComList.push_back("for");
    m_ComList.push_back("static");


    m_ComList= removeDuplicates(m_ComList);


    auto word = GetCurrentWord();
    
    if (word.size() > 0) {

        if (m_Edit->GetCodeComplete() != nullptr) {
            m_Edit->GetCodeComplete()->close();
            m_Edit->SetCodeComplete(nullptr);
        }
        QTextCursor cursor = m_Edit->textCursor();
        QTextDocument* document = m_Edit->document();
        QPointF cursorPos = m_Edit->viewport()->mapToGlobal(document->documentLayout()->blockBoundingRect(cursor.block()).topLeft());

        // Create and configure the code completion widget
        VCodeComplete* completeWidget = new VCodeComplete();
        //completeWidget->setSuggestions(suggestions);
        m_Edit->SetCodeComplete(completeWidget);

        // Set the widget position to the cursor position
        QRect cursorRect = m_Edit->cursorRect(cursor);
        QPoint widgetPos = m_Edit->viewport()->mapToGlobal(cursorRect.bottomLeft());
        widgetPos.setX(widgetPos.x() + 32);
        widgetPos.setY(widgetPos.y() - 64);
        completeWidget->move(widgetPos);
        completeWidget->SetWord(word);
        for (auto item : m_ComList) {

            completeWidget->AddItem(item);

        }
        if (completeWidget->HasWord(word)) {

        }
        else {
            if (completeWidget->GetCount() == 0) {

            }
            else {

                completeWidget->SetWord(word);
                // Show the widget
                completeWidget->show();
            }
        }
    }
    else {
        if (m_Edit->GetCodeComplete() != nullptr) {
            //m_CodeComplete->close();
            m_Edit->GetCodeComplete()->close();
            m_Edit->SetCodeComplete(nullptr);
            //m_CodeComplete = nullptr;

        }
    }

}

void VScriptEdit::Highlight() {

    QTextDocument* document = m_Edit->document();
    QTextCursor cursor(document);

    // Create a format for highlighting
    QTextCharFormat highlightFormat;
    //highlightFormat.setBackground(color);


    for(const auto & pair : m_Codes) {
        const std::string& word = pair.first;
        const KeyColor& color = pair.second;

        // Create a format for highlighting
        QTextCharFormat textFormat;
        textFormat.setForeground(QColor(color.R,color.G,color.B));

        // Start searching from the beginning of the document
        QTextCursor cursor(document);
        cursor.movePosition(QTextCursor::Start);

        while (!cursor.isNull() && !cursor.atEnd()) {
            cursor = document->find(QString::fromStdString(word), cursor);
            if (!cursor.isNull()) {
                // Apply the text color format
                cursor.mergeCharFormat(textFormat);
                cursor.movePosition(QTextCursor::NextCharacter); // Move past the found word
            }
        }
    }
    m_CodeChanged = false;
}

void VScriptEdit::onCodeChanged()
{
    m_CodeChanged = true;
    // This function will be called every time the text in the QTextEdit changes
    //QString currentText = textEdit->toPlainText();
    //qDebug() << "Text changed to:" << currentText;
}

void VScriptEdit::compile_Output(const std::string& s1, const std::string& s2) {
    //std::cout << "compile_Output called with: " << s1 << " and " << s2 << std::endl;
    m_This->AppendConsole("Error:" + s1 + " Type:" + s2);
}

void VScriptEdit::AppendConsole(std::string msg) {

    m_Console->append(msg.c_str());

}
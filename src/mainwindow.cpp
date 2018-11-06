#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Assembler/Assembler.h"
#include <QDebug>
#include <QMessageBox>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_chooseFile_btn_clicked()
{
    char* docdir = getenv("USERPROFILE");
    string path;
    if(docdir)
        {
            path = docdir;
            path += "\\Documents";
        }
    else
        path = "C:\\";
    directory = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     QString(path.c_str()),
                                                     tr("Hack Assembly (*.asm)"));

    ifstream sourceFile(directory.toStdString());
    string s, text;
    while(getline(sourceFile, s))
        text += s + "\n";
    ui->sourceFileAsText->setText(QString::fromStdString(text));
    sourceFile.close();
}

void MainWindow::on_Translate_btn_clicked()
{
    ifstream sourceFile;
    ofstream outputFile;
    Assembler assembler;

    string sourceFilePath = directory.toStdString();

    string outputFilePath;
    outputFilePath = sourceFilePath.substr(0, sourceFilePath.size() - 3) + "hack";

    sourceFile.open(sourceFilePath);
    outputFile.open(outputFilePath, std::ios::trunc);

    assembler.firstPass(sourceFile);

    sourceFile.clear();
    sourceFile.seekg(0, std::ios::beg);

    assembler.secondPass(sourceFile, outputFile);

    sourceFile.close();
    outputFile.close();

    sourceFile.open(outputFilePath);
    string s, text;
    while(getline(sourceFile, s))
        text += s + "\n";
    ui->outFileAsText->setText(QString::fromStdString(text));
    sourceFile.close();

}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msg;
    msg.setText("<b style='font-size: 12pt;'>&#169; Andrea Esposito, 2017</b>");
    msg.setInformativeText("This Assembler was developed by:"
                           "<ul>"
                           "<li>&#160; Andrea Esposito - UniBa, ICD Taranto (I year)</li>"
                           "</ul>");
    msg.setStandardButtons(QMessageBox::Close);

    QSpacerItem* horizontalSpacer = new QSpacerItem(400, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msg.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    msg.show();

    msg.exec();
}

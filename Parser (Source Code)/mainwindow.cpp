#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Parser.h"
#include "QCursor"
#include <QTextEdit>
#include <QScrollBar>

string code, tokens, fileName = "tokens.txt";
int cnt = 0;
bool slider = false, image = false, autoSave = true;

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
QString getName(QString path)
{
    int index;
    for(int i=path.length()-1; i > 0 ; i--) {
        if (path[i] == '/') {
            index = i;
            break;
        }
    }
    return path.mid(index+1, path.length()-index-1);
}
void MainWindow::on_importBTN_clicked()
{
    // Creating File Dialog
    QString fileName = QFileDialog::getOpenFileName(
                this,
                "Import Tokens File",
                ".",
                "Text files (*.txt);; All files (*.*)");
    ui->fileName->setText(getName(fileName));
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    QString tokens = stream.readAll();
    ui->TokensList2->setPlainText(tokens);
}
void MainWindow::on_tokensBTN_clicked()
{
    // Get Tokens List
    string codeSTR = ui->TinyCode->toPlainText().toUtf8().constData();
    if (codeSTR == "") {
        ui->TokensList1->setPlainText("");
        ui->TokensList2->setPlainText("");
    }
    else {
        code = readFile(codeSTR);
        tokens = getTokens(code);
        ui->TokensList1->setPlainText(QString::fromStdString(tokens));
        ui->TokensList2->setPlainText(QString::fromStdString(tokens));
        createFile(fileName);
        writeFile(tokens, fileName);
    }
}
void MainWindow::on_checkBTN_clicked()
{
    string TokensSTR = ui->TokensList2->toPlainText().toUtf8().constData();
    if (TokensSTR == "") {
        ui->checkLBL->setText("Please Enter Tokens List");
        ui->checkLBL->setStyleSheet("QLabel { font: 75 14pt \"MS Shell Dlg 2\"; }");
        ui->errors->setText(QString::fromStdString(""));
    }
    else {
        parseData data = parseTokens(TokensSTR);
        if (!data.check) {
            ui->checkLBL->setText("Valid Input");
            ui->checkLBL->setStyleSheet("QLabel { color : green; font: 75 14pt \"MS Shell Dlg 2\"; }");
            ui->errors->setText(QString::fromStdString(""));
        }
        else {
            ui->checkLBL->setText("Wrong Input");
            ui->checkLBL->setStyleSheet("QLabel { color: rgb(170, 0, 0); font: 75 14pt \"MS Shell Dlg 2\"; }");
            string errors = "";
            for (auto i = data.errors.begin(); i != data.errors.end(); i++)
                errors += "Error in line num. " + to_string(*i + 1) + "\n";
            ui->errors->setText(QString::fromStdString(errors));
            ui->errors->setStyleSheet("QLabel { color: rgb(170, 0, 0); font: 60 14pt \"MS Shell Dlg 2\"; }");
        }
    }
}
void MainWindow::on_drawBTN_2_clicked()
{
    string TokensSTR = ui->TokensList2->toPlainText().toUtf8().constData();
    if (TokensSTR != "") {
        parseData data = parseTokens(TokensSTR);
        if (!data.check) {
            ui->checkLBL->setText("Valid Input");
            ui->checkLBL->setStyleSheet("QLabel { color : green; font: 75 14pt \"MS Shell Dlg 2\"; }");
            ui->errors->setText(QString::fromStdString(""));
            createFile("graph.gv");
            writeFile(data.data, "graph.gv");
            system("Graphviz\\bin\\dot -Tsvg graph.gv -o file.svg");
            if (autoSave) {
                system("mkdir auto_save");
                string cmd = "auto_save\\graph" + to_string(cnt) + ".svg";
                system(("Graphviz\\bin\\dot -Tpng graph.gv -o " + cmd).c_str());
                cnt++;
            }
            QImage img("file.svg");
            bool valid = img.load(QString::fromStdString("file.svg"));
            if (valid) {
                QGraphicsScene* scene = new QGraphicsScene();
                ui->ParseTree->setScene(scene);
                QGraphicsSvgItem* item = new QGraphicsSvgItem("file.svg");
                scene->addItem(item);
                slider = image = true;
                ui->tabWidget->setCurrentIndex(2);
            }
        }
        else {
            ui->checkLBL->setText("Wrong Input");
            ui->checkLBL->setStyleSheet("QLabel { color: rgb(170, 0, 0); font: 75 14pt \"MS Shell Dlg 2\"; }");
            string errorsStr = "";
            for (auto i = data.errors.begin(); i != data.errors.end(); i++)
                errorsStr += "Error in line num. " + to_string(*i + 1) + "\n";
            ui->errors->setText(QString::fromStdString(errorsStr));
            ui->errors->setStyleSheet("QLabel { color: rgb(170, 0, 0); font: 60 14pt \"MS Shell Dlg 2\"; }");
        }
    }
    else {
        ui->checkLBL->setText("Please Enter Tokens List");
        ui->checkLBL->setStyleSheet("QLabel { font: 75 14pt \"MS Shell Dlg 2\"; }");
        ui->errors->setText(QString::fromStdString(""));
    }
}
void MainWindow::on_Zoom_valueChanged(int value)
{
    if (slider) {
        QImage img("file.svg");
        bool valid = img.load(QString::fromStdString("file.svg"));
        if (valid) {
            QGraphicsScene* scene = new QGraphicsScene();
            ui->ParseTree->setScene(scene);
            QGraphicsSvgItem* item = new QGraphicsSvgItem("file.svg");
            const double exp = value * 0.01;
            const double scl = pow(10.0, exp);
            item->setTransform(QTransform().scale(scl, scl));
            scene->addItem(item);
        }
    }
}
void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 0 && index == 1)
        slider = false;
}
void MainWindow::on_saveBTN_clicked()
{
    if (image) {
        QString path = QFileDialog::getSaveFileName(
            this, tr("Save Graph Image"), "Untitled.png", tr("Graph Image (*.png);;All Files (*)")
        );
        QPixmap img("file.svg");
        if (!img.isNull() && !path.isEmpty())
            img.save(path);
    }
}
void MainWindow::on_AutoSave_stateChanged()
{
    autoSave = !autoSave;
}
void MainWindow::on_importCodeBTN_clicked()
{
    // Creating File Dialog
    QString fileName = QFileDialog::getOpenFileName(
                this,
                "Import Tiny Code File",
                ".",
                "Text files (*.txt);; All files (*.*)");
    ui->tinyLBL->setText(getName(fileName));
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    QString code = stream.readAll();
    ui->TinyCode->setPlainText(code);
}

void MainWindow::on_TokensList2_textChanged()
{
    int count = 0;
    string text = ui->TokensList2->toPlainText().toUtf8().constData(), numbers = "";
    for (unsigned int i = 0; i < text.length(); i++){
        if (text[i] == '\n')
            count++;
    }
    for (int i = 1; i < count+2; i++){
        numbers += to_string(i) + '\n';
    }
    ui->Numbers->setPlainText(QString::fromStdString(numbers));
    connect(ui->TokensList2->verticalScrollBar(), SIGNAL(sliderMoved(int)), ui->Numbers->verticalScrollBar(), SLOT(setValue(int)));
}

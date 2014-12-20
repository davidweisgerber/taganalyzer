#include <QFile>
#include <QMessageBox>
#include "xmlreader.h"
#include "mainwindow.h"
#include "rawreader.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    XMLReader reader;
    reader.read();

    static const int lineLength = 16;

    QString formatted;
    for (int i=lineLength; i < reader.getResult().size(); i+=lineLength)
    {
        formatted += reader.getResult().mid(i-lineLength, lineLength) + "\n";
    }


    RawReader rawReader(reader.getResult());
    rawReader.calculate();
    ui->textEdit->setPlainText(rawReader.getResult());

    QString csvFile = reader.getFilename() + ".csv";
    QFile outFile(csvFile);
    if (outFile.open(QIODevice::WriteOnly) == false)
    {
        QMessageBox::critical(this, tr("Could not open outfile"), tr("%1 could not be opened for write").arg(csvFile));
        return;
    }

    outFile.write(rawReader.getResultCSV().toUtf8());
    outFile.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

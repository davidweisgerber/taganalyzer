#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "linechart.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QCoreApplication::setApplicationName("TagAnalyzer");
    QCoreApplication::setOrganizationName("David Weisgerber");

    QSettings settings;
    ui->startTimeEdit->setDateTime(settings.value("startDate", QDateTime::currentDateTime()).toDateTime());

    QString xmlFile = settings.value("xmlFile").toString();
    if (xmlFile.isEmpty() == false)
    {
        m_xmlReader.read(xmlFile);
        m_rawReader.calculate(m_xmlReader.getResult(), ui->startTimeEdit->dateTime());
        ui->textEdit->setPlainText(m_rawReader.getResult());
    }

    QString abbottFile = settings.value("abbottFile").toString();
    if (abbottFile.isEmpty() == false)
    {
        m_abbottReader.read(abbottFile);
        ui->freestyleLibreLineEdit->setText(abbottFile);
    }

    connect(ui->exportCSVButton, &QAbstractButton::clicked, this, &MainWindow::exportCSV);
    connect(ui->readXMLFileButton, &QAbstractButton::clicked, this, &MainWindow::selectXMLFile);
    connect(ui->freestyleLibreFileButton, &QAbstractButton::clicked, this, &MainWindow::selectFreestyLibreFile);
    connect(ui->startTimeEdit, &QDateTimeEdit::dateTimeChanged, this, &MainWindow::startDateChanged);
    connect(ui->saveGraphButton, &QAbstractButton::clicked, this, &MainWindow::saveGraph);

    m_graphicsView = new QGraphicsView();
    m_graphicsView->setGeometry(0, 0, 1024, 1024);
    m_graphicsView->setScene(new QGraphicsScene());
    m_lineChart = new LineChart(m_graphicsView->geometry());
    m_graphicsView->scene()->addItem(m_lineChart);
    m_graphicsView->show();
    updateGraphics();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectXMLFile()
{
    QString filename = QFileDialog::getOpenFileName(0, "Select file", QString(), "XML Files *.xml");
    if (filename.isEmpty())
    {
        return;
    }

    QSettings settings;
    settings.setValue("xmlFile", filename);

    m_xmlReader.read(filename);
    m_rawReader.calculate(m_xmlReader.getResult(), ui->startTimeEdit->dateTime());
    ui->textEdit->setPlainText(m_rawReader.getResult());
    updateGraphics();
}

void MainWindow::selectFreestyLibreFile()
{
    QString filename = QFileDialog::getOpenFileName(0, "Select file", QString(), "File from Abbott Software *.*");
    if (filename.isEmpty())
    {
        return;
    }

    QSettings settings;
    settings.setValue("abbottFile", filename);
    m_abbottReader.read(filename);
    ui->freestyleLibreLineEdit->setText(filename);
    updateGraphics();
}

void MainWindow::exportCSV()
{
    QString csvFile = m_xmlReader.getFilename() + ".csv";
    QFile outFile(csvFile);
    if (outFile.open(QIODevice::WriteOnly) == false)
    {
        QMessageBox::critical(this, tr("Could not open outfile"), tr("%1 could not be opened for write").arg(csvFile));
        return;
    }

    outFile.write(m_rawReader.getResultCSV().toUtf8());
    outFile.close();
}

void MainWindow::startDateChanged(const QDateTime &newDate)
{
    QSettings settings;
    settings.setValue("startDate", newDate);
    updateGraphics();
}

void MainWindow::updateGraphics()
{
    QMap<QDateTime, int> reference = m_abbottReader.getValues();
    QMap<QDateTime, int> measurement1 = m_rawReader.getResultCorrected(0);
    QMap<QDateTime, int> measurement2 = m_rawReader.getResultCorrected(1);

    m_lineChart->setReference(reference);
    m_lineChart->clear();
    m_lineChart->addValue(measurement1);
    m_lineChart->addValue(measurement2);
    m_lineChart->setResult(m_rawReader.getResultCorrected(2));
}

void MainWindow::saveGraph()
{
    QString filename = QFileDialog::getSaveFileName(0, "Select location to save PNG", QString(), "PNG Image *.png");
    if (filename.isEmpty())
    {
        return;
    }

    QImage img(m_graphicsView->height(), m_graphicsView->width(), QImage::Format_ARGB32_Premultiplied);
    QPainter p(&img);
    m_graphicsView->scene()->render(&p);
    p.end();
    img.save(filename);
}

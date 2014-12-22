#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "abbottsoftwarereader.h"
#include "rawreader.h"
#include "xmlreader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void selectXMLFile();
    void selectFreestyLibreFile();
    void exportCSV();
    void startDateChanged(const QDateTime &newDate);
    void updateGraphics();

private:
    Ui::MainWindow *ui;
    XMLReader m_xmlReader;
    RawReader m_rawReader;
    AbbottSoftwareReader m_abbottReader;
};

#endif // MAINWINDOW_H

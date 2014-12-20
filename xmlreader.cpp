#include <QtDebug>
#include <QXmlQuery>
#include <QUrl>
#include <QFileDialog>
#include "xmlreader.h"

XMLReader::XMLReader()
{
}

void XMLReader::read()
{
    m_filename = QFileDialog::getOpenFileName(0, "Select file", QString(), "XML Files *.xml");
    if (m_filename.isEmpty())
    {
        return;
    }

    QXmlQuery query;
    query.setFocus(QUrl::fromLocalFile(m_filename));
    query.setQuery("//Block/text()");

    if (query.isValid() == false)
    {
        qDebug() << "Invalid query";
    }

    query.evaluateTo(&m_result);
}

const QString &XMLReader::getResult()
{
    return m_result;
}

const QString &XMLReader::getFilename()
{
    return m_filename;
}

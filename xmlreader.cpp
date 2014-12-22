#include <QtDebug>
#include <QXmlQuery>
#include <QUrl>
#include <QFileDialog>
#include "xmlreader.h"

XMLReader::XMLReader()
{
}

void XMLReader::read(const QString &filename)
{
    m_filename = filename;

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

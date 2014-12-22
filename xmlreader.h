#ifndef XMLREADER_H
#define XMLREADER_H

#include <QString>

class XMLReader
{
public:
    XMLReader();

    void read(const QString &filename);
    const QString &getResult();
    const QString &getFilename();

private:
    QString m_result;
    QString m_filename;
};

#endif // XMLREADER_H

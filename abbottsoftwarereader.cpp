#include <QFile>
#include <QStringList>
#include "abbottsoftwarereader.h"

AbbottSoftwareReader::AbbottSoftwareReader()
{

}

AbbottSoftwareReader::~AbbottSoftwareReader()
{

}

bool AbbottSoftwareReader::read(const QString &filename)
{
    m_values.clear();
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly) == false)
    {
        return false;
    }

    file.readLine();
    file.readLine();
    file.readLine();

    QString line = file.readLine();
    while (line.isEmpty() == false)
    {
        QStringList parts = line.split("\t");
        if (parts.size() < 4)
        {
            line = file.readLine();
            continue;
        }

        QDateTime date = QDateTime::fromString(parts[1], "yyyy.MM.dd HH:mm");

        if (parts[2] != "0" && parts[2] != "1")
        {
            line = file.readLine();
            continue;
        }

        int result = 0;
        if (parts[3].isEmpty())
        {
            result = parts[4].toInt();
        }
        else
        {
            result = parts[3].toInt();
        }

        m_values.insert(date, result);
        line = file.readLine();
    }

    return true;
}

const QMap<QDateTime, int> &AbbottSoftwareReader::getValues()
{
    return m_values;
}


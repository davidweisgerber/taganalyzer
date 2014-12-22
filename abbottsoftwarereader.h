#ifndef ABBOTTSOFTWAREREADER_H
#define ABBOTTSOFTWAREREADER_H

#include <QString>
#include <QMap>
#include <QDateTime>

class AbbottSoftwareReader
{
public:
    AbbottSoftwareReader();
    ~AbbottSoftwareReader();

    bool read(const QString &filename);
    const QMap<QDateTime, int> &getValues();

private:
    QMap<QDateTime, int> m_values;
};

#endif // ABBOTTSOFTWAREREADER_H

#ifndef RAWREADER_H
#define RAWREADER_H

#include <QString>
#include <QMap>

class RawReader
{
public:
    RawReader(const QString &data);
    void calculate();
    const QString &getResult() const;
    const QString &getResultCSV() const;

private:
    class Record {
    public:
        int entry1;
        int entry2;
    };

    void calculateTimeRunning();
    void calculateNextWriteBlocks();
    void calculateCurrentValues();
    void calculateHistoricValues();
    Record recordToLong(const QString &record);

    QString m_data;
    int m_numberOfMinutesRunning;
    int m_nextWriteBlock1;
    int m_nextWriteBlock2;

    QString m_result;
    QString m_resultCSV;
    QMap<int, Record> m_resultMap;
};

#endif // RAWREADER_H

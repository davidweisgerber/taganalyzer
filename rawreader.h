#ifndef RAWREADER_H
#define RAWREADER_H

#include <QString>
#include <QMap>
#include <QDateTime>

class RawReader
{
public:
    RawReader();
    void calculate(const QString &data, const QDateTime &startDate);
    const QString &getResult() const;
    const QString &getResultCSV() const;
    QMap<QDateTime, int> getResultCorrected(int type) const;

private:
    class Record {
    public:
        int entry1;
        int entry2;
        int entry3;
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
    QDateTime m_startDate;
};

#endif // RAWREADER_H

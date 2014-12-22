#include "rawreader.h"

RawReader::RawReader() : m_data()
  , m_numberOfMinutesRunning(0)
  , m_nextWriteBlock1(0)
  , m_nextWriteBlock2(0)
{
}


void RawReader::calculate(const QString &data)
{
    m_data = data;
    calculateTimeRunning();
    calculateNextWriteBlocks();
    calculateCurrentValues();
    calculateHistoricValues();

    m_resultCSV = "sep=,\n";
    foreach (int time, m_resultMap.keys())
    {
        m_resultCSV += QString("%1,%2,%3\n").arg(time).arg(m_resultMap[time].entry1).arg(m_resultMap[time].entry2);
    }
}

const QString &RawReader::getResult() const
{
    return m_result;
}

const QString &RawReader::getResultCSV() const
{
    return m_resultCSV;
}

QMap<QDateTime, int> RawReader::getResultCorrected(int type, const QDateTime &startDate) const
{
    QDateTime nowTime = startDate.addSecs(m_numberOfMinutesRunning * 60);

    QMap<QDateTime, int> retValue;
    foreach (int minutesInPast, m_resultMap.keys())
    {
        int value = 0;
        Record record = m_resultMap[minutesInPast];
        if (type == 0)
        {
            value = record.entry1;
        }
        else if (type == 1)
        {
            value = record.entry2;
        }

        retValue.insert(nowTime.addSecs(minutesInPast * -60), value);
    }

    return retValue;
}

void RawReader::calculateTimeRunning()
{
    QString hexData = m_data.mid(317 * 2, 2) + m_data.mid(316 * 2, 2);
    m_numberOfMinutesRunning = hexData.toInt(0, 16);
    m_result += QString("Running for %1 days, %2 hours, %3 minutes\n").
            arg(m_numberOfMinutesRunning / 60 / 24).
            arg((m_numberOfMinutesRunning / 60) % 24).
            arg(m_numberOfMinutesRunning % 60);
}

void RawReader::calculateNextWriteBlocks()
{
    QString hexDataNextWriteBlock1 = m_data.mid(26 * 2, 2);
    QString hexDataNextWriteBlock2 = m_data.mid(27 * 2, 2);
    m_nextWriteBlock1 = hexDataNextWriteBlock1.toInt(0, 16);
    m_nextWriteBlock2 = hexDataNextWriteBlock2.toInt(0, 16);
}

void RawReader::calculateCurrentValues()
{
    int lineNumber = 0;
    for (int i=56; i < (16 * 6 * 2) + 56; i+=12)
    {
        QString record = m_data.mid(i, 12);
        int time = 0;
        Record value = recordToLong(record);

        for (int j=0; j < 6; j++)
        {
            m_result.append(record.mid(j * 2, 2) + " ");
        }

        if (m_nextWriteBlock1 - lineNumber != 0)
        {
            time = m_nextWriteBlock1 - lineNumber;
            m_result += QString(" %1 min").arg(time);
        }
        else
        {
            m_result += QString(" NOW");
            m_nextWriteBlock1 += 16;
        }

        m_result += QString("\t\t%1 %2\n").arg(value.entry1).arg(value.entry2);

        if (time != 0)
        {
            m_resultMap.insert(time, value);
        }

        lineNumber++;
    }
}

void RawReader::calculateHistoricValues()
{
    int lineNumber = 0;
    for (int i=56 + (16 * 6 * 2); i < (48 * 6 * 2) + 56; i+=12)
    {
        QString record = m_data.mid(i, 12);
        int time = 0;
        Record value = recordToLong(record);

        for (int j=0; j < 6; j++)
        {
            m_result.append(record.mid(j * 2, 2) + " ");
        }

        if (m_nextWriteBlock2 - lineNumber != 0)
        {
            time = (m_nextWriteBlock2 - lineNumber) * 15;
            m_result += QString(" %1 min").arg(time);
        }
        else
        {
            m_result += QString(" LAST");
            m_nextWriteBlock2 += 32;
        }

        m_result += QString("\t\t%1 %2\n").arg(value.entry1).arg(value.entry2);

        if (time != 0)
        {
            m_resultMap.insert(time, value);
        }

        lineNumber++;
    }
}

RawReader::Record RawReader::recordToLong(const QString &record)
{
    RawReader::Record recordData;
    recordData.entry1 = record.mid(10, 2).toLongLong(0, 16) << 16;
    recordData.entry1 += record.mid(8, 2).toLongLong(0, 16) << 8;
    recordData.entry1 += record.mid(6, 2).toLongLong(0, 16) << 0;
    recordData.entry2 = record.mid(4, 2).toLongLong(0, 16) << 16;
    recordData.entry2 += record.mid(2, 2).toLongLong(0, 16) << 8;
    recordData.entry2 += record.mid(0, 2).toLongLong(0, 16);

    return recordData;
}

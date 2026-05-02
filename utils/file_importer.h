#ifndef FILE_IMPORTER_H
#define FILE_IMPORTER_H

#include <QString>
#include <QVector>
#include <QList>
#include "../model/hospital.h"

class FileImporter {
public:
    static QVector<Hospital> importCSV(const QString& filePath);
    static QVector<Hospital> importJSON(const QString& filePath);
    static bool exportCSV(const QString& filePath, const QVector<Hospital>& hospitals);

private:
    static QList<QString> parseCSVLine(const QString& line);
};

#endif

#include "file_importer.h"
#include <QFile>
#include <QTextStream>
#include <QByteArray>

QVector<Hospital> FileImporter::importCSV(const QString& filePath) {
    QVector<Hospital> hospitals;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return hospitals;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    int id = 1;
    bool headerSkipped = false;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        if (!headerSkipped) {
            if (line.contains("name", Qt::CaseInsensitive) ||
                line.contains("id", Qt::CaseInsensitive) ||
                line.contains("x", Qt::CaseInsensitive)) {
                headerSkipped = true;
                continue;
            }
            headerSkipped = true;
        }

        QList<QString> parts = line.split(',');
        if (parts.size() >= 3) {
            QString name = parts[0].trimmed();
            double x = parts[1].trimmed().toDouble();
            double y = parts[2].trimmed().toDouble();
            hospitals.append(Hospital(id++, name, x, y));
        }
    }

    file.close();
    return hospitals;
}

QVector<Hospital> FileImporter::importJSON(const QString& filePath) {
    return QVector<Hospital>();
}

bool FileImporter::exportCSV(const QString& filePath, const QVector<Hospital>& hospitals) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << "name,x,y\n";
    for (const Hospital& h : hospitals) {
        out << h.name << "," << h.position.x() << "," << h.position.y() << "\n";
    }

    file.close();
    return true;
}

QList<QString> FileImporter::parseCSVLine(const QString& line) {
    return line.split(',');
}

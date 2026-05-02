#ifndef HOSPITAL_INPUT_WIDGET_H
#define HOSPITAL_INPUT_WIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVector>
#include "../model/hospital.h"

class HospitalInputWidget : public QWidget {
    Q_OBJECT

public:
    HospitalInputWidget(QWidget* parent = nullptr);

    QVector<Hospital> getHospitals() const;
    void setHospitals(const QVector<Hospital>& hospitals);
    void clear();
    void addHospital();
    void removeHospital();
    void importCSV();
    void exportCSV();

signals:
    void hospitalsChanged(const QVector<Hospital>& hospitals);

private slots:
    void cellChanged(int row, int column);

private:
    QTableWidget* m_table;
    QPushButton* m_addBtn;
    QPushButton* m_removeBtn;
    QPushButton* m_importBtn;
    QPushButton* m_exportBtn;
    int m_nextId;

    void setupUI();
};

#endif

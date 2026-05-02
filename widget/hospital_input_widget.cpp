#include "hospital_input_widget.h"
#include "../utils/file_importer.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>

HospitalInputWidget::HospitalInputWidget(QWidget* parent)
    : QWidget(parent), m_nextId(1) {
    setupUI();
}

void HospitalInputWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* title = new QLabel("Hospital List", this);
    title->setStyleSheet("font-weight: bold; font-size: 14px;");
    mainLayout->addWidget(title);

    m_table = new QTableWidget(0, 3, this);
    m_table->setHorizontalHeaderLabels({"Name", "X", "Y"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->setSelectionBehavior(QTableWidget::SelectRows);
    connect(m_table, &QTableWidget::cellChanged, this, &HospitalInputWidget::cellChanged);
    mainLayout->addWidget(m_table);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    m_addBtn = new QPushButton("Add", this);
    m_removeBtn = new QPushButton("Remove", this);
    m_importBtn = new QPushButton("Import CSV", this);
    m_exportBtn = new QPushButton("Export CSV", this);

    connect(m_addBtn, &QPushButton::clicked, this, &HospitalInputWidget::addHospital);
    connect(m_removeBtn, &QPushButton::clicked, this, &HospitalInputWidget::removeHospital);
    connect(m_importBtn, &QPushButton::clicked, this, &HospitalInputWidget::importCSV);
    connect(m_exportBtn, &QPushButton::clicked, this, &HospitalInputWidget::exportCSV);

    btnLayout->addWidget(m_addBtn);
    btnLayout->addWidget(m_removeBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(m_importBtn);
    btnLayout->addWidget(m_exportBtn);
    mainLayout->addLayout(btnLayout);
}

void HospitalInputWidget::addHospital() {
    int row = m_table->rowCount();
    m_table->insertRow(row);
    m_table->setItem(row, 0, new QTableWidgetItem(QString("Hospital_%1").arg(m_nextId)));
    m_table->setItem(row, 1, new QTableWidgetItem(QString::number(0)));
    m_table->setItem(row, 2, new QTableWidgetItem(QString::number(0)));
    m_nextId++;
    emit hospitalsChanged(getHospitals());
}

void HospitalInputWidget::removeHospital() {
    int row = m_table->currentRow();
    if (row >= 0) {
        m_table->removeRow(row);
        emit hospitalsChanged(getHospitals());
    }
}

void HospitalInputWidget::importCSV() {
    QString filePath = QFileDialog::getOpenFileName(this, "Import CSV", "", "CSV Files (*.csv)");
    if (filePath.isEmpty()) return;

    QVector<Hospital> hospitals = FileImporter::importCSV(filePath);
    if (hospitals.isEmpty()) {
        QMessageBox::warning(this, "Import Error", "Failed to import or empty file.");
        return;
    }

    setHospitals(hospitals);
    emit hospitalsChanged(hospitals);
}

void HospitalInputWidget::exportCSV() {
    QString filePath = QFileDialog::getSaveFileName(this, "Export CSV", "", "CSV Files (*.csv)");
    if (filePath.isEmpty()) return;

    QVector<Hospital> hospitals = getHospitals();
    if (!FileImporter::exportCSV(filePath, hospitals)) {
        QMessageBox::warning(this, "Export Error", "Failed to export.");
    }
}

void HospitalInputWidget::cellChanged(int row, int column) {
    emit hospitalsChanged(getHospitals());
}

QVector<Hospital> HospitalInputWidget::getHospitals() const {
    QVector<Hospital> hospitals;
    for (int i = 0; i < m_table->rowCount(); ++i) {
        QTableWidgetItem* nameItem = m_table->item(i, 0);
        QTableWidgetItem* xItem = m_table->item(i, 1);
        QTableWidgetItem* yItem = m_table->item(i, 2);

        QString name = nameItem ? nameItem->text() : QString("Hospital_%1").arg(i + 1);
        double x = xItem ? xItem->text().toDouble() : 0;
        double y = yItem ? yItem->text().toDouble() : 0;

        hospitals.append(Hospital(i + 1, name, x, y));
    }
    return hospitals;
}

void HospitalInputWidget::setHospitals(const QVector<Hospital>& hospitals) {
    m_table->setRowCount(0);
    for (const Hospital& h : hospitals) {
        int row = m_table->rowCount();
        m_table->insertRow(row);
        m_table->setItem(row, 0, new QTableWidgetItem(h.name));
        m_table->setItem(row, 1, new QTableWidgetItem(QString::number(h.position.x())));
        m_table->setItem(row, 2, new QTableWidgetItem(QString::number(h.position.y())));
    }
    m_nextId = hospitals.size() + 1;
}

void HospitalInputWidget::clear() {
    m_table->setRowCount(0);
    m_nextId = 1;
}

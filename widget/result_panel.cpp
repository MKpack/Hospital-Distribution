#include "result_panel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>

ResultPanel::ResultPanel(QWidget* parent)
    : QWidget(parent) {
    setupUI();
}

void ResultPanel::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* title = new QLabel("Evaluation Result", this);
    title->setStyleSheet("font-weight: bold; font-size: 14px;");
    mainLayout->addWidget(title);

    m_overallLabel = new QLabel("Overall: -", this);
    m_coverageLabel = new QLabel("Coverage: -", this);
    m_maxDistLabel = new QLabel("Max Distance: -", this);
    m_avgDistLabel = new QLabel("Avg Distance: -", this);

    QVBoxLayout* summaryLayout = new QVBoxLayout();
    summaryLayout->addWidget(m_overallLabel);
    summaryLayout->addWidget(m_coverageLabel);
    summaryLayout->addWidget(m_maxDistLabel);
    summaryLayout->addWidget(m_avgDistLabel);
    mainLayout->addLayout(summaryLayout);

    m_table = new QTableWidget(0, 5, this);
    m_table->setHorizontalHeaderLabels({"Hospital", "Max Dist (m)", "Avg Dist (m)", "Status", "Reasonable?"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->setSelectionBehavior(QTableWidget::SelectRows);
    m_table->setEditTriggers(QTableWidget::NoEditTriggers);
    mainLayout->addWidget(m_table);
}

void ResultPanel::setEvaluation(const CityEvaluationResult& eval) {
    m_overallLabel->setText(QString("Overall: %1")
        .arg(eval.isOverallReasonable ? "REASONABLE" : "UNREASONABLE"));
    m_overallLabel->setStyleSheet(eval.isOverallReasonable ? "color: green; font-weight: bold;" : "color: red; font-weight: bold;");

    m_coverageLabel->setText(QString("Coverage: %1% (%2/%3 hospitals)").arg(
        QString::number(eval.overallCoverage, 'f', 1))
        .arg(eval.reasonableCount).arg(eval.totalHospitals));

    m_maxDistLabel->setText(QString("Global Max Distance: %1 m").arg(
        QString::number(eval.globalMaxDistance, 'f', 1)));

    m_avgDistLabel->setText(QString("Global Avg Distance: %1 m").arg(
        QString::number(eval.globalAvgDistance, 'f', 1)));

    m_table->setRowCount(eval.hospitalEvaluations.size());
    for (int i = 0; i < eval.hospitalEvaluations.size(); ++i) {
        const HospitalEvaluation& h = eval.hospitalEvaluations[i];
        m_table->setItem(i, 0, new QTableWidgetItem(h.hospitalName));
        m_table->setItem(i, 1, new QTableWidgetItem(QString::number(h.maxDistance, 'f', 1)));
        m_table->setItem(i, 2, new QTableWidgetItem(QString::number(h.avgDistance, 'f', 1)));
        m_table->setItem(i, 3, new QTableWidgetItem(h.isReasonable ? "OK" : "WARNING"));
        m_table->setItem(i, 4, new QTableWidgetItem(h.isReasonable ? "Yes" : "No"));

        m_table->item(i, 3)->setTextAlignment(Qt::AlignCenter);
        m_table->item(i, 4)->setTextAlignment(Qt::AlignCenter);
        if (h.isReasonable) {
            m_table->item(i, 3)->setForeground(Qt::green);
            m_table->item(i, 4)->setForeground(Qt::green);
        } else {
            m_table->item(i, 3)->setForeground(Qt::red);
            m_table->item(i, 4)->setForeground(Qt::red);
        }
    }
}

void ResultPanel::clear() {
    m_overallLabel->setText("Overall: -");
    m_coverageLabel->setText("Coverage: -");
    m_maxDistLabel->setText("Max Distance: -");
    m_avgDistLabel->setText("Avg Distance: -");
    m_table->setRowCount(0);
}

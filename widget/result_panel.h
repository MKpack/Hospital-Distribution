#ifndef RESULT_PANEL_H
#define RESULT_PANEL_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include "../model/evaluation_result.h"

class ResultPanel : public QWidget {
    Q_OBJECT

public:
    ResultPanel(QWidget* parent = nullptr);

    void setEvaluation(const CityEvaluationResult& eval);
    void clear();

private:
    QTableWidget* m_table;
    QLabel* m_overallLabel;
    QLabel* m_coverageLabel;
    QLabel* m_maxDistLabel;
    QLabel* m_avgDistLabel;

    void setupUI();
};

#endif

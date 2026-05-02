#ifndef EVALUATIONRESULT_H
#define EVALUATIONRESULT_H

#include <QString>
#include <QVector>

struct HospitalEvaluation {
    int hospitalId;
    QString hospitalName;
    double maxDistance;
    double avgDistance;
    bool isReasonable;

    HospitalEvaluation()
        : hospitalId(0), maxDistance(0), avgDistance(0), isReasonable(true) {}
};

struct CityEvaluationResult {
    QVector<HospitalEvaluation> hospitalEvaluations;
    double overallCoverage;
    bool isOverallReasonable;
    int totalHospitals;
    int reasonableCount;
    int unreasonableCount;
    double globalMaxDistance;
    double globalAvgDistance;

    CityEvaluationResult()
        : overallCoverage(0), isOverallReasonable(true),
          totalHospitals(0), reasonableCount(0), unreasonableCount(0),
          globalMaxDistance(0), globalAvgDistance(0) {}
};

#endif

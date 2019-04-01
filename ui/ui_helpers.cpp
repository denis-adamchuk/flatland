#include "ui_helpers.h"

//std::vector<QString> GetLegend(const flatland::lib::IFlatland& flatland)
//{
//    const auto& stat = flatland.GetStatistics();

//    std::vector<QString> v;
//    v.push_back(QString("generation: %1").arg(stat._generation));
//    v.push_back(QString("alive cells: %1 (%2%3)").
//        arg(QString::number(stat._aliveCells),
//            (stat._aliveCellsDelta > 0 ? "+" : "-"), QString::number(stat._aliveCellsDelta)));
//    v.push_back(QString("loop: %1").arg((stat._loopDetected ? "Yes" : "No")));
//    return v;
//}

//std::vector<QString> getLegend(const flatland::lib::AdvancedFlatland& flatland)
//{
//    const auto& stat = flatland.GetStatistics();

//    std::vector<QString> v;
//    v.push_back(QString("generation: %1").arg(stat._generation));
//    v.push_back(QString("alive cells: %1 (%2%3)").
//        arg(QString::number(stat._aliveCells),
//            (stat._aliveCellsDelta > 0 ? "+" : "-"), QString::number(stat._aliveCellsDelta)));
//    v.push_back(QString("reproductive cells: %1 (%2%3)").
//        arg(QString::number(stat._reproductiveCells),
//            (stat._reproductiveCellsDelta > 0 ? "+" : "-"), QString::number(stat._reproductiveCellsDelta)));
//    return v;
//}

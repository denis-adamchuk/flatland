#include "ui_helpers.h"

QColor getColorOfCell(const flatland::lib::SimpleFlatland& /*flatland*/, size_t /*i*/, size_t /*j*/)
{
    return Qt::green;
}

QColor getColorOfCell(const flatland::lib::AdvancedFlatland& flatland, size_t i, size_t j)
{
    const auto maxAge = flatland.GetMaxAge();
    const auto age = flatland.GetCellAge(i, j);

    int h = 0;
    int s = 0;
    int v = 0;

         if (age > maxAge * 0.0 && age < maxAge * 0.2)
        h = 120; // G
    else if (age > maxAge * 0.2 && age < maxAge * 0.4)
        h = 30;  // Orange
    else if (age > maxAge * 0.4 && age < maxAge * 0.6)
        h = 0;   // R
    else if (age > maxAge * 0.6 && age < maxAge * 0.8)
        h = 300; // Magenta
    else if (age > maxAge * 0.8 && age < maxAge * 1.0)
        h = 240; // B
    const auto ageTo255 = static_cast<int>(255. * age / maxAge);
    static const auto lowestV = 130;
    s = v = std::max(lowestV, 255 - ageTo255);

    return QColor::fromHsv(h, s, v);
}

std::vector<QString> getLegend(const flatland::lib::SimpleFlatland& flatland)
{
    const auto& stat = flatland.GetStatistics();

    std::vector<QString> v;
    v.push_back(QString("generation: %1").arg(stat._generation));
    v.push_back(QString("alive cells: %1 (%2%3)").
        arg(QString::number(stat._aliveCells),
            (stat._aliveCellsDelta > 0 ? "+" : "-"), QString::number(stat._aliveCellsDelta)));
    v.push_back(QString("loop: %1").arg((stat._loopDetected ? "Yes" : "No")));
    return v;
}

std::vector<QString> getLegend(const flatland::lib::AdvancedFlatland& flatland)
{
    const auto& stat = flatland.GetStatistics();

    std::vector<QString> v;
    v.push_back(QString("generation: %1").arg(stat._generation));
    v.push_back(QString("alive cells: %1 (%2%3)").
        arg(QString::number(stat._aliveCells),
            (stat._aliveCellsDelta > 0 ? "+" : "-"), QString::number(stat._aliveCellsDelta)));
    v.push_back(QString("reproductive cells: %1 (%2%3)").
        arg(QString::number(stat._reproductiveCells),
            (stat._reproductiveCellsDelta > 0 ? "+" : "-"), QString::number(stat._reproductiveCellsDelta)));
    return v;
}

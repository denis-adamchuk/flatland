#pragma once

#include "lib/SimpleFlatland.h"
#include "lib/AdvancedFlatland.h"

#include <QString>
#include <QColor>
#include <QRgb>

#include <vector>

const QRgb sc_backgroundColor = qRgb(250, 250, 215);
const QRgb sc_externalAreaColor = qRgb(200, 200, 200);

QColor getColorOfCell(const flatland::lib::SimpleFlatland& /*flatland*/, size_t /*i*/, size_t /*j*/);
QColor getColorOfCell(const flatland::lib::AdvancedFlatland& flatland, size_t i, size_t j);

std::vector<QString> getLegend(const flatland::lib::SimpleFlatland& flatland);
std::vector<QString> getLegend(const flatland::lib::AdvancedFlatland& flatland);

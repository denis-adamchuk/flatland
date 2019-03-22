#pragma once

namespace flatland
{

namespace lib
{

struct Statistics
{
    unsigned long _aliveCells { 0 };
      signed long _aliveCellsDelta { 0 };
    unsigned long _generation { 0 };
             bool _loopDetected { false };
};

struct AdvancedStatistics : Statistics
{
    unsigned long _reproductiveCells { 0 };
      signed long _reproductiveCellsDelta { 0 };
};

}

}

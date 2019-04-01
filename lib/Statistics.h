#pragma once

#include <map>
#include <string>
#include <variant>

namespace flatland
{

namespace lib
{

using PropertyName = std::string;
class PropertyValue
{
public:
    template <typename T>
    const T& Get() const
    {
        return std::get<T>(m_value);
    }

    template <typename T>
    void Set(const T& value)
    {
        m_value = value;
    }

    std::string GetAsText() const
    {
        switch (m_value.index())
        {
        case 0: return std::to_string(std::get<0>(m_value));
        case 1: return std::to_string(std::get<1>(m_value));
        case 2: return std::get<2>(m_value) ? "Yes" : "No";
        }
        return {};
    }

private:
    std::variant<long, unsigned long, bool> m_value;
};

using StatisticsMap = std::map<PropertyName, PropertyValue>;

}

}

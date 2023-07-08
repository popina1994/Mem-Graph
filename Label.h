#pragma once
#include <string>
namespace MemGraph
{
    class Label
    {
        std::string m_labelName;
    public:
        Label(const std::string& labelName) : m_labelName(labelName) {};
        ~Label() {}
        const std::string& getLabelName(void) const
        {
            return m_labelName;
        }

        bool operator==(const Label& otherLabel) const
        {
            return m_labelName == otherLabel.m_labelName;
        }

        struct HashFunction
        {
            size_t operator()(const Label& label) const
            {
                return std::hash<std::string>()(label.m_labelName);
            }
        };
    };
}

template <>
struct std::hash<MemGraph::Label>
{
    std::size_t operator()(const MemGraph::Label& label) const
    {
        return std::hash<std::string>()(label.getLabelName());
    }
};

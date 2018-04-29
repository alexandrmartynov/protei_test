#ifndef PARSER_H
#define PARSER_H

#include <list>
#include <string>

class Parser
{
public:
    Parser() = default;
    void start(const std::string& message);
private:
    void display() const;
    void parse(std::string message);

    std::list<int> m_items;
};

#endif

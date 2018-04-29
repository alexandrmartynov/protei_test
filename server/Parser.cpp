#include "Parser.h"

#include <iostream>
#include <cstring>
#include <algorithm>

void Parser::start(const std::string& message)
{
    m_items.clear();

    parse(message);

    display();

    int sum_numbers = std::accumulate(m_items.begin(), m_items.end(), 0);

    std::cout << "sum of numbers: " << sum_numbers << std::endl;

    m_items.sort(std::greater<int>());

    display();

    auto minmax = std::minmax_element(m_items.begin(), m_items.end());
    std::cout << "min: " << *minmax.first << " max: " << *minmax.second << std::endl;
}

void Parser::parse(std::string message)
{
    std::string::iterator parser = message.begin();
    while(parser != message.end())
    {
        if((*parser >= '0') && (*parser <= '9'))
        {
            int number = *parser - '0';
            m_items.push_back(number);
        }

        parser++;
    }
}

void Parser::display() const
{
    for(int item: m_items)
    {
        std::cout << item << " ";    
    }
    std::cout << std::endl;
}

#include "Parser.h"

#include <iostream>
#include <cstring>
#include <algorithm>
#include <numeric>

void Parser::start(const std::string& message)
{
    m_items.clear();

    parse(message);

    std::cout << "Original numbers: ";
    display();

    m_items.sort(std::greater<int>());

    std::cout << "Sorted numbers: ";
    display();
    
    int sum_numbers = std::accumulate(m_items.begin(), m_items.end(), 0);
    std::cout << "sum of numbers: " << sum_numbers << std::endl;

    auto minmax = std::minmax_element(m_items.begin(), m_items.end());
    std::cout << "min: " << *minmax.first << " max: " << *minmax.second << std::endl;
}

void Parser::parse(const std::string& message)
{
    std::string::const_iterator parser = message.begin();
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

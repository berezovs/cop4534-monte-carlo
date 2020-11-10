#include "Simulation.hpp"
#include <vector>
#include <string>
#include <iostream>

int main()
{
    std::vector<std::string> files;
    files.push_back("t1.txt");
    files.push_back("t2.txt");
    files.push_back("t3.txt");
    files.push_back("t4.txt");
    Simulation *sim = new Simulation(files);
    sim->run();
    int counter = 0;
    
    delete sim;
    return 0;
}
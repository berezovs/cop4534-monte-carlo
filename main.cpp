/***************************************************************
  Student Name:Serghei Berezovschi
  File Name: main.cpp
  Assignment number:Project 4-DS&AII
 
***************************************************************/
#include "Simulation.hpp"
#include <vector>
#include <string>
#include <iostream>

int main()
{
    std::vector<std::string> files;
    //push filenames in vector
    files.push_back("t1.txt");
    files.push_back("t2.txt");
    files.push_back("t3.txt");
    files.push_back("t4.txt");

    Simulation *sim = new Simulation(files);

    //run simulation
    sim->run();

    
    delete sim;
    return 0;
}
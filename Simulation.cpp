#include "Simulation.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
Simulation::Simulation(std::vector<std::string> files)
{
    this->batchMetaInfo = files;
    int MIN = 0, MAX = 99;
   
    // Initialize a default_random_engine with the seed
}

void Simulation::run()
{
    std::uniform_int_distribution<int> dist = this->initializeRandomEngine(0, 99);
    for (std::string file : batchMetaInfo)
    {
        std::cout << file << std::endl;
        this->loadBatchConfigFile(file);
        this->generateDataSets(dist);
    }
}
void Simulation::loadBatchConfigFile(std::string fileName)
{
    this->in.open(fileName);

    if (this->in.is_open())
    {
        in >> this->numBatches;
        in >> this->numItemsInBatch;
        in >> this->badBatchesPercent;
        in >> this->badItemsPercent;
        in >> this->itemsSampled;
    }
    in.close();
}
void Simulation::generateDataSets(std::uniform_int_distribution<int> dist)
{
    
    int counter = 0;
    for (int i = 0; i < this->numBatches; i++)
    {
        std::ofstream out;
        std::string filename;

        filename = "ds";
        filename += std::to_string(i + 1);
        filename += ".txt";
        out.open(filename, std::ios_base::app);

        if (this->generateRandomNumberInRange(dist) < this->badBatchesPercent)
        {
            counter++;

            for (int k = 0; k < this->numItemsInBatch; k++)
            {
                out << this->getItem(this->generateRandomNumberInRange(dist), this->badItemsPercent) << "\n";
            }
        }
        else
        {
            for (int k = 0; k < this->numItemsInBatch; k++)
            {
                out << "g\n";
            }
        }

        out.close();
    }
    std::cout << "Bad Batches percent: " << this->badBatchesPercent << std::endl;
    std::cout << "Number of bad batches generated: " << counter << std::endl;
    counter = 0;
}
void Simulation::generateReport()
{
}
void Simulation::runDetectionAlgorithm()
{
}

int Simulation::generateRandomNumberInRange(std::uniform_int_distribution<int> myUnifIntDist)
{

    int number = myUnifIntDist(myRandomEngine);
    return number;
}

std::string Simulation::getItem(int number, int percent)
{
    if (number <= (percent - 1))
    {
        return "b";
    }
    return "g";
}

std::uniform_int_distribution<int> Simulation::initializeRandomEngine(int min, int max)
{
    std::default_random_engine myRandomDevice;
    seed = myRandomDevice();

    myRandomEngine.seed(seed);
    std::uniform_int_distribution<int> myUnifIntDist(min, max);
    return myUnifIntDist;
}
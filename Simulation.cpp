#include "Simulation.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
Simulation::Simulation(std::vector<std::string> files)
{
    this->batchMetaInfo = files;
}

void Simulation::run()
{
    std::uniform_int_distribution<int> dist = this->initializeRandomEngine(0, 99);
    for (std::string file : batchMetaInfo)
    {
        std::cout << file << std::endl;
        this->loadBatchConfigFile(file);
        this->generateDataSets(dist);
        this->runDetectionAlgorithm();
        this->calculateProbability();
        this->badBatchesDetected = 0;
        this->badBatchesGenerated = 0;
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

    std::uniform_int_distribution<int> random = this->initializeRandomEngine(0,1);

    for (int i = 0; i < this->numBatches; i++)
    {
        std::ofstream out, cleaner;
        std::string filename;

        filename = "ds";
        filename += std::to_string(i + 1);
        filename += ".txt";

        cleaner.open(filename, std::ofstream::out | std::ofstream::trunc);
        out.open(filename, std::ios_base::app);

        if (this->generateRandomNumberInRange(dist) < this->badBatchesPercent)
        {

            this->badBatchesGenerated++;
            int numBadItemsInBatchToWrite = (this->numItemsInBatch / 100) * this->badItemsPercent;
            for (int k = 0; k < this->numItemsInBatch; k++)
            {
                if (numBadItemsInBatchToWrite > 0 && generateRandomNumberInRange(random)!=0){
                    out << "b\n";
                    numBadItemsInBatchToWrite--;
                }
                else
                    out << "g\n";
            }
            //std::cout << "Bad Batches generated: " << filename << std::endl;
        }
        else
        {
            for (int k = 0; k < this->numItemsInBatch; k++)
            {
                out << "g\n";
            }
        }
        //std::cout << "Countined b.. "  << filename << std::endl;
        out.close();
    }
    std::cout << "Bad Batches percent: " << this->badBatchesPercent << std::endl;
    std::cout << "Number of bad batches generated: " << this->badBatchesGenerated << std::endl;
}
void Simulation::generateReport()
{
}
void Simulation::runDetectionAlgorithm()
{
    std::uniform_int_distribution<int> random = this->initializeRandomEngine(0,1);
    for (int i = 0; i < this->numBatches; i++)
    {
        int sampleSize = this->itemsSampled;
        std::string filename, item;
        std::ifstream in;
        filename = "ds";
        filename += std::to_string(i + 1);
        filename += ".txt";
        in.open(filename);
        if (in.is_open())

        {
            while (getline(in, item) && sampleSize > 0)
            {
                
                
                if (item == "b")
                {
                    this->badBatchesDetected++;
                    //std::cout << "Bad batch " << filename << "." << std::endl;
                    break;
                }
                sampleSize--;
                
            }
        }
    }
    std::cout << "Number of bad batches " << this->badBatchesDetected << "." << std::endl;
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
        std::cout << "b.. " << std::endl;
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

void Simulation::calculateProbability()
{
    std::string returnString;
    double base = 1 - (double)1 / (double)this->badBatchesPercent;
    double probabilityFailure = std::pow(base, this->itemsSampled);
    double percentDetected = ((double)(this->badBatchesDetected) / (double)(this->badBatchesGenerated)) * (double)100;

    std::cout << "Base = " << base;

    std::cout << " exponent = " << this->itemsSampled << std::endl;

    std::cout << "P(failure to detect bad batch) = " << probabilityFailure << " %" << std::endl;

    std::cout << "Percentage of bad batches actually detected =  " << percentDetected << " %" << std::endl;
}
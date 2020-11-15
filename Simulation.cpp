#include "Simulation.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
Simulation::Simulation(std::vector<std::string> files)
{
    this->batchMetaInfo = files;
}

void Simulation::run()
{
    int MIN = 0, MAX = 99;

    std::uniform_int_distribution<int> dist = this->initializeRandomEngine(MIN, MAX);
    for (std::string file : batchMetaInfo)
    {
        this->simNumber++;
        this->loadBatchConfigFile(file);
        this->printSimulationDetails();
        this->generateDataSets(dist);
        this->runDetectionAlgorithm();
        this->calculateAndShowAnalytics();
        //reset variables specific to individual simulations
        this->badBatchesDetected = 0;
        this->badBatchesGenerated = 0;
    }
}


//prints simulation details
void Simulation::printSimulationDetails()
{
    std::cout << "\nSimulation: " << this->simNumber << std::endl;
    std::cout << "\tNumber of batches of items: " << this->numBatches << std::endl;
    std::cout << "\tNumber of items in each batch : " << this->numItemsInBatch << std::endl;
    std::cout << "\tPercentage of batches containing bad items: " << this->badBatchesPercent << std::endl;
    std::cout << "\tPercentage of items that are bad in a bad set: " << this->badItemsPercent << std::endl;
    std::cout << "\tItems sampled from each set: " << this->itemsSampled << std::endl;
}

//loads .txt file containing simulation details
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

//generates data sets as specified by configuration file
void Simulation::generateDataSets(std::uniform_int_distribution<int> dist)
{

    //setup random engine
    std::random_device rd;
    std::mt19937 g(rd());

    int numBadBatches = ((double)this->badBatchesPercent / (double)100) * (double)this->numBatches;
    std::vector<std::string> items;
    bool flag = false;

    std::cout << "\nGenerating data sets:\n"
              << std::endl;
    for (int i = 0; i < this->numBatches; i++)
    {

        //push items to vector
        int numBadItems = 0;
        for (int j = 0; j < this->numItemsInBatch; j++)
        {
            if (this->generateRandomNumberInRange(dist) < this->badItemsPercent && numBadBatches > 0)
            {
                items.push_back("b");
                numBadItems++;
                if (!flag)
                    this->badBatchesGenerated++;
                flag = true;
            }
            else
            {
                items.push_back("g");
            }
        }

        numBadBatches--;
        //log bad batch
        if (flag)
            std::cout << "\tCreate bad set batch #" << i << " totalbad = " << numBadItems << " total = " << this->numBatches << " pct " << this->badItemsPercent << std::endl;
        //reset bad batch flag
        flag = false;
        //shuffle items in vector 
        std::shuffle(items.begin(), items.end(), g);
        //writes batch to file
        this->writeBatchToFile(items, i + 1);
        items.clear();
    }
    std::cout << "\tBad batches generated: " << this->badBatchesGenerated << std::endl;
}


//writes batch to file
void Simulation::writeBatchToFile(std::vector<std::string> &items, int fileNumber)
{
    std::ofstream out, cleaner;
    std::string filename;

    filename = "ds";
    filename += std::to_string(fileNumber);
    filename += ".txt";

    //erase the contents of the file
    cleaner.open(filename, std::ofstream::out | std::ofstream::trunc);
    //open the file and set write mode to append
    out.open(filename, std::ios_base::app);

    if (out.is_open())
    {
        for (std::string item : items)
        {

            out << item;
            out << "\n";
        }
    }
}


//uses montecarlo algorithm approach to detect bad batches
void Simulation::runDetectionAlgorithm()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<std::string> items;

    std::cout << "\nAnalyzing data sets..." << std::endl;
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
            while (getline(in, item))
            {
                items.push_back(item);
            }

        std::shuffle(items.begin(), items.end(), g);
        std::shuffle(items.begin(), items.end(), g);

        for (int j = 0; j < sampleSize; j++)
        {
            if (items.at(j) == "b")
            {
                this->badBatchesDetected++;
                std::cout << "\tbatch #" << i << " is bad" << std::endl;
                break;
            }
        }
        //empty vector
        items.clear();
    }
}

//generates a random number in the range  
int Simulation::generateRandomNumberInRange(std::uniform_int_distribution<int> myUnifIntDist)
{
    int number = myUnifIntDist(myRandomEngine);
    return number;
}

//initializes random engine
std::uniform_int_distribution<int> Simulation::initializeRandomEngine(int min, int max)
{
    std::uniform_int_distribution<int> myUnifIntDist(min, max);
    return myUnifIntDist;
}

void Simulation::calculateAndShowAnalytics()
{
    double base = 1.0 - ((double)this->badItemsPercent / (double)100);
    double probabilityFailure = std::pow(base, this->itemsSampled);
    double percentDetected = ((double)(this->badBatchesDetected) / (double)(this->badBatchesGenerated)) * (double)100;

    std::cout << "\nBase = " << base;
    std::cout << " exponent = " << this->itemsSampled << std::endl;
    std::cout << "P(failure to detect bad batch) = " << probabilityFailure << " %" << std::endl;
    std::cout << "Percentage of bad batches actually detected =  " << percentDetected << " %" << std::endl;
}
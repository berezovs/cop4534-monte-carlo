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

    std::random_device rd;
    std::mt19937 g(rd());
    int numBadBatches = ((double)this->badBatchesPercent / (double)100) * (double)this->numBatches;
    std::vector<std::string> items;
    bool flag = false;

    std::cout << "Generating data sets:" << std::endl;
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
            std::cout << "Create bad set batch #" << i << " totalbad = " << numBadItems << " total = " << this->numBatches << " pct " << this->badItemsPercent << std::endl;
        //reset bad batch flag
        flag = false;
        //shuffle items in vector
        std::shuffle(items.begin(), items.end(), g);
        this->writeBatchToFile(items, i+1);
        items.clear();
    }
    std::cout << "Bad batches generated: " << this->badBatchesGenerated << std::endl;
}

void Simulation::writeBatchToFile(std::vector<std::string> &items, int fileNumber)
{
    std::ofstream out, cleaner;
    std::string filename;

    filename = "ds";
    filename += std::to_string(fileNumber);
    filename += ".txt";

    cleaner.open(filename, std::ofstream::out | std::ofstream::trunc);
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

void Simulation::runDetectionAlgorithm()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<std::string> items;
    //std::uniform_int_distribution<int> random = this->initializeRandomEngine(0, 1);
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

        for (int i = 0; i < sampleSize; i++)
        {
            if (items.at(i) == "b")
            {
                this->badBatchesDetected++;
                break;
            }
        }
        items.clear();
    }
    
    std::cout << "Number of bad batches " << this->badBatchesDetected << "." << std::endl;
}

int Simulation::generateRandomNumberInRange(std::uniform_int_distribution<int> myUnifIntDist)
{

    int number = myUnifIntDist(myRandomEngine);
    return number;
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
    double base = 1.0-((double)this->badItemsPercent/(double)100);
    double probabilityFailure = std::pow(base, this->itemsSampled);
    double percentDetected = ((double)(this->badBatchesDetected) / (double)(this->badBatchesGenerated)) * (double)100;

    std::cout << "Base = " << base;

    std::cout << " exponent = " << this->itemsSampled << std::endl;

    std::cout << "P(failure to detect bad batch) = " << probabilityFailure << " %" << std::endl;

    std::cout << "Percentage of bad batches actually detected =  " << percentDetected << " %" << std::endl;
}
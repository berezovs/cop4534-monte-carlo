
/***************************************************************
  Student Name:Serghei Berezovschi
  File Name: Simulation.hpp
  Assignment number:Project 4-DS&AII
 
***************************************************************/
#ifndef SIMULATION_H
#define SIMULATION_H
#include <vector>
#include <string>
#include <fstream>
#include<random>
#include <algorithm>
class Simulation
{
public:
    Simulation(std::vector<std::string> files);
    void run();
    private:
    void printSimulationDetails();
    void loadBatchConfigFile(std::string fileName);
    void generateDataSets(std::uniform_int_distribution<int> dist);
    void writeBatchToFile(std::vector<std::string> &items, int fileNumber);
    void runDetectionAlgorithm();
    int generateRandomNumberInRange(std::uniform_int_distribution<int> uniformIntDist);
    void calculateAndShowAnalytics();
    std::uniform_int_distribution<int> initializeRandomEngine(int min, int max);

    int numBatches, numItemsInBatch, badItemsPercent, badBatchesPercent, itemsSampled, badBatchesDetected, badBatchesGenerated, simNumber;
    std::vector<std::string> batchMetaInfo;
    std::ifstream in;
    std::default_random_engine myRandomEngine;
};
#endif
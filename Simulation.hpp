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
    void printSimulationDetails();
    void loadBatchConfigFile(std::string fileName);
    void generateDataSets(std::uniform_int_distribution<int> dist);
    void writeBatchToFile(std::vector<std::string> &items, int fileNumber);
    void runDetectionAlgorithm();
    int generateRandomNumberInRange(std::uniform_int_distribution<int> myUnifIntDist);
    void calculateAndShowAnalytics();
    std::uniform_int_distribution<int> initializeRandomEngine(int min, int max);



private:
    int numBatches, numItemsInBatch, badItemsPercent, badBatchesPercent, itemsSampled, badBatchesDetected, badBatchesGenerated, simNumber;
    std::vector<std::string> batchMetaInfo;
    std::ifstream in;
    std::default_random_engine myRandomEngine;
};
#endif
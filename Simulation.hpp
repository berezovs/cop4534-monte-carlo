#ifndef SIMULATION_H
#define SIMULATION_H
#include <vector>
#include <string>
#include <fstream>
#include<random>
class Simulation
{
public:
    Simulation(std::vector<std::string> files);
    void run();
    void loadBatchConfigFile(std::string fileName);
    void generateDataSets(std::uniform_int_distribution<int> dist);
    void generateReport();
    void runDetectionAlgorithm();
    int generateRandomNumberInRange(std::uniform_int_distribution<int> myUnifIntDist);
    std::string getItem(int number, int percent);
    void calculateProbability();
    std::uniform_int_distribution<int> initializeRandomEngine(int min, int max);

private:
    int numBatches, numItemsInBatch, badItemsPercent, badBatchesPercent, itemsSampled, badBatchesDetected, badBatchesGenerated=0;
    unsigned int seed;
    std::vector<std::string> batchMetaInfo;
    std::ifstream in;
    std::default_random_engine myRandomEngine;
};
#endif
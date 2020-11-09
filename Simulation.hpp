#ifndef SIMULATION_H
#define SIMULATION_H
#include <vector>
#include <string>
class Simulation
{
public:
    Simulation(std::vector<std::string> files);
    void run();
    void loadBatchConfigFile(std::string fileName);
    void generateDataSet();
    void generateReport();
    void runDetectionAlgorithm();
    int generateRandomNumInRange(int from, int to);

private:
    int numBatches, numItemsInBatch, badItemsPercent, badBatchesPercent, itemsSampled, badItemsDetected;
    std::vector<std::string> batchMetaInfo;
};
#endif
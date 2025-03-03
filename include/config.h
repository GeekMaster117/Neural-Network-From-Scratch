#include <string>

#ifndef CONFIG_H
#define CONFIG_H

extern const unsigned int dotMulBlockSize;

extern const double randomGenMean;
double randomGenStdDeviation(unsigned int inputs);

extern const std::string trainDataset;
extern const std::string testDataset;

extern const unsigned int batchSize;

extern const double epsilon;

extern const double learningRate;
extern const double decay;

#endif
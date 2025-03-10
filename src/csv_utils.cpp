#include "csv_utils.hpp"

#include <fstream>
#include <sstream>

#include "config.h"
#include "error.h"

CSV::CSV(std::string datasetName): datasetName(datasetName)
{
    this -> datasetSize = this -> readDatasetSize();
    this -> sampleSize = this -> readSampleSize();
}

unsigned int CSV::readDatasetSize()
{
    std::ifstream file(datasetName);
    if(!file.is_open())
        throwFileCannotBeOpenedError(datasetName);

    std::string line;

    if(std::getline(file, line))
        if(line.empty())
            throwDatasetEmptyError();

    unsigned int size = 0;
    while(std::getline(file, line))
        if(!line.empty())
            ++size;

    return size;
}

unsigned int CSV::readSampleSize()
{
    std::ifstream file(datasetName);
    if(!file.is_open())
        throwFileCannotBeOpenedError(datasetName);

    std::string line;

    unsigned int size = 0;
    if(std::getline(file, line))
    {
        if(line.empty())
            throwDatasetEmptyError();

        std::stringstream ss(line);
        std::string cell;
        for(; std::getline(ss, cell, ','); ++size);
    }

    if(size != 0)
        --size;

    return size;
}

Matrix CSV::readDataset(unsigned int startIndex, unsigned int endIndex)
{
    if(startIndex >= endIndex)
        throwValueCannotBeGreaterError("StartIndex", startIndex, "EndIndex", endIndex);

    std::ifstream file(this -> datasetName);
    if(!file.is_open())
        throwFileCannotBeOpenedError(this -> datasetName);

    std::string line;

    if(std::getline(file, line))
        if(line.empty())
            throwDatasetEmptyError();

    std::vector<std::vector<double>> dataset(std::min(endIndex - startIndex, datasetSize - startIndex), std::vector<double>(sampleSize + 1));
    for(unsigned int currentIndex = 0; std::getline(file, line); ++currentIndex) 
    {
        if(currentIndex < startIndex)
            continue;
        if(currentIndex >= endIndex)
            break;

        if(line.empty())
        {
            --currentIndex;
            continue;
        }

        std::stringstream ss(line);
        std::string cell;
        for(unsigned int i = 0; std::getline(ss, cell, ','); ++i)
            dataset[currentIndex - startIndex][i] = std::stod(cell);
    }

    file.close();

    if(dataset.empty())
        throwDatasetEmptyError();

    return Matrix(std::min(endIndex - startIndex, (this -> datasetSize) - startIndex), (this -> sampleSize) + 1, dataset);
}

unsigned int CSV::getDatasetSize()
{
    return this -> datasetSize;
}

unsigned int CSV::getSampleSize()
{
    return this -> sampleSize;
}

Matrix CSV::getDataset()
{
    return this -> dataset;
}

unsigned int CSV::getChunkCount()
{
    unsigned int chunkCount = (this -> datasetSize) % chunkSize == 0 ? 0 : 1;
    chunkCount += (this -> datasetSize) / chunkSize;

    return chunkCount;
}

void CSV::loadAll()
{
    this -> dataset = this -> readDataset(0, this -> datasetSize);
}

void CSV::loadChunk(unsigned int chunkIndex)
{
    unsigned int chunkCount = this -> getChunkCount();
    if(chunkIndex >= chunkCount)
        throwValueCannotBeGreaterError("ChunkIndex", chunkIndex, "ChunkCount", chunkCount);

    unsigned int startIndex = chunkIndex * chunkSize, endIndex = std::min((chunkIndex * chunkSize) + chunkSize, datasetSize);

    this -> dataset = this -> readDataset(startIndex, endIndex);
}

unsigned int CSV::getBatchCount()
{
    unsigned int batchCount = (this -> dataset.getRows()) % batchSize == 0 ? 0 : 1;
    batchCount += (this -> dataset.getRows()) / batchSize;

    return batchCount;
}

Matrix CSV::getDatasetBatch(unsigned int batchIndex)
{
    unsigned int batchCount = this -> getBatchCount();
    if(batchIndex >= batchCount)
        throwValueCannotBeGreaterError("BatchIndex", batchIndex, "BatchCount", batchCount);

    unsigned int startIndex = batchIndex * batchSize, endIndex = std::min((batchIndex * batchSize) + batchSize, this -> dataset.getRows());

    std::vector<std::vector<double>> batch(endIndex - startIndex, std::vector<double>((this -> sampleSize) + 1));
    for(unsigned int i = 0; i < endIndex - startIndex; ++i)
        for(unsigned int j = 0; j < (this -> sampleSize) + 1; ++j)
            batch[i][j] = this -> dataset.getValue(i, j);

    return Matrix(endIndex - startIndex, (this -> sampleSize) + 1, batch);
}

Matrix CSV::getLabels()
{
    std::vector<std::vector<double>> labels(this -> dataset.getRows(), std::vector<double>(1));

    for(unsigned int i = 0; i < this -> dataset.getRows(); ++i)
        labels[i][0] = this -> dataset.getValue(i, 0);

    return Matrix(this -> dataset.getRows(), 1, labels);
}

Matrix CSV::getLabels(unsigned int batchIndex)
{
    Matrix batch = this -> getDatasetBatch(batchIndex);

    std::vector<std::vector<double>> labels(batch.getRows(), std::vector<double>(1));

    for(unsigned int i = 0; i < batch.getRows(); ++i)
        labels[i][0] = batch.getValue(i, 0);

    return Matrix(batch.getRows(), 1, labels);
}

Matrix CSV::getSamples()
{
    std::vector<std::vector<double>> samples(this -> dataset.getRows(), std::vector<double>(this -> sampleSize));

    for(unsigned int i = 0; i < this -> dataset.getRows(); ++i)
        for(unsigned int j = 1; j < (this -> sampleSize) + 1; ++j)
            samples[i][j - 1] = this -> dataset.getValue(i, j);

    return Matrix(this -> dataset.getRows(), this -> sampleSize, samples);
}

Matrix CSV::getSamples(unsigned int batchIndex)
{
    Matrix batch = this -> getDatasetBatch(batchIndex);

    std::vector<std::vector<double>> samples(batch.getRows(), std::vector<double>(this -> sampleSize));

    for(unsigned int i = 0; i < batch.getRows(); ++i)
        for(unsigned int j = 1; j < batch.getCols(); ++j)
            samples[i][j - 1] = batch.getValue(i, j);

    return Matrix(batch.getRows(), this -> sampleSize, samples);
}

unsigned int getDatasetSize(std::string datasetName)
{
    std::ifstream file(datasetName);
    if(!file.is_open())
        throwFileCannotBeOpenedError(datasetName);

    std::string line;

    if(std::getline(file, line))
        if(line.empty())
            throwDatasetEmptyError();

    unsigned int size = 0;
    for(; std::getline(file, line); ++size)
        if(line.empty())
        {
            --size;
            continue;
        }

    return size;
}

unsigned int getDatasetSampleSize(std::string datasetName)
{
    std::ifstream file(datasetName);
    if(!file.is_open())
        throwFileCannotBeOpenedError(datasetName);

    std::string line;

    unsigned int size = -1;
    if(std::getline(file, line))
    {
        if(line.empty())
            throwDatasetEmptyError();

        std::stringstream ss(line);
        std::string cell;
        for(; std::getline(ss, cell, ','); ++size);
    }

    return size;
}

Matrix readDataset(unsigned int startIndex, unsigned int endIndex, std::string datasetName)
{
    if(startIndex >= endIndex)
        throwValueCannotBeGreaterError("StartIndex", startIndex, "EndIndex", endIndex);

    std::ifstream file(datasetName);
    if(!file.is_open())
        throwFileCannotBeOpenedError(datasetName);

    std::string line;

    if(std::getline(file, line))
        if(line.empty())
            throwDatasetEmptyError();

    std::vector<std::vector<double>> dataset(std::min(endIndex - startIndex, getDatasetSize(datasetName) - startIndex), std::vector<double>(getDatasetSampleSize(datasetName)));
    for(unsigned int currentIndex = 0; std::getline(file, line); ++currentIndex) 
    {
        if(currentIndex < startIndex)
            continue;
        if(currentIndex >= endIndex)
            break;

        if(line.empty())
        {
            --currentIndex;
            continue;
        }

        std::stringstream ss(line);
        std::string cell;
        for(unsigned int i = 0; std::getline(ss, cell, ','); ++i)
            dataset[currentIndex - startIndex][i] = std::stod(cell);
    }

    file.close();

    if(dataset.empty())
        throwDatasetEmptyError();

    return Matrix(std::min(endIndex - startIndex, getDatasetSize(datasetName) - startIndex), getDatasetSampleSize(datasetName), dataset);
}

void writeDataset(Matrix* matrix, std::string datasetName)
{
    std::ofstream file(datasetName);
    if(!file.is_open())
        throwFileCannotBeOpenedError(datasetName);

    file << "row\\col";
    for(unsigned int i = 0; i < matrix -> getCols(); ++i)
        file << "," << i + 1;
    file << std::endl;

    for(unsigned int i = 0; i < matrix -> getRows(); ++i)
    {
        file << i + 1;
        for(unsigned int j = 0; j < matrix -> getCols(); ++j)
            file << "," << matrix -> getValue(i, j);
        file << std::endl;
    }

    file.close();
}

unsigned int getBatchCount(std::string datasetName)
{
    unsigned int datasetSize = getDatasetSize(datasetName);
    
    unsigned int batchCount = datasetSize % batchSize == 0 ? 0 : 1;
    batchCount += datasetSize / batchSize;

    return batchCount;
}

Matrix getDatasetBatch(unsigned int batchIndex, std::string datasetName)
{
    unsigned int batchCount = getBatchCount(datasetName);
    if(batchIndex >= batchCount)
        throwValueCannotBeGreaterError("BatchIndex", batchIndex, "BatchCount", batchCount);

    unsigned int datasetSize = getDatasetSize(datasetName);
    return readDataset(batchIndex * batchSize, std::min((batchIndex * batchSize) + batchSize, datasetSize), datasetName);
}

Matrix getLabels(std::string datasetName)
{
    Matrix dataset = readDataset(0, getDatasetSize(datasetName), datasetName);

    std::vector<std::vector<double>> labels(dataset.getRows(), std::vector<double>(1));
    for(unsigned int i = 0; i < dataset.getRows(); ++i)
        labels[i][0] = dataset.getValue(i, 0);

    return Matrix(dataset.getRows(), 1, labels);
}

Matrix getLabels(unsigned int batchIndex, std::string datasetName)
{
    Matrix datasetBatch = getDatasetBatch(batchIndex, datasetName);
 
    std::vector<std::vector<double>> labels(datasetBatch.getRows(), std::vector<double>(1));
    for(unsigned int i = 0; i < datasetBatch.getRows(); ++i)
        labels[i][0] = datasetBatch.getValue(i, 0);

    return Matrix(datasetBatch.getRows(), 1, labels);
}

Matrix getSamples(std::string datasetName)
{
    Matrix dataset = readDataset(0, getDatasetSize(datasetName), datasetName);

    std::vector<std::vector<double>> samples(dataset.getRows(), std::vector<double>(getDatasetSampleSize(datasetName)));
    for(unsigned int i = 0; i < dataset.getRows(); ++i)
        for(unsigned int j = 1; j < dataset.getCols(); ++j)
            samples[i][j - 1] = dataset.getValue(i, j);

    return Matrix(dataset.getRows(), getDatasetSampleSize(datasetName), samples);
}

Matrix getSamples(unsigned int batchIndex, std::string datasetName)
{
    Matrix datasetBatch = getDatasetBatch(batchIndex, datasetName);

    std::vector<std::vector<double>> samples(datasetBatch.getRows(), std::vector<double>(getDatasetSampleSize(datasetName)));
    for(unsigned int i = 0; i < datasetBatch.getRows(); ++i)
        for(unsigned int j = 1; j < datasetBatch.getCols(); ++j)
            samples[i][j - 1] = datasetBatch.getValue(i, j);

    return Matrix(datasetBatch.getRows(), getDatasetSampleSize(datasetName), samples);
}
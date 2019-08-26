### Description
There are five social network datasets [soc-Epinions1 (DS1)](https://snap.stanford.edu/data/soc-Epinions1.html), [Slashdot901 (DS2)](https://snap.stanford.edu/data/soc-Slashdot0902.html), [Amazon0302 (DS3)](https://snap.stanford.edu/data/amazon0302.html), [Amazon0601 (DS4)](https://snap.stanford.edu/data/amazon0601.html), and [com-youtube.ungraph (DS5)](https://snap.stanford.edu/data/com-Youtube.html), respectively. IMP has one parameter that is the probability of propagation, so use "CreateSampleDataset.cpp" to sample. The information for the datasets is described in the following table.


Dataset | number of nodes | number of edges | propagation probability
------- |:---------------:|:---------------:|:----------------------------:
DS1     | 75,879          | 508,837         | 0.05
DS2     | 82,168          | 948,464         | 0.04
DS3     | 262,111         | 1,234,877       | 0.10
DS4     | 403,394         | 3,387,388       | 0.04
DS5     | 1,134,890       | 2,987,624       | 0.08

### Compile
    g++ CreateSampleDataset.cpp -o CreateSampleDataset.exe -O3

### Usage
Three parameters are required to execute the program.

1. Input filename
2. propagation probability
3. Output filename

#### For example
Generate a sample "Sample.txt" with dataset "amazon0302.txt". Propagation probability is 0.05

    CreateSampleDataset.exe amazon0302.txt 0.05 > Sample.txt
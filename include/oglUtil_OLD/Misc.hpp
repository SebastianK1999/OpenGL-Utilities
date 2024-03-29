#ifndef MISC_INCLUDE
#define MISC_INCLUDE

#include <random>
#include <climits>

namespace Misc{
    class rand{
    private:
    rand(const rand&) = delete; // Delete copy constructor
    rand(); // private constructor
    static rand staticRand;

    size_t seed;

    std::random_device rd; // random device
    std::mt19937 generator; // generator

    // int distributors 
    std::uniform_int_distribution<int> disBinary; 
    std::uniform_int_distribution<int> disMaxInt;
    std::uniform_int_distribution<int> disAllInt;
    // double distributors
    std::uniform_real_distribution<double> disZeroOneDouble;
    
    public:
    static rand& get();

    static void setSeed(size_t);
    static void rmSeed();


    int binary(); // returns one or zero
    int allInt(); // returns positive and negative numbers
    int maxInt(); // returns positive number
    int randInt(int n, int N); // returns number in range
    int randInt(int N); // returns number in range from 0 to N

    double decim(); // returns number from 0.0 to 1.0
    double decim(double,double); // returns number in range
    double decim(double); // returns number in range from 0 to N

    };
} // end Misc

#endif
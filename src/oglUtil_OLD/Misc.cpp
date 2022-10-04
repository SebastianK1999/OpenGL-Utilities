#include "oglUtil_OLD/Misc.hpp"

Misc::rand Misc::rand::staticRand;
//Misc::rand Misc::rand::staticRandFromSeed(mn);

Misc::rand::rand():
    generator(rd()),
    disAllInt(INT_MIN,INT_MAX),
    disMaxInt(0,INT_MAX),
    disBinary(0,1),
    disZeroOneDouble(0.0,1.0)
{}

Misc::rand& Misc::rand::get(){ return Misc::rand::staticRand; }

void Misc::rand::setSeed(size_t arg_seed){ 
    Misc::rand::get().seed = arg_seed;
    std::mt19937 gen(arg_seed);
    Misc::rand().generator = gen;
}

void Misc::rand::rmSeed(){
    std::mt19937 gen(Misc::rand().rd());
    Misc::rand().generator = gen;
}

int Misc::rand::binary(){
    return this->disBinary(this->generator);
}
int Misc::rand::maxInt(){
    return this->disMaxInt(this->generator);
}
int Misc::rand::allInt(){
    return this->disAllInt(this->generator);
}
int Misc::rand::randInt(int N){
    return this->disMaxInt(this->generator)%N;
}
int Misc::rand::randInt(int n,int N){
    return this->disAllInt(this->generator)%(N-n) + n;
}

// rand double
double Misc::rand::decim(){
    return this->disZeroOneDouble(this->generator);
}
double Misc::rand::decim(double n, double N){
    return this->disZeroOneDouble(this->generator) * (N-n) + n;
}
double Misc::rand::decim(double N){
    return this->decim(0,N);
}
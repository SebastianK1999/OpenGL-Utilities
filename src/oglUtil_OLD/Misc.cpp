#include "oglUtil_OLD/Misc.hpp"

Misc::rand Misc::rand::staticRand;
//Misc::rand Misc::rand::staticRandFromSeed(mn);

Misc::rand::rand():
    generator(rd()),
    disAllInt(INT_MIN,INT_MAX),
    disMaxInt(0,INT_MAX),
    disBinary(0,1),
    disZeroOnedouble(0.0,1.0)
{}

Misc::rand& Misc::rand::get(){ return Misc::rand::staticRand; }

void Misc::rand::setSeed(size_t arg_seed){ 
    Misc::rand::get().seed = arg_seed;
    std::mt19937 gen(arg_seed);
    Misc::rand::get().generator = gen;
}

void Misc::rand::rmSeed(){
    std::mt19937 gen(Misc::rand::get().rd());
    Misc::rand::get().generator = gen;
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
double Misc::rand::Decim(){
    return this->disZeroOnedouble(this->generator);
}
double Misc::rand::Decim(double n, double N){
    return this->disZeroOnedouble(this->generator) * (N-n) + n;
}
double Misc::rand::Decim(double N){
    return this->Decim(0,N);
}
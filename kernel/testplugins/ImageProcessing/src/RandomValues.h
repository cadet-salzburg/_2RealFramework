#pragma once

#include "Poco/Random.h"

Poco::Random gRandom = Poco::Random::Random();

template<typename T>
T random_t() { return T(); }

template<>
unsigned int random_t< unsigned int >() { return gRandom.next(UINT_MAX); }

template<>
unsigned short random_t< unsigned short >() {  return gRandom.next(USHRT_MAX); }

template<>
float random_t< float >() {  return gRandom.nextFloat(); }

template<>
double random_t< double >() {  return gRandom.nextDouble(); }
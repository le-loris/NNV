#include <stdlib.h>
#include <math.h>
#include <chrono>
#include <vector>
#define MIN(a,b) (a > b) ? b : a
#define MAX(a,b) (a < b) ? b : a

double range_rand(double max = 1, double min = 0);

void activation(double &value);

std::vector<double> multiply(std::vector<std::vector<double>> matrix, std::vector<double> vector);
std::vector<std::vector<double>> multiply(std::vector<double> vA, std::vector<double> vB, double rate = 1);

std::vector<double> child_product(std::vector<double> A, std::vector<double>B);

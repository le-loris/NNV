#include "fonctions.h"

double range_rand(double max, double min){
    return min + (double(rand()) / RAND_MAX) * (max - min);
}

void activation(double &value){
    value = MIN(value,1);
    value = MAX(value,0);
    //value = (1000)/(1+exp(5-(value/100)));
}

std::vector<double> multiply(std::vector<std::vector<double> > matrix, std::vector<double> vector){
    std::vector<double> result;
    double temp;
    for(auto row : matrix){
        temp = 0;
        for(unsigned int i = 0; i < vector.size(); i++)
            temp += row.at(i) * vector.at(i);

        result.push_back(temp);
    }

    return result;
}

std::vector<std::vector<double>> multiply(std::vector<double> vA, std::vector<double> vB, double rate){
    std::vector<double> buf;
    std::vector<std::vector<double>> result;
    for(unsigned int i = 0; i < vA.size(); i++){
        buf.clear();
        for(unsigned int j = 0; j < vB.size(); j++){
            buf.push_back(rate * vA.at(i) * vB.at(j));
        }
        result.push_back(buf);
    }

    return result;
}

std::vector<double> child_product(std::vector<double> A, std::vector<double> B){
    if(A.size() != B.size()){
        return std::vector<double>();
    }

    std::vector<double> result;
    for(unsigned int i = 0; i < A.size(); i++){
        result.push_back(A.at(i)*B.at(i));
    }

    return result;
}

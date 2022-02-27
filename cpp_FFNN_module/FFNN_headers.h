
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<double> getRow(vector<vector<double>> matrix, int rownum) {

    vector<double> row;

    double b;

    for (int n = 0; n < matrix.size(); n++) {

        b = matrix[n][rownum];
        row.push_back(b);

    }

    return row;
}

double sigmoid(double x) {
    double result;
    result = 1 / (1 + exp(-x));
    return result;
}

double Dsigmoid(double x) {
    double result;
    result = exp(-x) / ((1 + exp(-x))* (1 + exp(-x)));
    return result;
}

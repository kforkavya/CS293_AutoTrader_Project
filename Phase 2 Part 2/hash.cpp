#include<cmath>
#include<string>
// Global variables
int N = 10000; //Map Size
double hash_ratio = (sqrt(5) - 1) / 2;
int max_string_length = 1000;
double* pre_computed_table;

double fractional(double x)
{
    return x-int(x);
}

void calc_pre_computed_table()
{
    pre_computed_table = new double[max_string_length];
    pre_computed_table[0] = hash_ratio;
    for(int i = 1; i<1000; i++)
        pre_computed_table[i] = fractional(33*pre_computed_table[i-1]);
}

int hashValue(std::string key){
    int hashValue = 0;
    double random = fractional(5381*hash_ratio);
    int minimum = 1000 < key.length() ? 1000 : key.length();
    for (int i = 0; i < minimum; i++)
    {
        double test = fractional(pre_computed_table[i] * key[i]);
        random = fractional(random + test);
    }
    hashValue = floor(N * random);
    return hashValue;
}

void delete_pre_computed_table()
{
    delete[] pre_computed_table;
    pre_computed_table = NULL;
}
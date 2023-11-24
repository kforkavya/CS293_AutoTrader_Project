#include<string>
#include<vector>
#include<algorithm>
using namespace std;

void stock_structure_disintegrator(string actual_stock_structure, vector<pair<string, int>>& tokenised_string, string& sorted_stock_structure)
{
    sorted_stock_structure = "";
    actual_stock_structure = actual_stock_structure + " ";
    int i=0;
    while(i<actual_stock_structure.length())
    {
        string company_name = "", quantity_string = "";
        while(actual_stock_structure[i]!=' ')
        {
            company_name = company_name + actual_stock_structure[i];
            i++;
        }
        i++;
        while(actual_stock_structure[i]!=' ')
        {
            quantity_string = quantity_string + actual_stock_structure[i];
            i++;
        }
        i++;
        int quantity = stoi(quantity_string);
        tokenised_string.push_back(make_pair(company_name, quantity));
    }
    sort(tokenised_string.begin(), tokenised_string.end());
    for(int i=0; i<tokenised_string.size(); i++)
    {
        sorted_stock_structure += tokenised_string[i].first + " " + to_string(tokenised_string[i].second) + " ";
    }
    sorted_stock_structure = sorted_stock_structure.substr(0, sorted_stock_structure.length()-1);
}
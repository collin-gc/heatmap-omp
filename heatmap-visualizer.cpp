#include "EasyBMP/EasyBMP.cpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int main()    {
    //---ARGUMENT VARIABLES---
    float base_temp;
    string csvFileName;
    string outputFileName = "heatmap.bmp";
    cout << "Enter base temperature:\t";
    cin >> base_temp;
    cout << "\nEnter csv file name:\t";
    cin.ignore();
    getline(cin, csvFileName);

    // Read in CSV file
    ifstream infile(csvFileName);
    if(!infile) {
        cout << "\nERROR: Could not open file.\n";
        return 1;
    }
    vector<vector<float>> data;
    string line;
    while (getline(infile, line)) {
        vector<float> row;
        float value;
        stringstream ss(line);
        while (ss >> value) {
            row.push_back(value);
            if (ss.peek() == ',') {
                ss.ignore();
            }
        }
        data.push_back(row);
    }
    infile.close();

    // Determine image dimensions
    int width = data[0].size();
    int height = data.size();

    // Create new BMP image
    BMP image;
    image.SetSize(width, height);
    image.SetBitDepth(24);

    // Set color of each pixel based on CSV data
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float value = data[y][x];
            float color;
            if(value > base_temp)   {
                color = (value - base_temp)*2;
                if (color > 255) { color = 255; }
                image(x, y)->Red = color;
                image(x, y)->Green = 81 + color/2;
                image(x, y)->Blue = 0;
            }
            else if (value < base_temp){
                color = (base_temp - value)*2;
                if (color > 255) { color = 255; }
                image(x, y)->Red = 0;
                image(x, y)->Green = 80 - color/2;
                image(x, y)->Blue = color;
            }
            else    {
                image(x, y)->Red = 0;
                image(x, y)->Green = 80;
                image(x, y)->Blue = 0;
            }
        }
    }

    // Save BMP image to file
    image.WriteToFile(outputFileName.c_str());
    cout << "\nHeatmap visualization has been stored into " << outputFileName << ".";

    return 0;
}

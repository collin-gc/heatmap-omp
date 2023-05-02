//AUTHOR: Collin Cunningham

#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>

int ROWS;
int COLS;

using namespace std;

struct Heater   {
    int row;
    int col;
    float heat;
};

void setHeaters(float* in, Heater* heaters, int h_count){
    for(int i = 0; i < h_count; i++)
    {
        in[(COLS*heaters[i].col) + heaters[i].row] = heaters[i].heat;
    }
}

float sumNeighbors(float* in, float k, int r, int c, float b)    {
    float sum = 0.0;
    if (c > 0 && c < COLS-1 && r > 0 && r < ROWS-1) {
        sum += in[(COLS*(c-1)) + (r-1)];
        sum += in[(COLS*(c-1)) + r];
        sum += in[(COLS*(c-1)) + (r+1)];
        sum += in[(COLS*c) + (r-1)];
        sum += in[(COLS*c) + (r+1)];
        sum += in[(COLS*(c+1)) + (r-1)];
        sum += in[(COLS*(c+1)) + r];
        sum += in[(COLS*(c+1)) + (r+1)];

        return (in[(COLS*c) + r] + (k*(sum))/8.0)/2.0;
    }
    for (int i = c - 1; i <= c + 1; i++) {
        for (int j = r - 1; j <= r + 1; j++) {
            if (i >= 0 && i < COLS && j >= 0 && j < ROWS && !(i == c && j == r)) {
                sum += in[(COLS*i) + j];
            }
            else if(!(i == c && j == r))    {
                sum += b;
            }
        }
    }
    return (in[(COLS*c) + r] + (k*(sum))/8.0)/2.0;
}

int main(int argc, char* argv[])    {
    //---ARGUMENT VARIABLES---
    int thread_count = stoi(argv[1]);
    ROWS = stoi(argv[2]);
    COLS = stoi(argv[3]);
    float base_temp  = stof(argv[4]);
    float k = stof(argv[5]);
    int timesteps = stoi(argv[6]);
    string heaterFileName = argv[7];
    string outputFileName = argv[8];

    //---ARRAYS & FILE---
    Heater (*registeredHeaters);
    float (*temperatureInput) = new float[ROWS * COLS];
    float (*temporary);
    float (*temperatureOutput) = new float[ROWS * COLS];
    ofstream outputFile;

    double startTime, endTime;
    startTime = omp_get_wtime();

    //INITIALIZING ARRAY
    for(int n = 0; n < COLS; n++)   {
        for(int m = 0; m < ROWS; m++)   {
            temperatureInput[(COLS*n) + m] = base_temp;
        }
    }

    //REGISTERING HEATERS
    int num_heaters;
    ifstream inFile;
    inFile.open(heaterFileName);
    inFile >> num_heaters;
    registeredHeaters = new Heater[num_heaters];
    for(int i = 0; i < num_heaters; i++)    {
        inFile >> registeredHeaters[i].row;
        inFile >> registeredHeaters[i].col;
        inFile >> registeredHeaters[i].heat;
    }
    inFile.close();

    //putting the heaters in the temperature file for the first time.
    setHeaters(&(*temperatureInput), registeredHeaters, num_heaters);

    //TIMESTEP EXECUTION
    for(int t = 0; t < timesteps; t++)  {
        int r,c;
        #pragma omp parallel for schedule(static) collapse(2) num_threads(thread_count)
        for(c = 0; c < COLS; c++)   {
            for(r = 0; r < ROWS; r++)   {
                temperatureOutput[(COLS*c) + r] = sumNeighbors(temperatureInput, k, r, c, base_temp);
            }
        }
        setHeaters(&(*temperatureOutput), registeredHeaters, num_heaters);
        if(t < timesteps-1) {
            temporary = temperatureInput;
            temperatureInput = temperatureOutput;
            temperatureOutput = temporary;
        }
    }


    //open output file
    outputFile.open(outputFileName);
    //write the grid to the file in CSV format
    for(int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            outputFile << temperatureOutput[(COLS*i) + j];
            if (j < (ROWS-1)) {
                //add a comma separator between columns
                outputFile << ",";
            }
        }
        //start a new row on a new line
        outputFile << "\n";
    }



    //closing file...
    outputFile.close();

    //clearing vector arrays...
    delete [] temperatureInput;
    delete [] temperatureOutput;

    endTime = omp_get_wtime();
    printf("\nTotal time elapsed:\t\t%.5f seconds\n", endTime - startTime);
    //ending program.
    return 0;
}
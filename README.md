# heatmap-omp
This is a heatmap simulation program built using C++ and OpenMP.

A two-dimensional matrix is populated with a given base temperature from command-line input and Heaters from a file, then heat transfer is calculated in parallel for a user-defined amount of timesteps. After completing the timesteps, the final matrix is output to a .CSV file, with the filename user-defined.

The following algorithm is used to calculate the temperature distribution:
T.new = (T.old + (k*(T.top + T.bottom + T.left + T.right + T.top_left + T.bottom_left + T.top_right + T.bottom_right))/8.0)/2.0


# Program Execution
./heat_transfer <num_threads> <numRows> <numCols> <base temp> <k> <timesteps> <heaterFileName> <outputFileName>
where K is the temperature transfer rate. A large K will result in a faster heat transfer.

# Visualization
The visualization program relies on the EasyBMP library. It is automatically included into the
visualizer upon compilation, so compiling it is the same as compiling any standard C++ file.
g++ HeatmapVisualizer.cpp -o HeatmapVisualizer
Running the visualizer is as simple as executing it through a terminal with the command
./HeatmapVisualizer or by double-clicking the executable file if it was compiled on
Windows.

When the visualizer is executed, it will ask the user for the same base temperature number that
was used in the Heat Transfer Algorithm program. Then, it will ask for the name of the CSV file
that the Heat Transfer Algorithm had output. When the correct arguments are given, the heatmap
in BMP format is created by the visualizer. Then, the visualizer prints a success message and
exits

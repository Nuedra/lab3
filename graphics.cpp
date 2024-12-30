#include "graphics.hpp"
#include "gnuplot-iostream.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

void plot_histogram_txt(const std::string& txt_filename, const std::string& png_filename) {
    std::ifstream file(txt_filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open txt file: " << txt_filename << std::endl;
        return;
    }

    int item_count = 0;
    file >> item_count;
    if (!file.good() || item_count <= 0) {
        std::cerr << "Invalid or empty txt file format (item_count <= 0)" << std::endl;
        return;
    }

    std::vector<std::string> interval_labels;
    std::vector<int> counts;
    interval_labels.reserve(item_count);
    counts.reserve(item_count);

    for (int i = 0; i < item_count; i++) {
        int start, end, val;
        file >> start >> end >> val;
        if (!file.good()) {
            std::cerr << "Failed to read row " << i << " from file." << std::endl;
            return;
        }

        std::ostringstream label_ss;
        label_ss << "[" << start << "," << end << ")";
        interval_labels.push_back(label_ss.str());
        counts.push_back(val);
    }

    file.close();

    Gnuplot gp;
    gp << "set terminal png size 800,600 enhanced font 'Arial,10'\n";
    gp << "set output '" << png_filename << "'\n";
    gp << "set title 'Histogram'\n";
    gp << "set xlabel 'Intervals'\n";
    gp << "set ylabel 'Count'\n";
    gp << "set grid\n";
    gp << "set boxwidth 0.5\n";
    gp << "set style fill solid 0.5\n";
    gp << "set xtics rotate by -45\n"; // поворот подписи, если много интервалов
    gp << "set key off\n";

    gp << "plot '-' using 2:xtic(1) with boxes lc rgb 'blue'\n";

    std::vector<std::pair<std::string, int>> plot_data;
    plot_data.reserve(item_count);
    for (int i = 0; i < item_count; i++) {
        plot_data.push_back(std::make_pair(interval_labels[i], counts[i]));
    }

    gp.send1d(plot_data);

    std::string command = "open " + png_filename; // для mac os
    // для Linux std::string command = "xdg-open " + png_filename;
    system(command.c_str());
}

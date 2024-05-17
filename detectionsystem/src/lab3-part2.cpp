#include <linesdiscoverysystem/readfiles.h>  //read points function -> given

#include <vector>
#include <string>
#include <filesystem>

#include <fstream>    //file reading and writing
#include <algorithm>  //sort
#include <vector>
#include <unordered_map>  //unordered map
#include <set>            //set

#include <rendering/window.h>
#include <fmt/format.h>

//structs allow public members by default - create class for points
struct Point {
    double x{0.0};
    double y{0.0};

    //comparison operator for points
    bool operator<(const Point& p) const {
        if (x != p.x) {
            return x < p.x;
        }
        return y < p.y;
    }
};

//compute slopes function
double compute_slope(const Point& p1, const Point& p2);

//compute collinear points
void find_collinears(const std::vector<Point>& points, const std::string& file_name);

//print collienar points segments
void print_collinear(const std::set<std::set<Point>>& segments); 

void plotData(const std::string& name);

//file handling
std::vector<Point> read_points_file(const std::string& file);
void write_to_file(const std::set<std::set<Point>>& lines, const std::string& file_name);

/* ************************************* */

int main() {
    try {
        std::cout << "Enter the name of input points file: ";
        std::string s;
        std::cin >> s;  // e.g. points1.txt, points200.txt, largeMystery.txt

        //read points from file
        std::vector<Point> the_points = read_points_file(s);
        //compute, print and write to file the collinear points segments 
        find_collinears(the_points, s);

        plotData(s);
    } catch (const std::exception& e) {
        fmt::print("Error: {}", e.what());
        return 1;
    }
}

/* ************************************* */

void plotData(const std::string& name) {
    std::filesystem::path points_name = name;
    const auto points = readPoints(data_dir / points_name);

    std::filesystem::path segments_name = "segments-" + name;
    const auto lines = readLineSegments(data_dir / "output" / segments_name);

    rendering::Window window(850, 850, rendering::Window::UseVSync::Yes);
    while (!window.shouldClose()) {
        window.beginFrame();
        window.clear({0, 0, 0, 1});
        window.drawLines(lines);    //to plot the line segments discovered
        window.drawPoints(points);  //to plot input points
        window.endFrame();
    }
}

/* **************************************** */
/* Function definitions */
/* **************************************** */

//file handling
std::vector<Point> read_points_file(const std::string& file) {

    //vector to store points
    std::vector<Point> points{};

    //create a file path system from the data folder
    std::filesystem::path points_file = data_dir / file;

    //input stream for file reading
    std::ifstream file_reader(points_file);

    //read file, if not possible return error message
    if (!file_reader) {
        std::cout << "error reading file!\n";
        return {};
    }

    //read first line = number of points
    int n_points = 0;
    file_reader >> n_points;

    //reserve vector for points in file
    points.reserve(n_points);

    //read the rest of the points
    for (int i = 0; i < n_points; ++i) {
        Point current_point = Point{};
        auto& the_point = points.emplace_back(current_point);
        file_reader >> the_point.x >> the_point.y;
    }

    return points;
}

void write_to_file(const std::set<std::set<Point>>& lines, const std::string& file_name) {

    //set up file path to output directory
    std::string output_dir = "output";
    std::string name = "segments-" + file_name;
    std::filesystem::path output_target = data_dir / output_dir / name;

    // input stream for file reading
    std::ofstream file_writer(output_target);

    // check if the file is open
    if (!file_writer.is_open()) {
        std::cout << "Error opening the file for writing!" << std::endl;
        return;
    }

    for (const auto& line : lines) {
        // convert the set to a vector and sort to determine endpoints
        std::vector<Point> sortedPoints(line.begin(), line.end());
        std::stable_sort(sortedPoints.begin(), sortedPoints.end());

        // assuming the first and last are the start and endpoints
        Point startPoint = sortedPoints.front();
        Point endPoint = sortedPoints.back();

        file_writer << startPoint.x << " " << startPoint.y << " " << endPoint.x << " " << endPoint.y
                    << std::endl;
    }
    file_writer.close();
}

//compute slopes function
double compute_slope(const Point& p1, const Point& p2) {

    //if lines are vertical, division by 0 not allowed
    if (p1.x == p2.x) {
        return std::numeric_limits<double>::infinity();
    }

    //compute the slope
    return (p2.y - p1.y) / (p2.x - p1.x);
}


//print collienar points segments
void print_collinear(const std::set<std::set<Point>>& segments) {

    //print points on segments 
    for (const auto& seg : segments) {

        //iterators to start and end of each segment seg
        auto seg_start_it = seg.begin(); 
        auto seg_end_it = seg.end(); 
        //move last iterator to last entry on seg
        --seg_end_it;

        for (; seg_start_it != seg.end(); ++seg_start_it) {
            std::cout << "(" << seg_start_it->x << ", " << seg_start_it->y << ")";
            if (seg_start_it != seg_end_it) {
                std::cout << " -> "; 
            }
        }

       std::cout << std::endl; 
    }
}

//compute collinear points
void find_collinears(const std::vector<Point>& points, const std::string& file_name) {

    //map=hash for points and slopes
    std::set<std::set<Point>> uniqueLines;

    //iterate points vector and compute slope for every point relative to all other points
    for (std::size_t i = 0; i < points.size(); ++i) {

        //map for points and slopes 
        std::unordered_map<double, std::set<Point>> points_and_slopes;

        for (std::size_t j = 0; j < points.size(); ++j) {

            //don't compute slope from the same point
            if (i == j) continue;

            //compute the slope
            double slope = compute_slope(points[i], points[j]);

            //insert computed slope and the points
            points_and_slopes[slope].insert(points[i]);
            points_and_slopes[slope].insert(points[j]);
        }
        //insert unique segemnts with 4 or more points 
        for (const auto& [slope, line] : points_and_slopes) {
            if (line.size() >= 4) {
                uniqueLines.insert(line);
            }
        }
    }

    //print collinear points 
    print_collinear(uniqueLines); 

    //write start- and end points to txt file
    write_to_file(uniqueLines, file_name);
}

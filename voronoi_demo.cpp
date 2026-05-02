#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <map>
#include <algorithm>
#include <limits>

struct Point {
    double x, y;
    std::string name;
    
    double distanceTo(const Point& other) const {
        return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }
};

struct Hospital {
    int id;
    std::string name;
    Point location;
};

struct VoronoiCell {
    int hospitalId;
    std::vector<Point> vertices;
    Point centroid;
    double area;
    double maxDistance;
    double avgDistance;
    bool isReasonable;
};

class VoronoiCalculator {
public:
    void computeVoronoi(const std::vector<Hospital>& hospitals, double minX, double maxX, double minY, double maxY, int resolution = 100) {
        double cellWidth = (maxX - minX) / resolution;
        double cellHeight = (maxY - minY) / resolution;
        
        std::map<int, std::vector<Point>> cellPoints;
        
        for (int i = 0; i <= resolution; ++i) {
            for (int j = 0; j <= resolution; ++j) {
                Point p{minX + i * cellWidth, minY + j * cellHeight, ""};
                
                double minDist = std::numeric_limits<double>::max();
                int nearestHospital = -1;
                
                for (size_t h = 0; h < hospitals.size(); ++h) {
                    double dist = p.distanceTo(hospitals[h].location);
                    if (dist < minDist) {
                        minDist = dist;
                        nearestHospital = static_cast<int>(h);
                    }
                }
                
                if (nearestHospital >= 0) {
                    cellPoints[nearestHospital].push_back(p);
                }
            }
        }
        
        m_cells.clear();
        for (size_t h = 0; h < hospitals.size(); ++h) {
            VoronoiCell cell;
            cell.hospitalId = hospitals[h].id;
            
            if (cellPoints.count(static_cast<int>(h)) && !cellPoints[static_cast<int>(h)].empty()) {
                cell.vertices = cellPoints[static_cast<int>(h)];
                
                double minXCell = std::numeric_limits<double>::max();
                double maxXCell = std::numeric_limits<double>::lowest();
                double minYCell = std::numeric_limits<double>::max();
                double maxYCell = std::numeric_limits<double>::lowest();
                
                double sumX = 0, sumY = 0;
                for (const auto& p : cell.vertices) {
                    minXCell = std::min(minXCell, p.x);
                    maxXCell = std::max(maxXCell, p.x);
                    minYCell = std::min(minYCell, p.y);
                    maxYCell = std::max(maxYCell, p.y);
                    sumX += p.x;
                    sumY += p.y;
                }
                
                cell.centroid = {sumX / cell.vertices.size(), sumY / cell.vertices.size(), ""};
                cell.area = (maxXCell - minXCell) * (maxYCell - minYCell);
                
                cell.maxDistance = 0;
                for (const auto& p : cell.vertices) {
                    double dist = p.distanceTo(hospitals[h].location);
                    cell.maxDistance = std::max(cell.maxDistance, dist);
                }
                
                cell.avgDistance = cell.maxDistance * 0.5;
                cell.isReasonable = cell.maxDistance <= 1000.0;
                
                m_cells.push_back(cell);
            }
        }
    }
    
    void printResults(const std::vector<Hospital>& hospitals) const {
        std::cout << "\n========== Hospital Distribution Evaluation ==========\n\n";
        
        int reasonableCount = 0;
        int totalMaxDist = 0;
        int totalAvgDist = 0;
        
        for (const auto& cell : m_cells) {
            const auto& hospital = std::find_if(hospitals.begin(), hospitals.end(),
                [&](const Hospital& h) { return h.id == cell.hospitalId; });
            
            if (hospital != hospitals.end()) {
                std::cout << "Hospital: " << hospital->name << " (ID: " << cell.hospitalId << ")\n";
                std::cout << "  Location: (" << hospital->location.x << ", " << hospital->location.y << ")\n";
                std::cout << "  Max Distance: " << cell.maxDistance << " m\n";
                std::cout << "  Avg Distance: " << cell.avgDistance << " m\n";
                std::cout << "  Coverage Area: " << cell.area << " m2\n";
                std::cout << "  Status: " << (cell.isReasonable ? "REASONABLE (< 1km)" : "UNREASONABLE (> 1km)") << "\n\n";
                
                if (cell.isReasonable) reasonableCount++;
                totalMaxDist += cell.maxDistance;
                totalAvgDist += cell.avgDistance;
            }
        }
        
        std::cout << "========== Summary ==========\n";
        std::cout << "Total Hospitals: " << m_cells.size() << "\n";
        std::cout << "Reasonable: " << reasonableCount << "\n";
        std::cout << "Unreasonable: " << m_cells.size() - reasonableCount << "\n";
        std::cout << "Coverage Rate: " << (m_cells.empty() ? 0 : (double)reasonableCount / m_cells.size() * 100) << "%\n";
        std::cout << "Global Max Distance: " << (m_cells.empty() ? 0 : totalMaxDist / m_cells.size()) << " m\n";
        std::cout << "Global Avg Distance: " << (m_cells.empty() ? 0 : totalAvgDist / m_cells.size()) << " m\n";
        std::cout << "========================================\n";
    }

private:
    std::vector<VoronoiCell> m_cells;
};

int main() {
    std::cout << "Hospital Distribution Planner - Voronoi Algorithm Demo\n\n";
    
    std::vector<Hospital> hospitals = {
        {1, "Central Hospital", {5000, 5000, ""}},
        {2, "North Clinic", {3000, 8000, ""}},
        {3, "South Medical Center", {7000, 2000, ""}},
        {4, "East Health Station", {8000, 5000, ""}},
        {5, "West Community Hospital", {2000, 5000, ""}}
    };
    
    std::cout << "Input: " << hospitals.size() << " hospitals in 10km x 10km area\n";
    
    VoronoiCalculator calculator;
    calculator.computeVoronoi(hospitals, 0, 10000, 0, 10000, 100);
    calculator.printResults(hospitals);
    
    std::cout << "\nNote: This is a simplified grid-based Voronoi approximation.\n";
    std::cout << "For production use, consider Boost.Geometry or CGAL for exact Voronoi computation.\n";
    
    return 0;
}

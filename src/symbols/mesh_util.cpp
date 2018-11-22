#include "mesh_util.h"
#include <algorithm>
#include <utility>

const GLfloat MIN_THICKNESS = 0.005f;


std::vector<glp::point<GLfloat>> sym::make_line_mesh(
        const std::vector<glp::point<GLfloat>>& points, 
        GLfloat thickness) {
    
    std::vector<glp::point<GLfloat>> result;
    
    thickness = std::max(thickness, MIN_THICKNESS);
    const GLfloat radius = thickness/2.0f;

    if (points.size() == 1) {
        result.emplace_back(points[0].x - radius, points[0].y - radius);
        result.emplace_back(points[0].x - radius, points[0].y + radius);
        result.emplace_back(points[0].x + radius, points[0].y + radius);
        
        result.emplace_back(points[0].x - radius, points[0].y - radius);
        result.emplace_back(points[0].x - radius, points[0].y + radius);
        result.emplace_back(points[0].x + radius, points[0].y - radius);
    } else {
        // Apply normal
        auto norm = get_normal(points[0], points[1]);
        result.emplace_back(points[0] - (norm * radius));
        result.emplace_back(points[0] + (norm * radius));
        for(size_t i=1; i<points.size() - 1; i++) {
            auto left_norm = get_normal(points[i-1], points[i]);
            auto right_norm = get_normal(points[i], points[i+1]);

            norm = (left_norm + right_norm) * 0.5f;
            size_t len = result.size();
            result.emplace_back(points[i] + (norm * radius));
            result.push_back(result[len - 2]);
            result.push_back(result[len]);
            result.emplace_back(points[i] - (norm * radius));
        }

        const auto& last_point = points[points.size()-1];
        // Apply normal
        norm = get_normal(points[points.size()-2], last_point);
        size_t len = result.size();
        result.push_back(result[len - 1]);
        result.push_back(result[len - 2]);
        result.emplace_back(last_point + (norm * radius));
        result.push_back(result[len - 1]);
        result.emplace_back(last_point + (norm * radius));
        result.emplace_back(last_point - (norm * radius));
    }

    return result;
}

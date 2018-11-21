#include "mesh_util.h"
#include <algorithm>

const GLfloat MIN_THICKNESS = 0.01f;


std::vector<glp::point<GLfloat>> sym::make_line_mesh(
        const std::vector<glp::point<GLfloat>>& points, 
        GLfloat thickness) {
    
    std::vector<glp::point<GLfloat>> result;
    
    thickness = std::min(thickness, MIN_THICKNESS);
    const GLfloat radius = thickness/2;

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
            result.push_back(result[result.size()-2]);
            auto left_norm = get_normal(points[i-1], points[i]);
            auto right_norm = get_normal(points[i], points[i+1]);

            norm = (left_norm + right_norm) * 0.5f;

            result.emplace_back(points[i] + (norm * radius));
            result.emplace_back(points[i] - (norm * radius));
        }

        const auto& last_point = points[points.size()-1];
        // Apply normal
        norm = get_normal(points[points.size()-2], last_point);
        result.emplace_back(last_point + (norm * radius));
        result.emplace_back(last_point - (norm * radius));
    }

    return result;
}

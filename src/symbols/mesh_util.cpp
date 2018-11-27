#include "mesh_util.h"
#include <algorithm>
#include <cmath>
#include <glm/ext.hpp>
#include <iostream>
#include <utility>

const GLfloat MIN_THICKNESS = 0.003f;

std::vector<glp::point<GLfloat>>
    sym::make_line_mesh(const std::vector<glp::point<GLfloat>> &points,
                        GLfloat thickness) {

    std::vector<glp::point<GLfloat>> result;

    thickness = std::max(thickness, MIN_THICKNESS);
    const GLfloat radius = thickness / 2.0f;

    if(points.size() == 1) {
        result.emplace_back(points[0].x - radius, points[0].y - radius);
        result.emplace_back(points[0].x - radius, points[0].y + radius);
        result.emplace_back(points[0].x + radius, points[0].y + radius);

        result.emplace_back(points[0].x - radius, points[0].y - radius);
        result.emplace_back(points[0].x - radius, points[0].y + radius);
        result.emplace_back(points[0].x + radius, points[0].y - radius);
    } else if(points.size() == 2) {
        auto norm = get_normal(points[0], points[1]);
        result.emplace_back(points[0] - (norm * radius));
        result.emplace_back(points[0] + (norm * radius));
        result.emplace_back(points[1] + (norm * radius));
        result.emplace_back(points[0] - (norm * radius));
        result.emplace_back(points[1] + (norm * radius));
        result.emplace_back(points[1] - (norm * radius));
    } else {
        // Apply normal
        auto norm = get_normal(points[0], points[1]);
        result.emplace_back(points[0] - (norm * radius));
        result.emplace_back(points[0] + (norm * radius));
        for(size_t i = 1; i < points.size() - 1; i++) {
            auto left_norm = get_normal(points[i - 1], points[i]);
            auto right_norm = get_normal(points[i], points[i + 1]);

            norm = (left_norm + right_norm) * 0.5f;
            size_t len = result.size();
            result.emplace_back(points[i] + (norm * radius));
            result.push_back(result[len - 2]);
            result.push_back(result[len]);
            result.emplace_back(points[i] - (norm * radius));
        }

        const auto &last_point = points[points.size() - 1];
        // Apply normal
        norm = get_normal(points[points.size() - 2], last_point);
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

std::vector<glp::point<GLfloat>>
    sym::make_scale_line(const glp::point<GLfloat> &left,
                         const glp::point<GLfloat> &right, GLfloat spacing,
                         GLfloat width, GLfloat thickness) {

    if(spacing <= 0.0f || width <= 0.0f || thickness <= 0.0f) {
        return {};
    }

    std::vector<glp::point<GLfloat>> result;

    thickness = std::max(thickness, MIN_THICKNESS);
    const GLfloat radius = thickness * 0.5f;

    auto direction = (right - left);
    auto length = direction.magnitude();
    direction = spacing * direction.normalise();
    auto norm =
        radius *
        get_normal(glp::point<GLfloat>(0.0f, 0.0f), direction).normalise();

    GLfloat acc = 0.0f;
    glp::point<GLfloat> pos = left;
    result.reserve(6 * length / spacing);
    while(acc < length) {
        auto segment = make_line_mesh({pos + norm, pos - norm}, width);
        result.insert(std::end(result), std::begin(segment), std::end(segment));
        acc += direction.magnitude();
        pos += direction;
    }

    return result;
}

std::vector<glp::point<GLfloat>>
    sym::radial_ticks(const glp::point<GLfloat> &centre, GLfloat d_theta,
                      GLfloat inner_radius, GLfloat outer_radius,
                      GLfloat thickness, GLfloat start_angle,
                      GLfloat end_angle) {

    std::vector<glp::point<GLfloat>> result;

    const size_t N_SLICES = (end_angle - start_angle) / d_theta + 1;
    result.reserve(N_SLICES * 6);

    std::vector<glp::point<GLfloat>> two_points(2);
    GLfloat theta = start_angle;
    for(size_t i = 0; i < N_SLICES; i++) {
        if(theta > end_angle) {
            break;
        }
        GLfloat ct = std::cos(theta);
        GLfloat st = std::sin(theta);
        theta += d_theta;
        two_points[0] = {inner_radius * ct + centre.x,
                         inner_radius * st + centre.y};
        two_points[1] = {outer_radius * ct + centre.x,
                         outer_radius * st + centre.y};

        const auto temp = make_line_mesh(two_points, thickness);
        result.insert(result.end(), temp.begin(), temp.end());
    }
    std::cout << result.size() << std::endl;

    return result;
}

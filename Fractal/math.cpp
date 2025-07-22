#include <complex>
#include <vector>
#include <cmath>


#define M_PI 3.14159265358979

float mandelbrot(double real, double imag) {
    std::complex<double> c(real, imag);
    std::complex<double> z(0.0, 0.0);
    int max_iter = 50;
    for (int i = 0; i < max_iter; ++i) {
        z = z * z + c;
        if (std::abs(z) > 2.0) {
            return static_cast<float>(i) / max_iter;
        }
    }
    return 1.0f;
}

std::vector<std::complex<double>> generate_koch_curve(int iterations) {
    std::vector<std::complex<double>> points = { {0.0, 0.0}, {1.0, 0.0} };
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        for (size_t j = 0; j < points.size() - 1; ++j) {
            auto p1 = points[j];
            auto p2 = points[j + 1];
            auto diff = (p2 - p1) / 3.0;
            new_points.push_back(p1);
            new_points.push_back(p1 + diff);
            new_points.push_back(p1 + diff * 1.5 + diff * std::complex<double>(0.0, 0.5 * sqrt(3.0)));
            new_points.push_back(p1 + diff * 2.0);
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}


 

float mandelbrot2(double real, double imag) {
    std::complex<double> c(real, imag);
    std::complex<double> z(0, 0);
    int max_iter = 100;
    int iter = 0;
    while (std::norm(z) < 4.0 && iter < max_iter) {
        z = z * z + c;
        ++iter;
    }
    if (iter < max_iter) {
        double log_zn = std::log(std::norm(z)) / 2.0;
        double nu = std::log(log_zn / std::log(2)) / std::log(2);
        return iter + 1 - nu;
    }
    return max_iter;
}
 
std::vector<std::complex<double>> generate_koch_curve2(int iterations) {
    std::vector<std::complex<double>> points = { {0.0, 0.0}, {1.0, 0.0} };
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        for (size_t j = 0; j < points.size() - 1; ++j) {
            std::complex<double> p1 = points[j];
            std::complex<double> p2 = points[j + 1];
            std::complex<double> delta = (p2 - p1) / 3.0;
            std::complex<double> p_mid1 = p1 + delta;
            std::complex<double> p_mid2 = p1 + delta * 1.5 + delta * std::complex<double>(0, std::sqrt(3) / 2);
            std::complex<double> p_mid3 = p1 + delta * 2.0;
            new_points.push_back(p1);
            new_points.push_back(p_mid1);
            new_points.push_back(p_mid2);
            new_points.push_back(p_mid3);
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}

float sierpinski_carpet(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < 0 || y > 1) return 0.0f;
    for (int i = 0; i < iterations; ++i) {
        int xi = static_cast<int>(x * 3);
        int yi = static_cast<int>(y * 3);
        if (xi == 1 && yi == 1) return 0.0f;
        x = (x * 3) - xi;
        y = (y * 3) - yi;
    }
    return 1.0f;
}

float cantor_dust(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < 0 || y > 1) return 0.0f;
    float value = 1.0f;
    for (int i = 0; i < iterations; ++i) {
        int xi = static_cast<int>(x * 3);
        int yi = static_cast<int>(y * 3);
        if (xi == 1 || yi == 1) value *= 0.0f;
        x = (x * 3) - xi;
        y = (y * 3) - yi;
    }
    return value;
}

std::vector<std::complex<double>> generate_dragon_curve(int iterations) {
    std::vector<std::complex<double>> points = { {0.0, 0.0}, {1.0, 0.0} };
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        for (size_t j = 0; j < points.size() - 1; ++j) {
            std::complex<double> p1 = points[j];
            std::complex<double> p2 = points[j + 1];
            std::complex<double> delta = (p2 - p1) / std::sqrt(2.0);
            std::complex<double> p_mid = p1 + delta * std::complex<double>(1.0, 1.0);
            new_points.push_back(p1);
            new_points.push_back(p_mid);
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}

std::vector<std::complex<double>> generate_levy_curve(int iterations) {
    std::vector<std::complex<double>> points = { {0.0, 0.0}, {1.0, 0.0} };
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        for (size_t j = 0; j < points.size() - 1; ++j) {
            std::complex<double> p1 = points[j];
            std::complex<double> p2 = points[j + 1];
            std::complex<double> delta = (p2 - p1) / std::sqrt(2.0);
            std::complex<double> p_mid = p1 + delta * std::complex<double>(1.0, 1.0);
            new_points.push_back(p1);
            new_points.push_back(p_mid);
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}

float peano_curve(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < 0 || y > 1) return 0.0f;
    int n = 1 << iterations;
    int index = 0;
    for (int i = 0; i < iterations; ++i) {
        int rx = static_cast<int>(x * 3);
        int ry = static_cast<int>(y * 3);
        index = index * 9 + (ry * 3 + rx);
        x = (x * 3) - rx;
        y = (y * 3) - ry;
    }
    return index / static_cast<float>(n * n);
}

float hilbert_curve(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < 0 || y > 1) return 0.0f;
    int n = 1 << iterations;
    int index = 0;
    for (int i = 0; i < iterations; ++i) {
        int rx = static_cast<int>(x * 2);
        int ry = static_cast<int>(y * 2);
        index = index * 4 + ((rx ^ ry) * 2 + rx);
        x = (x * 2) - rx;
        y = (y * 2) - ry;
    }
    return index / static_cast<float>(n * n);
}

float sierpinski_triangle(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < 0 || y > 1 || y > 1 - x) return 0.0f;
    for (int i = 0; i < iterations; ++i) {
        int xi = static_cast<int>(x * 2);
        int yi = static_cast<int>(y * 2);
        if (xi + yi >= 2) return 0.0f;
        x = (x * 2) - xi;
        y = (y * 2) - yi;
    }
    return 1.0f;
}

float box_fractal(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < 0 || y > 1) return 0.0f;
    float value = 1.0f;
    for (int i = 0; i < iterations; ++i) {
        int xi = static_cast<int>(x * 3);
        int yi = static_cast<int>(y * 3);
        if ((xi == 0 || xi == 2) && (yi == 0 || yi == 2)) value *= 1.0f;
        else value *= 0.0f;
        x = (x * 3) - xi;
        y = (y * 3) - yi;
    }
    return value;
}


float cantor_ternary_grid(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < 0 || y > 1) return 0.0f;
    float value = 1.0f;
    for (int i = 0; i < iterations; ++i) {
        int xi = static_cast<int>(x * 3);
        int yi = static_cast<int>(y * 3);
        if (xi == 1 && yi == 1) value *= 0.0f;
        x = (x * 3) - xi;
        y = (y * 3) - yi;
    }
    return value;
}

std::vector<std::complex<double>> generate_gosper_curve(int iterations) {
    std::vector<std::complex<double>> points = { {0.0, 0.0}, {1.0, 0.0} };
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        double scale = 1.0 / std::sqrt(7.0);
        std::complex<double> rot60 = std::polar(1.0, M_PI / 3.0);
        for (size_t j = 0; j < points.size() - 1; ++j) {
            std::complex<double> p1 = points[j];
            std::complex<double> p2 = points[j + 1];
            std::complex<double> delta = (p2 - p1) * scale;
            new_points.push_back(p1);
            new_points.push_back(p1 + delta);
            new_points.push_back(p1 + delta * (1.0 + rot60));
            new_points.push_back(p1 + delta * (1.0 + rot60 + rot60 * rot60));
            new_points.push_back(p1 + delta * (2.0 + rot60));
            new_points.push_back(p1 + delta * (2.0 + rot60 * 2.0));
            new_points.push_back(p1 + delta * (3.0 + rot60));
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}

std::vector<std::complex<double>> generate_cesaro_curve(int iterations) {
    std::vector<std::complex<double>> points = { {0.0, 0.0}, {1.0, 0.0} };
    double angle = M_PI * 85.0 / 180.0; // 85-degree angle as per Cesàro curve
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        double scale = 1.0 / (2.0 * std::cos(angle / 2.0));
        std::complex<double> rot = std::polar(1.0, angle / 2.0);
        for (size_t j = 0; j < points.size() - 1; ++j) {
            std::complex<double> p1 = points[j];
            std::complex<double> p2 = points[j + 1];
            std::complex<double> delta = (p2 - p1) * scale;
            new_points.push_back(p1);
            new_points.push_back(p1 + delta * rot);
            new_points.push_back(p1 + delta * (rot + std::conj(rot)));
            new_points.push_back(p1 + delta * (std::conj(rot) + 1.0));
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}

std::vector<std::complex<double>> generate_koch_snowflake(int iterations) {
    double side = 1.0;
    std::vector<std::complex<double>> points = {
        {0.0, 0.0},
        {side, 0.0},
        {side / 2.0, side * std::sqrt(3.0) / 2.0},
        {0.0, 0.0}
    };
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        for (size_t j = 0; j < points.size() - 1; ++j) {
            std::complex<double> p1 = points[j];
            std::complex<double> p2 = points[j + 1];
            std::complex<double> delta = (p2 - p1) / 3.0;
            std::complex<double> p_mid1 = p1 + delta;
            std::complex<double> p_mid2 = p1 + delta * 1.5 + delta * std::complex<double>(0, std::sqrt(3) / 2);
            std::complex<double> p_mid3 = p1 + delta * 2.0;
            new_points.push_back(p1);
            new_points.push_back(p_mid1);
            new_points.push_back(p_mid2);
            new_points.push_back(p_mid3);
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}

std::vector<std::complex<double>> generate_sierpinski_arrowhead(int iterations) {
    std::vector<std::complex<double>> points = { {0.0, 0.0}, {1.0, 0.0} };
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        double scale = 0.5;
        std::complex<double> rot60 = std::polar(1.0, M_PI / 3.0);
        for (size_t j = 0; j < points.size() - 1; ++j) {
            std::complex<double> p1 = points[j];
            std::complex<double> p2 = points[j + 1];
            std::complex<double> delta = (p2 - p1) * scale;
            new_points.push_back(p1);
            new_points.push_back(p1 + delta * rot60);
            new_points.push_back(p1 + delta * (rot60 + 1.0));
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}

std::vector<std::complex<double>> generate_quadric_koch(int iterations) {
    std::vector<std::complex<double>> points = {
        {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}, {0.0, 0.0}
    };
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        double scale = 1.0 / 3.0;
        for (size_t j = 0; j < points.size() - 1; ++j) {
            std::complex<double> p1 = points[j];
            std::complex<double> p2 = points[j + 1];
            std::complex<double> delta = (p2 - p1) * scale;
            new_points.push_back(p1);
            new_points.push_back(p1 + delta);
            new_points.push_back(p1 + delta + std::complex<double>(0.0, delta.imag()));
            new_points.push_back(p1 + delta * 2.0 + std::complex<double>(0.0, delta.imag()));
            new_points.push_back(p1 + delta * 2.0 + std::complex<double>(0.0, delta.imag()) - delta);
            new_points.push_back(p1 + delta * 2.0);
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}



std::vector<std::complex<double>> generate_minkowski_sausage(int iterations) {
    std::vector<std::complex<double>> points = { {0.0, 0.0}, {1.0, 0.0} };
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        double scale = 1.0 / 4.0;
        for (size_t j = 0; j < points.size() - 1; ++j) {
            std::complex<double> p1 = points[j];
            std::complex<double> p2 = points[j + 1];
            std::complex<double> delta = (p2 - p1) * scale;
            new_points.push_back(p1);
            new_points.push_back(p1 + delta);
            new_points.push_back(p1 + delta + std::complex<double>(0.0, delta.real()));
            new_points.push_back(p1 + delta * 2.0 + std::complex<double>(0.0, delta.real()));
            new_points.push_back(p1 + delta * 2.0);
            new_points.push_back(p1 + delta * 2.0 - std::complex<double>(0.0, delta.real()));
            new_points.push_back(p1 + delta * 3.0 - std::complex<double>(0.0, delta.real()));
            new_points.push_back(p1 + delta * 3.0);
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}

float moore_curve(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < 0 || y > 1) return 0.0f;
    int n = 1 << iterations;
    int index = 0;
    for (int i = 0; i < iterations; ++i) {
        int rx = static_cast<int>(x * 2);
        int ry = static_cast<int>(y * 2);
        int quadrant = (rx ^ ry) * 2 + rx;
        if (i % 2 == 0) quadrant = (quadrant + 1) % 4;
        index = index * 4 + quadrant;
        x = (x * 2) - rx;
        y = (y * 2) - ry;
    }
    return index / static_cast<float>(n * n);
}

float sierpinski_hexagon(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < -0.5 || y > 1.5) return 0.0f;
    double cx = 0.5, cy = std::sqrt(3.0) / 4.0;
    double r = std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
    if (r > std::sqrt(3.0) / 2.0) return 0.0f;
    for (int i = 0; i < iterations; ++i) {
        double scale = 3.0;
        int closest = 0;
        double min_dist = std::numeric_limits<double>::max();
        for (int j = 0; j < 6; ++j) {
            double angle = j * M_PI / 3.0;
            double hx = cx + std::cos(angle) * std::sqrt(3.0) / (2.0 * scale);
            double hy = cy + std::sin(angle) * std::sqrt(3.0) / (2.0 * scale);
            double dist = std::sqrt((x - hx) * (x - hx) + (y - hy) * (y - hy));
            if (dist < min_dist) {
                min_dist = dist;
                closest = j;
            }
        }
        if (closest == 0 && min_dist < std::sqrt(3.0) / (6.0 * scale)) return 0.0f;
        double angle = closest * M_PI / 3.0;
        x = (x - cx) * scale + cx - std::cos(angle) * std::sqrt(3.0) / 2.0;
        y = (y - cy) * scale + cy - std::sin(angle) * std::sqrt(3.0) / 2.0;
    }
    return 1.0f;
}

float cantor_maze(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < 0 || y > 1) return 0.0f;
    float value = 1.0f;
    for (int i = 0; i < iterations; ++i) {
        int xi = static_cast<int>(x * 3);
        int yi = static_cast<int>(y * 3);
        if ((xi == 1 || yi == 1) && !(xi == 1 && yi == 1)) value *= 0.0f;
        x = (x * 3) - xi;
        y = (y * 3) - yi;
    }
    return value;
}

std::vector<std::complex<double>> generate_koch_anti_snowflake(int iterations) {
    double side = 1.0;
    std::vector<std::complex<double>> points = {
        {0.0, 0.0},
        {side, 0.0},
        {side / 2.0, side * std::sqrt(3.0) / 2.0},
        {0.0, 0.0}
    };
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        for (size_t j = 0; j < points.size() - 1; ++j) {
            std::complex<double> p1 = points[j];
            std::complex<double> p2 = points[j + 1];
            std::complex<double> delta = (p2 - p1) / 3.0;
            std::complex<double> p_mid1 = p1 + delta;
            std::complex<double> p_mid2 = p1 + delta * 1.5 - delta * std::complex<double>(0, std::sqrt(3) / 2);
            std::complex<double> p_mid3 = p1 + delta * 2.0;
            new_points.push_back(p1);
            new_points.push_back(p_mid1);
            new_points.push_back(p_mid2);
            new_points.push_back(p_mid3);
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}

float peano_meander_curve(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < 0 || y > 1) return 0.0f;
    int n = 1 << iterations;
    int index = 0;
    for (int i = 0; i < iterations; ++i) {
        int rx = static_cast<int>(x * 3);
        int ry = static_cast<int>(y * 3);
        int quadrant = (ry * 3 + rx);
        if (i % 2 == 1) quadrant = (9 - quadrant) % 9;
        index = index * 9 + quadrant;
        x = (x * 3) - rx;
        y = (y * 3) - ry;
    }
    return index / static_cast<float>(n * n);
}


std::vector<std::complex<double>> generate_terdragon_curve(int iterations) {
    std::vector<std::complex<double>> points = { {0.0, 0.0}, {1.0, 0.0} };
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        double scale = 1.0 / std::sqrt(3.0);
        std::complex<double> rot120 = std::polar(1.0, 2.0 * M_PI / 3.0);
        for (size_t j = 0; j < points.size() - 1; ++j) {
            std::complex<double> p1 = points[j];
            std::complex<double> p2 = points[j + 1];
            std::complex<double> delta = (p2 - p1) * scale;
            new_points.push_back(p1);
            new_points.push_back(p1 + delta);
            new_points.push_back(p1 + delta * (1.0 + rot120));
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}

float vicsek_fractal(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < 0 || y > 1) return 0.0f;
    float value = 1.0f;
    for (int i = 0; i < iterations; ++i) {
        int xi = static_cast<int>(x * 3);
        int yi = static_cast<int>(y * 3);
        if ((xi == 1 && yi != 1) || (yi == 1 && xi != 1) || (xi == 1 && yi == 1)) value *= 1.0f;
        else value *= 0.0f;
        x = (x * 3) - xi;
        y = (y * 3) - yi;
    }
    return value;
}

std::vector<std::complex<double>> generate_koch_island(int iterations) {
    std::vector<std::complex<double>> points = {
        {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}, {0.0, 0.0}
    };
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        double scale = 1.0 / 3.0;
        for (size_t j = 0; j < points.size() - 1; ++j) {
            std::complex<double> p1 = points[j];
            std::complex<double> p2 = points[j + 1];
            std::complex<double> delta = (p2 - p1) * scale;
            new_points.push_back(p1);
            new_points.push_back(p1 + delta);
            new_points.push_back(p1 + delta + std::complex<double>(delta.imag(), -delta.real()));
            new_points.push_back(p1 + delta * 2.0 + std::complex<double>(delta.imag(), -delta.real()));
            new_points.push_back(p1 + delta * 2.0);
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}

float hexaflake(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < -0.5 || y > 1.5) return 0.0f;
    double cx = 0.5, cy = std::sqrt(3.0) / 4.0;
    double r = std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
    if (r > std::sqrt(3.0) / 2.0) return 0.0f;
    for (int i = 0; i < iterations; ++i) {
        double scale = 3.0;
        int closest = 0;
        double min_dist = std::numeric_limits<double>::max();
        for (int j = 0; j < 7; ++j) {
            double angle = (j == 0) ? 0 : (j - 1) * M_PI / 3.0;
            double hx = cx + (j == 0 ? 0 : std::cos(angle) * std::sqrt(3.0) / (3.0 * scale));
            double hy = cy + (j == 0 ? 0 : std::sin(angle) * std::sqrt(3.0) / (3.0 * scale));
            double dist = std::sqrt((x - hx) * (x - hx) + (y - hy) * (y - hy));
            if (dist < min_dist) {
                min_dist = dist;
                closest = j;
            }
        }
        if (closest == 0) return 1.0f;
        double angle = (closest - 1) * M_PI / 3.0;
        x = (x - cx) * scale + cx - std::cos(angle) * std::sqrt(3.0) / 3.0;
        y = (y - cy) * scale + cy - std::sin(angle) * std::sqrt(3.0) / 3.0;
    }
    return 1.0f;
}


std::vector<std::complex<double>> generate_heighway_dragon_variant(int iterations) {
    std::vector<std::complex<double>> points = { {0.0, 0.0}, {1.0, 0.0} };
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        double scale = 1.0 / std::sqrt(2.0);
        std::complex<double> rot90 = std::polar(1.0, M_PI / 2.0);
        for (size_t j = 0; j < points.size() - 1; ++j) {
            std::complex<double> p1 = points[j];
            std::complex<double> p2 = points[j + 1];
            std::complex<double> delta = (p2 - p1) * scale;
            new_points.push_back(p1);
            new_points.push_back(p1 + delta * (j % 2 == 0 ? rot90 : std::conj(rot90)));
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}

std::vector<std::complex<double>> generate_snowflake_sweep(int iterations) {
    std::vector<std::complex<double>> points = {
        {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}, {0.0, 0.0}
    };
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        double scale = 1.0 / 4.0;
        for (size_t j = 0; j < points.size() - 1; ++j) {
            std::complex<double> p1 = points[j];
            std::complex<double> p2 = points[j + 1];
            std::complex<double> delta = (p2 - p1) * scale;
            new_points.push_back(p1);
            new_points.push_back(p1 + delta);
            new_points.push_back(p1 + delta + std::complex<double>(0.0, delta.real()));
            new_points.push_back(p1 + delta * 2.0);
            new_points.push_back(p1 + delta * 2.0 + std::complex<double>(0.0, -delta.real()));
            new_points.push_back(p1 + delta * 3.0);
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}

float cantor_square(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < 0 || y > 1) return 0.0f;
    float value = 1.0f;
    for (int i = 0; i < iterations; ++i) {
        int xi = static_cast<int>(x * 3);
        int yi = static_cast<int>(y * 3);
        if (!(xi == 0 || xi == 2) || !(yi == 0 || yi == 2)) value *= 0.0f;
        x = (x * 3) - xi;
        y = (y * 3) - yi;
    }
    return value;
}

float hilbert_variant(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < 0 || y > 1) return 0.0f;
    int n = 1 << iterations;
    int index = 0;
    for (int i = 0; i < iterations; ++i) {
        int rx = static_cast<int>(x * 2);
        int ry = static_cast<int>(y * 2);
        int quadrant = (rx * 2 + ry);
        if (i % 2 == 1) quadrant = (3 - quadrant) % 4;
        index = index * 4 + quadrant;
        x = (x * 2) - rx;
        y = (y * 2) - ry;
    }
    return index / static_cast<float>(n * n);
}

float sierpinski_pentagon(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < -0.5 || y > 1.5) return 0.0f;
    double cx = 0.5, cy = 0.5 * std::tan(M_PI / 5.0);
    double r = std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
    if (r > std::sin(2.0 * M_PI / 5.0) / (2.0 * std::cos(M_PI / 5.0))) return 0.0f;
    for (int i = 0; i < iterations; ++i) {
        double scale = 2.0 + std::cos(2.0 * M_PI / 5.0);
        int closest = 0;
        double min_dist = std::numeric_limits<double>::max();
        for (int j = 0; j < 6; ++j) {
            double angle = j * 2.0 * M_PI / 5.0;
            double px = cx + (j == 0 ? 0 : std::cos(angle) * std::sin(2.0 * M_PI / 5.0) / scale);
            double py = cy + (j == 0 ? 0 : std::sin(angle) * std::sin(2.0 * M_PI / 5.0) / scale);
            double dist = std::sqrt((x - px) * (x - px) + (y - py) * (y - py));
            if (dist < min_dist) {
                min_dist = dist;
                closest = j;
            }
        }
        if (closest == 0) return 0.0f;
        double angle = (closest - 1) * 2.0 * M_PI / 5.0;
        x = (x - cx) * scale + cx - std::cos(angle) * std::sin(2.0 * M_PI / 5.0) / scale;
        y = (y - cy) * scale + cy - std::sin(angle) * std::sin(2.0 * M_PI / 5.0) / scale;
    }
    return 1.0f;
}


std::vector<std::complex<double>> generate_dekking_curve(int iterations) {
    std::vector<std::complex<double>> points = { {0.0, 0.0}, {1.0, 0.0} };
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        double scale = 1.0 / std::sqrt(5.0);
        std::complex<double> rot90 = std::polar(1.0, M_PI / 2.0);
        for (size_t j = 0; j < points.size() - 1; ++j) {
            std::complex<double> p1 = points[j];
            std::complex<double> p2 = points[j + 1];
            std::complex<double> delta = (p2 - p1) * scale;
            new_points.push_back(p1);
            new_points.push_back(p1 + delta * rot90);
            new_points.push_back(p1 + delta * (rot90 + 1.0));
            new_points.push_back(p1 + delta * (rot90 + 2.0));
            new_points.push_back(p1 + delta * (rot90 + 2.0 + rot90));
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}

std::vector<std::complex<double>> generate_gosper_island(int iterations) {
    std::vector<std::complex<double>> points = {
        {0.0, 0.0}, {1.0, 0.0}, {0.5, std::sqrt(3.0) / 2.0}, {0.0, 0.0}
    };
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        double scale = 1.0 / std::sqrt(7.0);
        std::complex<double> rot60 = std::polar(1.0, M_PI / 3.0);
        for (size_t j = 0; j < points.size() - 1; ++j) {
            std::complex<double> p1 = points[j];
            std::complex<double> p2 = points[j + 1];
            std::complex<double> delta = (p2 - p1) * scale;
            new_points.push_back(p1);
            new_points.push_back(p1 + delta);
            new_points.push_back(p1 + delta * (1.0 + rot60));
            new_points.push_back(p1 + delta * (1.0 + rot60 + rot60));
            new_points.push_back(p1 + delta * (2.0 + rot60));
            new_points.push_back(p1 + delta * (2.0 + rot60 * 2.0));
            new_points.push_back(p1 + delta * (3.0 + rot60));
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}

float sierpinski_square(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < 0 || y > 1) return 0.0f;
    float value = 1.0f;
    for (int i = 0; i < iterations; ++i) {
        int xi = static_cast<int>(x * 5);
        int yi = static_cast<int>(y * 5);
        if (xi == 2 && yi == 2) return 0.0f;
        x = (x * 5) - xi;
        y = (y * 5) - yi;
    }
    return value;
}

std::vector<std::complex<double>> generate_koch_quadratic(int iterations) {
    std::vector<std::complex<double>> points = { {0.0, 0.0}, {1.0, 0.0} };
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::complex<double>> new_points;
        double scale = 1.0 / 4.0;
        std::complex<double> rot90 = std::polar(1.0, M_PI / 2.0);
        for (size_t j = 0; j < points.size() - 1; ++j) {
            std::complex<double> p1 = points[j];
            std::complex<double> p2 = points[j + 1];
            std::complex<double> delta = (p2 - p1) * scale;
            new_points.push_back(p1);
            new_points.push_back(p1 + delta);
            new_points.push_back(p1 + delta + delta * rot90);
            new_points.push_back(p1 + delta * 2.0 + delta * rot90);
            new_points.push_back(p1 + delta * 2.0);
            new_points.push_back(p1 + delta * 2.0 - delta * rot90);
            new_points.push_back(p1 + delta * 3.0 - delta * rot90);
            new_points.push_back(p1 + delta * 3.0);
        }
        new_points.push_back(points.back());
        points = new_points;
    }
    return points;
}

float cantor_cloud(double x, double y, int iterations) {
    x = (x - 0.0) / (1.0 - 0.0);
    y = (y - 0.0) / (1.0 - 0.0);
    if (x < 0 || x > 1 || y < 0 || y > 1) return 0.0f;
    float value = 1.0f;
    for (int i = 0; i < iterations; ++i) {
        int xi = static_cast<int>(x * 3);
        int yi = static_cast<int>(y * 3);
        if (xi == 1 && yi == 1) value *= 0.0f;
        else if ((xi == 1 || yi == 1) && (rand() % 2)) value *= 0.5f;
        x = (x * 3) - xi;
        y = (y * 3) - yi;
    }
    return value;
}

 
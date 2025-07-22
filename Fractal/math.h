#ifndef MATH_H
#define MATH_H

#include <complex>
#include <vector>
#include <cmath>

// Mandelbrot set
float mandelbrot(double real, double imag);

// Fractal generators that return point sets
std::vector<std::complex<double>> generate_koch_curve(int iterations);
std::vector<std::complex<double>> generate_dragon_curve(int iterations);
std::vector<std::complex<double>> generate_levy_curve(int iterations);
std::vector<std::complex<double>> generate_gosper_curve(int iterations);
std::vector<std::complex<double>> generate_cesaro_curve(int iterations);
std::vector<std::complex<double>> generate_koch_snowflake(int iterations);
std::vector<std::complex<double>> generate_sierpinski_arrowhead(int iterations);
std::vector<std::complex<double>> generate_quadric_koch(int iterations);
std::vector<std::complex<double>> generate_minkowski_sausage(int iterations);
std::vector<std::complex<double>> generate_koch_anti_snowflake(int iterations);
std::vector<std::complex<double>> generate_terdragon_curve(int iterations);
std::vector<std::complex<double>> generate_koch_island(int iterations);
std::vector<std::complex<double>> generate_heighway_dragon_variant(int iterations);
std::vector<std::complex<double>> generate_snowflake_sweep(int iterations);
std::vector<std::complex<double>> generate_dekking_curve(int iterations);
std::vector<std::complex<double>> generate_gosper_island(int iterations);
std::vector<std::complex<double>> generate_koch_quadratic(int iterations);

// Fractal evaluation functions (point-in-fractal tests)
float koch_curve(double x, double y, int iterations);
float dragon_curve(double x, double y, int iterations);
float levy_curve(double x, double y, int iterations);
float gosper_curve(double x, double y, int iterations);
float cesaro_curve(double x, double y, int iterations);
float koch_snowflake(double x, double y, int iterations);
float sierpinski_arrowhead(double x, double y, int iterations);
float quadric_koch(double x, double y, int iterations);
float minkowski_sausage(double x, double y, int iterations);
float koch_anti_snowflake(double x, double y, int iterations);
float terdragon_curve(double x, double y, int iterations);
float koch_island(double x, double y, int iterations);
float heighway_dragon_variant(double x, double y, int iterations);
float snowflake_sweep(double x, double y, int iterations);
float dekking_curve(double x, double y, int iterations);
float gosper_island(double x, double y, int iterations);
float koch_quadratic(double x, double y, int iterations);

// Other fractal functions
float cantor_dust(double x, double y, int iterations);
float peano_curve(double x, double y, int iterations);
float hilbert_curve(double x, double y, int iterations);
float sierpinski_triangle(double x, double y, int iterations);
float box_fractal(double x, double y, int iterations);
float sierpinski_carpet(double x, double y, int iterations);
float cantor_ternary_grid(double x, double y, int iterations);
float moore_curve(double x, double y, int iterations);
float sierpinski_hexagon(double x, double y, int iterations);
float cantor_maze(double x, double y, int iterations);
float peano_meander_curve(double x, double y, int iterations);
float vicsek_fractal(double x, double y, int iterations);
float hexaflake(double x, double y, int iterations);
float cantor_square(double x, double y, int iterations);
float hilbert_variant(double x, double y, int iterations);
float sierpinski_pentagon(double x, double y, int iterations);
float cantor_cloud(double x, double y, int iterations);
float sierpinski_square(double x, double y, int iterations);

float koch_quadratic(double x, double y, int iterations);
float gosper_island(double x, double y, int iterations);
float snowflake_sweep(double x, double y, int iterations);
float dekking_curve(double x, double y, int iterations);
float heighway_dragon_variant(double x, double y, int iterations);
float koch_island(double x, double y, int iterations);
float terdragon_curve(double x, double y, int iterations);
float koch_anti_snowflake(double x, double y, int iterations);
float minkowski_sausage(double x, double y, int iterations);
float quadric_koch(double x, double y, int iterations);
float sierpinski_arrowhead(double x, double y, int iterations);
float koch_snowflake(double x, double y, int iterations);
float cesaro_curve(double x, double y, int iterations);
float gosper_curve(double x, double y, int iterations);
float koch_curve(double x, double y, int iterations);
float dragon_curve(double x, double y, int iterations);
float levy_curve(double x, double y, int iterations);

#endif
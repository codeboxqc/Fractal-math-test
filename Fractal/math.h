 

#ifndef MATH_H
#define MATH_H

#include <complex>
#include <vector>
#include <cmath>

float mandelbrot(double real, double imag);
std::vector<std::complex<double>> generate_koch_curve(int iterations);
float cantor_dust(double x, double y, int iterations);
std::vector<std::complex<double>> generate_dragon_curve(int iterations);
float sierpinski_triangle(double x, double y, int iterations);
float peano_curve(double x, double y, int iterations);
float hilbert_curve(double x, double y, int iterations);
float box_fractal(double x, double y, int iterations);
float sierpinski_carpet(double x, double y, int iterations);
std::vector<std::complex<double>> generate_levy_curve(int iterations);
std::vector<std::complex<double>> generate_dragon_curve(int iterations);
float cantor_ternary_grid(double x, double y, int iterations);
std::vector<std::complex<double>> generate_gosper_curve(int iterations);
std::vector<std::complex<double>> generate_cesaro_curve(int iterations);
std::vector<std::complex<double>> generate_koch_snowflake(int iterations);
std::vector<std::complex<double>> generate_sierpinski_arrowhead(int iterations);
std::vector<std::complex<double>> generate_quadric_koch(int iterations);

std::vector<std::complex<double>> generate_minkowski_sausage(int iterations);
float moore_curve(double x, double y, int iterations);
float sierpinski_hexagon(double x, double y, int iterations);
float cantor_maze(double x, double y, int iterations);
std::vector<std::complex<double>> generate_koch_anti_snowflake(int iterations);
float peano_meander_curve(double x, double y, int iterations);
float hexaflake(double x, double y, int iterations);
float vicsek_fractal(double x, double y, int iterations);
std::vector<std::complex<double>> generate_koch_island(int iterations);
std::vector<std::complex<double>> generate_terdragon_curve(int iterations);
float sierpinski_pentagon(double x, double y, int iterations);
float hilbert_variant(double x, double y, int iterations);
float cantor_square(double x, double y, int iterations);
std::vector<std::complex<double>> generate_snowflake_sweep(int iterations);
std::vector<std::complex<double>> generate_heighway_dragon_variant(int iterations);

float cantor_cloud(double x, double y, int iterations);
std::vector<std::complex<double>> generate_koch_quadratic(int iterations);
float sierpinski_square(double x, double y, int iterations);
std::vector<std::complex<double>> generate_gosper_island(int iterations);
std::vector<std::complex<double>> generate_dekking_curve(int iterations);


#endif
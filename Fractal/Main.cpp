#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include "math.h"
#include <fstream>

#include <complex>
 
#include <cmath>
#include <cstdlib> // For rand()
#include <limits>  // For numeric_limits
 
#include <algorithm> // For std::min and std::max

// Custom clamp function for C++14 or earlier
template <typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
	return std::min(std::max(v, lo), hi);
}


#define M_PI 3.14159265358979

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 780
#define THREAD_COUNT 4




enum FractalType {
	MANDELBROT, KOCH, SIERPINSKI_CARPET, CANTOR, DRAGON, PEANO, HILBERT,
	SIERPINSKI_TRIANGLE, BOX, LEVY, GOSPER, CESARO, CANTOR_TERNARY,
	KOCH_SNOWFLAKE, SIERPINSKI_ARROWHEAD, QUADRIC_KOCH, MINKOWSKI,
	MOORE, SIERPINSKI_HEXAGON, CANTOR_MAZE, KOCH_ANTI_SNOWFLAKE, PEANO_MEANDER,
	TERDRAGON, VICSEK, KOCH_ISLAND, HEXAFLAKE, HEIGHWAY_DRAGON, SNOWFLAKE_SWEEP,
	CANTOR_SQUARE, HILBERT_VARIANT, SIERPINSKI_PENTAGON, DEKKING, GOSPER_ISLAND,
	SIERPINSKI_SQUARE, KOCH_QUADRATIC, CANTOR_CLOUD
};

struct Viewport {
	double x_min = -2.0, x_max = 1.0;
	double y_min = -1.5, y_max = 1.5;
	double zoom = 1.0;
};


std::vector<std::vector<float>> pixel_data(WINDOW_HEIGHT, std::vector<float>(WINDOW_WIDTH, 0.0f));
// std::vector<float> pixel_data(WINDOW_WIDTH* WINDOW_HEIGHT, 0.0f);
 

FractalType current_fractal = MANDELBROT;
std::mutex mtx;

GLuint texture = 0;

void check_gl_error(const char* stage) {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cerr << "OpenGL Error at " << stage << ": " << err << std::endl;
	}
}

 
void compute_fractal(Viewport& view, FractalType type) {
	std::vector<std::thread> threads;
	int rows_per_thread = WINDOW_HEIGHT / THREAD_COUNT;


	 

	for (int t = 0; t < THREAD_COUNT; ++t) {
		threads.emplace_back([&, t]() {
			int start_row = t * rows_per_thread;
			int end_row = (t == THREAD_COUNT - 1) ? WINDOW_HEIGHT : start_row + rows_per_thread;

			for (int y = start_row; y < end_row; ++y) {
				for (int x = 0; x < WINDOW_WIDTH; ++x) {
					double real = view.x_min + (view.x_max - view.x_min) * x / WINDOW_WIDTH;
					double imag = view.y_min + (view.y_max - view.y_min) * y / WINDOW_HEIGHT;
					float value = 0.0f;
					int iterations = 6;

					switch (type) {

						 
		 

					case SIERPINSKI_CARPET: value = sierpinski_carpet(real, imag, iterations); break;
					case CANTOR: value = cantor_dust(real, imag, iterations); break;
					case PEANO: value = peano_curve(real, imag, iterations); break;
					case HILBERT: value = hilbert_curve(real, imag, iterations); break;
					case SIERPINSKI_TRIANGLE: value = sierpinski_triangle(real, imag, iterations); break;
					case BOX: value = box_fractal(real, imag, iterations); break;
					case CANTOR_TERNARY: value = cantor_ternary_grid(real, imag, iterations); break;
					case SIERPINSKI_HEXAGON: value = sierpinski_hexagon(real, imag, iterations); break;
					case CANTOR_MAZE: value = cantor_maze(real, imag, iterations); break;
					case PEANO_MEANDER: value = peano_meander_curve(real, imag, iterations); break;
					case VICSEK: value = vicsek_fractal(real, imag, iterations); break;
					case HEXAFLAKE: value = hexaflake(real, imag, iterations); break;
					case CANTOR_SQUARE: value = cantor_square(real, imag, iterations); break;
					case HILBERT_VARIANT: value = hilbert_variant(real, imag, iterations); break;
					case SIERPINSKI_PENTAGON: value = sierpinski_pentagon(real, imag, iterations); break;
					case CANTOR_CLOUD: value = cantor_cloud(real, imag, iterations); break;
					case MOORE: value = moore_curve(real, imag, iterations); break;
					case SIERPINSKI_SQUARE: value = sierpinski_square(real, imag, iterations); break;

 
 		 

					default: value = 0.1f; break;
					}


					value = clamp(value, 0.0f, 1.0f);


					// Debug: Log if value is out of range
					if (value < 0.0f || value > 1.0f) {
						std::lock_guard<std::mutex> lock(mtx);
						std::cerr << "Invalid value for fractal " << type << " at (" << real << ", " << imag << "): " << value << std::endl;
					}

					std::lock_guard<std::mutex> lock(mtx);
					if (y >= 0 && y < WINDOW_HEIGHT && x >= 0 && x < WINDOW_WIDTH) {
						pixel_data[y][x] = value;
					}

				}
			}
			});
	} 


	for (auto& thread : threads) {
		thread.join();
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RED, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	check_gl_error("texture setup");
}

GLuint compile_shader(const char* source, GLenum type) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "Shader compilation failed: " << infoLog << std::endl;
	}
	return shader;
}

GLuint create_shader_program(const char* vertex_source, const char* fragment_source) {
	GLuint vertex_shader = compile_shader(vertex_source, GL_VERTEX_SHADER);
	GLuint fragment_shader = compile_shader(fragment_source, GL_FRAGMENT_SHADER);
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cerr << "Program linking failed: " << infoLog << std::endl;
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	return program;
}


void render_line_fractal(GLuint shader_program, Viewport& view, FractalType type, GLuint vao, GLuint vbo) {
	std::vector<std::complex<double>> points;
	int iterations = 6; // Adjustable iterations



	switch (type) {
	case KOCH: points = generate_koch_curve(iterations); break;
	case DRAGON: points = generate_dragon_curve(iterations); break;
	case LEVY: points = generate_levy_curve(iterations); break;
	case GOSPER: points = generate_gosper_curve(iterations); break;
	case CESARO: points = generate_cesaro_curve(iterations); break;
	case KOCH_SNOWFLAKE: points = generate_koch_snowflake(iterations); break;
	case SIERPINSKI_ARROWHEAD: points = generate_sierpinski_arrowhead(iterations); break;
	case QUADRIC_KOCH: points = generate_quadric_koch(iterations); break;
	case MINKOWSKI: points = generate_minkowski_sausage(iterations); break;
	case TERDRAGON: points = generate_terdragon_curve(iterations); break;
	case KOCH_ISLAND: points = generate_koch_island(iterations); break;
	case HEIGHWAY_DRAGON: points = generate_heighway_dragon_variant(iterations); break;
	case SNOWFLAKE_SWEEP: points = generate_snowflake_sweep(iterations); break;
	case DEKKING: points = generate_dekking_curve(iterations); break;
	case GOSPER_ISLAND: points = generate_gosper_island(iterations); break;
	case KOCH_QUADRATIC: points = generate_koch_quadratic(iterations); break;
	case KOCH_ANTI_SNOWFLAKE: points = generate_koch_anti_snowflake(iterations); break;

	 
	default:
		std::cerr << "Unsupported fractal type for line rendering: " << type << std::endl;
		return;
	}


	 


	std::vector<float> vertices;
	for (const auto& p : points) {
		float x = static_cast<float>((p.real() - view.x_min) / (view.x_max - view.x_min) * 2.0 - 1.0);
		float y = static_cast<float>((p.imag() - view.y_min) / (view.y_max - view.y_min) * 2.0 - 1.0);
		vertices.push_back(x);
		vertices.push_back(y);
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shader_program);
	glUniform1i(glGetUniformLocation(shader_program, "useTexture"), 0);
	glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 2);
	glBindVertexArray(0);
	check_gl_error("line fractal render");
}


void render_line_fractal2(GLuint shader_program, Viewport& view, FractalType type, GLuint vao, GLuint vbo) {
	std::vector<std::complex<double>> points;
	if (type == KOCH) points = generate_koch_curve(4);

	std::vector<float> vertices;
	for (const auto& p : points) {
		float x = static_cast<float>((p.real() - view.x_min) / (view.x_max - view.x_min) * 2.0 - 1.0);
		float y = static_cast<float>((p.imag() - view.y_min) / (view.y_max - view.y_min) * 2.0 - 1.0);
		vertices.push_back(x);
		vertices.push_back(y);
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shader_program);
	glUniform1i(glGetUniformLocation(shader_program, "useTexture"), 0);
	glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 2);
	glBindVertexArray(0);
}

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_Window* window = SDL_CreateWindow(
		"Fractal Art",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);
	if (!window) {
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (!context) {
		std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RED, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	check_gl_error("texture setup");

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	check_gl_error("viewport setup");

	const char* vertex_shader_source = R"(
        #version 330 core
        layout(location = 0) in vec2 position;
        out vec2 fragCoord;
        void main() {
            gl_Position = vec4(position, 0.0, 1.0);
            fragCoord = position * 0.5 + 0.5;
        }
    )";
	const char* fragment_shader_source = R"(
    #version 330 core
    in vec2 fragCoord;
    out vec4 fragColor;
    uniform float maxIter;
    uniform int useTexture;
    uniform vec2 view_min;
    uniform vec2 view_max;
    uniform vec3 color;

    float mandelbrot(vec2 c) {
        vec2 z = vec2(0.0, 0.0);
        for (int i = 0; i < int(maxIter); i++) {
            z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
            if (dot(z, z) > 4.0) {
                return float(i) / maxIter;
            }
        }
        return 1.0;
    }

    void main() {
        if (useTexture == 1) {
            vec2 c = view_min + fragCoord * (view_max - view_min);
            float value = mandelbrot(c);
            fragColor = vec4(color * value, 1.0);
        } else {
            fragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red for lines
        }
    }
)";

	GLuint shader_program = create_shader_program(vertex_shader_source, fragment_shader_source);

	float quad_vertices[] = {
		-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f
	};
	GLuint quad_vao, quad_vbo;
	glGenVertexArrays(1, &quad_vao);
	glGenBuffers(1, &quad_vbo);
	glBindVertexArray(quad_vao);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	check_gl_error("quad setup");

 
	

	GLuint line_vao, line_vbo;
	glGenVertexArrays(1, &line_vao);
	glGenBuffers(1, &line_vbo);
	check_gl_error("line buffer setup");

	Viewport view;
	bool running = true;
	SDL_Event event;
	bool dragging = false;
	int mouse_x, mouse_y;
	int iterations = 100;
	float color[3] = { 1.0f, 1.0f, 1.0f };

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) running = false;
			if (event.type == SDL_KEYDOWN) {


				switch (event.key.keysym.sym) {


				case SDLK_1: current_fractal = MANDELBROT; view = { -2.0, 1.0, -1.5, 1.5, 1.0 }; break;
				case SDLK_2: current_fractal = KOCH; view = { 0.0, 1.0, -0.5, 0.5, 1.0 }; break;
				case SDLK_3: current_fractal = SIERPINSKI_CARPET; view = { 0.0, 0.6, 0.0, 0.5, 0.5 }; break;
				case SDLK_4: current_fractal = CANTOR; view = { 0.0, 0.1, 0.0, 0.1, 0.1 }; break;
				case SDLK_5: current_fractal = DRAGON; view = { -1.0, 1.0, -1.0, 1.0, 1.0 }; break;
				case SDLK_6: current_fractal = PEANO; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;
				case SDLK_7: current_fractal = HILBERT; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;
				case SDLK_8: current_fractal = SIERPINSKI_TRIANGLE; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;
				case SDLK_9: current_fractal = BOX; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;
				case SDLK_0: current_fractal = LEVY; view = { -1.0, 1.0, -1.0, 1.0, 1.0 }; break;
				case SDLK_q: current_fractal = GOSPER; view = { -1.0, 1.0, -1.0, 1.0, 1.0 }; break;
				case SDLK_w: current_fractal = CESARO; view = { -1.0, 1.0, -1.0, 1.0, 1.0 }; break;
				case SDLK_e: current_fractal = KOCH_SNOWFLAKE; view = { -0.5, 1.5, -0.5, 1.0, 1.0 }; break;
				case SDLK_r: current_fractal = SIERPINSKI_ARROWHEAD; view = { -0.5, 1.5, -0.5, 1.0, 1.0 }; break;
				case SDLK_t: current_fractal = QUADRIC_KOCH; view = { -0.5, 1.5, -0.5, 1.5, 1.0 }; break;
				case SDLK_y: current_fractal = MINKOWSKI; view = { -0.5, 1.5, -0.5, 1.0, 1.0 }; break;
				case SDLK_u: current_fractal = MOORE; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;
				case SDLK_i: current_fractal = SIERPINSKI_HEXAGON; view = { 0.0, 1.0, -0.5, 1.5, 1.0 }; break;
				case SDLK_o: current_fractal = CANTOR_MAZE; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;
				case SDLK_p: current_fractal = KOCH_ANTI_SNOWFLAKE; view = { -0.5, 1.5, -0.5, 1.0, 1.0 }; break;
				case SDLK_a: current_fractal = PEANO_MEANDER; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;
				case SDLK_s: current_fractal = TERDRAGON; view = { -1.0, 1.0, -1.0, 1.0, 1.0 }; break;
				case SDLK_d: current_fractal = VICSEK; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;
				case SDLK_f: current_fractal = KOCH_ISLAND; view = { -0.5, 1.5, -0.5, 1.5, 1.0 }; break;
				case SDLK_g: current_fractal = HEXAFLAKE; view = { 0.0, 1.0, -0.5, 1.5, 1.0 }; break;
				case SDLK_h: current_fractal = HEIGHWAY_DRAGON; view = { -1.0, 1.0, -1.0, 1.0, 1.0 }; break;
				case SDLK_j: current_fractal = SNOWFLAKE_SWEEP; view = { -0.5, 1.5, -0.5, 1.5, 1.0 }; break;
				case SDLK_k: current_fractal = CANTOR_SQUARE; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;
				case SDLK_l: current_fractal = HILBERT_VARIANT; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;
				case SDLK_z: current_fractal = SIERPINSKI_PENTAGON; view = { 0.0, 1.0, -0.5, 1.5, 1.0 }; break;
				case SDLK_x: current_fractal = DEKKING; view = { -1.0, 1.0, -1.0, 1.0, 1.0 }; break;
				case SDLK_c: current_fractal = GOSPER_ISLAND; view = { -0.5, 1.5, -0.5, 1.0, 1.0 }; break;
				case SDLK_v: current_fractal = KOCH_QUADRATIC; view = { -0.5, 1.5, -0.5, 1.0, 1.0 }; break;
				case SDLK_b: current_fractal = CANTOR_CLOUD; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;






				case SDLK_SPACE: view = { -2.0, 1.0, -1.5, 1.5, 1.0 }; iterations = 100; color[0] = 1.0f; color[1] = 1.0f; color[2] = 1.0f; break;


				 
				case SDLK_EXCLAIM: std::cout << "Commands:" << std::endl;
					std::cout << "1-9: Change fractal" << std::endl;
					std::cout << "Up/Down: Change iterations" << std::endl;
					std::cout << "r/g/b: Change color" << std::endl;
					std::cout << "Space: Reset" << std::endl;
					std::cout << "h: Help" << std::endl;
					std::cout << "q: Quit" << std::endl;
					break;
				case SDLK_COLON: {
					std::vector<unsigned char> data(WINDOW_WIDTH * WINDOW_HEIGHT * 3);
					glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, data.data());
					std::ofstream file("fractal.ppm", std::ios::out | std::ios::binary);
					file << "P6\n" << WINDOW_WIDTH << " " << WINDOW_HEIGHT << "\n255\n";
					file.write((char*)data.data(), data.size());
					file.close();
					std::cout << "Saved to fractal.ppm" << std::endl;
					break;
				}
				case SDLK_QUOTEDBL: {
					std::ifstream file("fractal.ppm", std::ios::in | std::ios::binary);
					if (file.is_open()) {
						std::string line;
						std::getline(file, line);
						std::getline(file, line);
						std::getline(file, line);
						std::vector<unsigned char> data(WINDOW_WIDTH * WINDOW_HEIGHT * 3);
						file.read((char*)data.data(), data.size());
						file.close();

						 
						
						std::vector<float> dummy(WINDOW_WIDTH * WINDOW_HEIGHT, 0.5f);
						glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RED, GL_FLOAT, dummy.data());


						glBindTexture(GL_TEXTURE_2D, texture);
						//glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RED, GL_FLOAT, nullptr);
						glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, data.data());
						std::cout << "Loaded from fractal.ppm" << std::endl;
					}
					else {
						std::cout << "Could not open fractal.ppm" << std::endl;
					}

					
					break;
				}
				case SDLK_PERCENT: {
					Uint32 fullscreen_flag = SDL_WINDOW_FULLSCREEN;
					bool is_fullscreen = SDL_GetWindowFlags(window) & fullscreen_flag;
					SDL_SetWindowFullscreen(window, is_fullscreen ? 0 : fullscreen_flag);
					break;
				}
				case SDLK_DOLLAR: {
					std::vector<unsigned char> data(WINDOW_WIDTH * WINDOW_HEIGHT * 3);
					glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, data.data());
					std::ofstream file("screenshot.ppm", std::ios::out | std::ios::binary);
					file << "P6\n" << WINDOW_WIDTH << " " << WINDOW_HEIGHT << "\n255\n";
					file.write((char*)data.data(), data.size());
					file.close();
					std::cout << "Saved to screenshot.ppm" << std::endl;
					break;
				}
				case SDLK_HASH: {
					auto start = std::chrono::high_resolution_clock::now();
					compute_fractal(view, current_fractal);
					auto end = std::chrono::high_resolution_clock::now();
					std::cout << "Time to compute fractal: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
					break;
				}
				case SDLK_ASTERISK: {
					std::cout << "Viewport: " << view.x_min << ", " << view.y_min << " -> " << view.x_max << ", " << view.y_max << std::endl;
					std::cout << "Iterations: " << iterations << std::endl;
					std::cout << "Color: " << color[0] << ", " << color[1] << ", " << color[2] << std::endl;
					break;
				}
				case SDLK_PLUS: {
					std::cout << "Fractal Art v1.0" << std::endl;
					break;
				}
				case SDLK_COMMA: {
					std::cout << "This program is licensed under the MIT License." << std::endl;
					break;
				}
				case SDLK_SLASH: {
					std::cout << "Credits:" << std::endl;
					std::cout << "Created by: Me" << std::endl;
					break;
				}
				 
				}
				
			}
			if (event.type == SDL_MOUSEWHEEL) {
				double zoom_factor = event.wheel.y > 0 ? 0.9 : 1.1;
				double mx = view.x_min + (view.x_max - view.x_min) * (event.wheel.x / (double)WINDOW_WIDTH);
				double my = view.y_min + (view.y_max - view.y_min) * (event.wheel.y / (double)WINDOW_HEIGHT);
				view.x_min = mx + (view.x_min - mx) * zoom_factor;
				view.x_max = mx + (view.x_max - mx) * zoom_factor;
				view.y_min = my + (view.y_min - my) * zoom_factor;
				view.y_max = my + (view.y_max - my) * zoom_factor;
				view.zoom *= zoom_factor;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				dragging = true;
				mouse_x = event.button.x;
				mouse_y = event.button.y;
			}
			if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
				dragging = false;
			}
			if (event.type == SDL_MOUSEMOTION && dragging) {
				double dx = (view.x_max - view.x_min) * (mouse_x - event.motion.x) / WINDOW_WIDTH;
				double dy = (view.y_max - view.y_min) * (event.motion.y - mouse_y) / WINDOW_HEIGHT;
				view.x_min += dx;
				view.x_max += dx;
				view.y_min += dy;
				view.y_max += dy;
				mouse_x = event.motion.x;
				mouse_y = event.motion.y;
			}
		}

		if (current_fractal == MANDELBROT) {
			glClear(GL_COLOR_BUFFER_BIT);
			glUseProgram(shader_program);
			glUniform1i(glGetUniformLocation(shader_program, "useTexture"), 1);
			glUniform1i(glGetUniformLocation(shader_program, "fractalType"), (int)current_fractal);
			glUniform1f(glGetUniformLocation(shader_program, "maxIter"), iterations);
			glUniform2f(glGetUniformLocation(shader_program, "view_min"), (float)view.x_min, (float)view.y_min);
			glUniform2f(glGetUniformLocation(shader_program, "view_max"), (float)view.x_max, (float)view.y_max);
			glUniform3fv(glGetUniformLocation(shader_program, "color"), 1, color);
			glBindVertexArray(quad_vao);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			check_gl_error("mandelbrot render");
		}
		else {
			bool is_pixel_fractal = current_fractal == SIERPINSKI_CARPET || current_fractal == CANTOR ||
				current_fractal == PEANO || current_fractal == HILBERT ||
				current_fractal == SIERPINSKI_TRIANGLE || current_fractal == BOX ||
				current_fractal == CANTOR_TERNARY || current_fractal == SIERPINSKI_HEXAGON ||
				current_fractal == CANTOR_MAZE || current_fractal == PEANO_MEANDER ||
				current_fractal == VICSEK || current_fractal == HEXAFLAKE ||
				current_fractal == CANTOR_SQUARE || current_fractal == HILBERT_VARIANT ||
				current_fractal == SIERPINSKI_PENTAGON || current_fractal == CANTOR_CLOUD ||
				current_fractal == MOORE || current_fractal == SIERPINSKI_SQUARE;
 

			if (is_pixel_fractal) {
				compute_fractal(view, current_fractal);
				glClear(GL_COLOR_BUFFER_BIT);
				glUseProgram(shader_program);
				glUniform1i(glGetUniformLocation(shader_program, "useTexture"), 1);
				glUniform1i(glGetUniformLocation(shader_program, "fractalType"), (int)current_fractal);
				glUniform1f(glGetUniformLocation(shader_program, "maxIter"), iterations);
				glUniform2f(glGetUniformLocation(shader_program, "view_min"), (float)view.x_min, (float)view.y_min);
				glUniform2f(glGetUniformLocation(shader_program, "view_max"), (float)view.x_max, (float)view.y_max);
				glUniform3fv(glGetUniformLocation(shader_program, "color"), 1, color);
				glActiveTexture(GL_TEXTURE0); // Ensure texture unit 0 is active
				glBindTexture(GL_TEXTURE_2D, texture);
				glUniform1i(glGetUniformLocation(shader_program, "textureSampler"), 0); // Bind texture to sampler
				glBindVertexArray(quad_vao);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				check_gl_error("pixel fractal render");
			}
			else {
				render_line_fractal(shader_program, view, current_fractal, line_vao, line_vbo);
			}
		}

		SDL_GL_SwapWindow(window);
	}

	glDeleteTextures(1, &texture);
	glDeleteVertexArrays(1, &quad_vao);
	glDeleteBuffers(1, &quad_vbo);
	glDeleteVertexArrays(1, &line_vao);
	glDeleteBuffers(1, &line_vbo);
	glDeleteProgram(shader_program);
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}






 ////////////////////////////////////



/*
// --- CORE OPENGL LIBRARIES ---
#include <glad/glad.h>   // MUST BE FIRST! This defines all gl* functions.
#include <GLFW/glfw3.h>  // Second, as it uses GLAD's definitions for context creation.

// --- OTHER LIBRARIES ---
#include <glm/glm.hpp>   // Your math library
#include <iostream>      // Standard C++
#include <vector>
#include <thread>
#include <mutex>

// THESE ARE THE PROBLEM CHILD HEADERS
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "math.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define THREAD_COUNT 4
*/

/*
// --- CORE OPENGL LIBRARIES ---
#include <glad/glad.h>   // MUST BE FIRST! This defines all gl* functions.
#include <GLFW/glfw3.h>  // Second, as it uses GLAD's definitions for context creation.

// --- OTHER LIBRARIES ---
#include <glm/glm.hpp>   // Your math library
#include <iostream>      // Standard C++
#include <vector>
#include <thread>
#include <mutex>

// THESE ARE THE PROBLEM CHILD HEADERS
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "math.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define THREAD_COUNT 4
*/

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

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
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
FractalType current_fractal = MANDELBROT;
std::mutex mtx;

void check_gl_error(const char* stage) {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cerr << "OpenGL Error at " << stage << ": " << err << std::endl;
	}
}

void compute_fractal(Viewport& view, FractalType type) {
	// This is now handled by the shader
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

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RED, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	check_gl_error("texture setup");

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
				case SDLK_3: current_fractal = SIERPINSKI_CARPET; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;
				case SDLK_4: current_fractal = CANTOR; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;
				case SDLK_5: current_fractal = DRAGON; view = { -1.0, 1.0, -1.0, 1.0, 1.0 }; break;
				case SDLK_6: current_fractal = PEANO; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;
				case SDLK_7: current_fractal = HILBERT; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;
				case SDLK_8: current_fractal = SIERPINSKI_TRIANGLE; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;
				case SDLK_9: current_fractal = BOX; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;
				case SDLK_0: current_fractal = LEVY; view = { 0.0, 1.0, 0.0, 1.0, 1.0 }; break;
				case SDLK_UP: iterations += 10; break;
				case SDLK_DOWN: iterations -= 10; break;
				case SDLK_r: color[0] += 0.1f; break;
				case SDLK_g: color[1] += 0.1f; break;
				case SDLK_b: color[2] += 0.1f; break;
				case SDLK_SPACE: view = { -2.0, 1.0, -1.5, 1.5, 1.0 }; iterations = 100; color[0] = 1.0f; color[1] = 1.0f; color[2] = 1.0f; break;
				case SDLK_h: std::cout << "Commands:" << std::endl;
					std::cout << "1-9: Change fractal" << std::endl;
					std::cout << "Up/Down: Change iterations" << std::endl;
					std::cout << "r/g/b: Change color" << std::endl;
					std::cout << "Space: Reset" << std::endl;
					std::cout << "h: Help" << std::endl;
					std::cout << "q: Quit" << std::endl;
					break;
				case SDLK_s: {
					std::vector<unsigned char> data(WINDOW_WIDTH * WINDOW_HEIGHT * 3);
					glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, data.data());
					std::ofstream file("fractal.ppm", std::ios::out | std::ios::binary);
					file << "P6\n" << WINDOW_WIDTH << " " << WINDOW_HEIGHT << "\n255\n";
					file.write((char*)data.data(), data.size());
					file.close();
					std::cout << "Saved to fractal.ppm" << std::endl;
					break;
				}
				case SDLK_l: {
					std::ifstream file("fractal.ppm", std::ios::in | std::ios::binary);
					if (file.is_open()) {
						std::string line;
						std::getline(file, line);
						std::getline(file, line);
						std::getline(file, line);
						std::vector<unsigned char> data(WINDOW_WIDTH * WINDOW_HEIGHT * 3);
						file.read((char*)data.data(), data.size());
						file.close();
						glBindTexture(GL_TEXTURE_2D, texture);
						glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, data.data());
						std::cout << "Loaded from fractal.ppm" << std::endl;
					}
					else {
						std::cout << "Could not open fractal.ppm" << std::endl;
					}
					break;
				}
				case SDLK_f: {
					Uint32 fullscreen_flag = SDL_WINDOW_FULLSCREEN;
					bool is_fullscreen = SDL_GetWindowFlags(window) & fullscreen_flag;
					SDL_SetWindowFullscreen(window, is_fullscreen ? 0 : fullscreen_flag);
					break;
				}
				case SDLK_p: {
					std::vector<unsigned char> data(WINDOW_WIDTH * WINDOW_HEIGHT * 3);
					glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, data.data());
					std::ofstream file("screenshot.ppm", std::ios::out | std::ios::binary);
					file << "P6\n" << WINDOW_WIDTH << " " << WINDOW_HEIGHT << "\n255\n";
					file.write((char*)data.data(), data.size());
					file.close();
					std::cout << "Saved to screenshot.ppm" << std::endl;
					break;
				}
				case SDLK_o: {
					auto start = std::chrono::high_resolution_clock::now();
					compute_fractal(view, current_fractal);
					auto end = std::chrono::high_resolution_clock::now();
					std::cout << "Time to compute fractal: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
					break;
				}
				case SDLK_d: {
					std::cout << "Viewport: " << view.x_min << ", " << view.y_min << " -> " << view.x_max << ", " << view.y_max << std::endl;
					std::cout << "Iterations: " << iterations << std::endl;
					std::cout << "Color: " << color[0] << ", " << color[1] << ", " << color[2] << std::endl;
					break;
				}
				case SDLK_v: {
					std::cout << "Fractal Art v1.0" << std::endl;
					break;
				}
				case SDLK_c: {
					std::cout << "This program is licensed under the MIT License." << std::endl;
					break;
				}
				case SDLK_x: {
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
			glUniform1f(glGetUniformLocation(shader_program, "maxIter"), iterations);

			glUniform2f(glGetUniformLocation(shader_program, "view_min"), (float)view.x_min, (float)view.y_min);
			glUniform2f(glGetUniformLocation(shader_program, "view_max"), (float)view.x_max, (float)view.y_max);

			glUniform3fv(glGetUniformLocation(shader_program, "color"), 1, color);
			glBindVertexArray(quad_vao);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			check_gl_error("pixel fractal render");
		}
		else {
			render_line_fractal(shader_program, view, current_fractal, line_vao, line_vbo);
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






/// /////////////////////////////////////


// Vertex Shader Source
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main() {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";

// Fragment Shader Source (simple red color)
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f); // Solid Red
    }
)";


int main1(int argc, char* argv[]) {
    // 1. Initialize SDL Video Subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 2. Set OpenGL Context Attributes (requesting OpenGL 3.3 Core Profile)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // Optional: Depth buffer

    // 3. Create an SDL Window with OpenGL flag
    SDL_Window* window = SDL_CreateWindow(
        "Minimal SDL2 + GLAD OpenGL Test", // Window title
        SDL_WINDOWPOS_UNDEFINED,           // Initial x position
        SDL_WINDOWPOS_UNDEFINED,           // Initial y position
        WINDOW_WIDTH,                      // Width in pixels
        WINDOW_HEIGHT,                     // Height in pixels
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN // Flags: Enable OpenGL, make window visible
    );

    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // 4. Create an OpenGL Context and make it current
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_GL_MakeCurrent(window, gl_context);

    // 5. Initialize GLAD - THIS IS THE CRUCIAL STEP FOR MODERN OPENGL FUNCTIONS!
    // GLAD uses SDL_GL_GetProcAddress to load the function pointers from the driver.
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Print OpenGL version (now it should show your actual version)
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // 6. NOW you can use modern OpenGL functions like glCreateShader, glGenVertexArrays, etc.

    // Compile and link shaders (minimal example)
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set up a basic VAO and VBO for a simple triangle (modern way)
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
         0.5f, -0.5f, 0.0f, // right
         0.0f,  0.5f, 0.0f  // top
    };
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
    glBindVertexArray(0); // Unbind VAO

    // 7. Main loop
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // 8. OpenGL Rendering commands (using modern pipeline)
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Dark background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram); // Activate your shader program
        glBindVertexArray(VAO);      // Bind the VAO
        glDrawArrays(GL_TRIANGLES, 0, 3); // Draw the triangle

        // 9. Swap buffers
        SDL_GL_SwapWindow(window);
    }

    // 10. Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();


    std::cout << "Minimal SDL2 + GLAD OpenGL test finished successfully." << std::endl;

    return 0;
}
//////////////////////////////////
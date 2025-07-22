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
 
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 600

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

void check_gl_error(const char* stage) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Error at " << stage << ": " << err << std::endl << std::flush;
    }
}

void compute_fractal(int start_y, int end_y, Viewport& view, FractalType type) {
    std::cerr << "Computing fractal: " << type << " from y=" << start_y << " to " << end_y << std::endl << std::flush;
    if (type == MANDELBROT) {
        for (int y = start_y; y < end_y; ++y) {
            for (int x = 0; x < WINDOW_WIDTH; ++x) {
                double real = view.x_min + (view.x_max - view.x_min) * x / WINDOW_WIDTH;
                double imag = view.y_min + (view.y_max - view.y_min) * y / WINDOW_HEIGHT;
                pixel_data[y][x] = mandelbrot(real, imag);
            }
        }
    }
}

GLuint compile_shader(const char* source, GLenum type) {
    std::cerr << "Compiling shader type " << type << std::endl << std::flush;
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl << std::flush;
    }
    check_gl_error("compile_shader");
    return shader;
}

GLuint create_shader_program(const char* vertex_source, const char* fragment_source) {
    std::cerr << "Creating shader program" << std::endl << std::flush;
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
        std::cerr << "Program linking failed: " << infoLog << std::endl << std::flush;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    check_gl_error("create_shader_program");
    return program;
}

void render_line_fractal(GLuint shader_program, Viewport& view, FractalType type, GLuint vao, GLuint vbo) {
    std::cerr << "Rendering line fractal: " << type << std::endl << std::flush;
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
    check_gl_error("render_line_fractal");
}

int main(int argc, char* argv[]) {
    std::cerr << "Starting program" << std::endl << std::flush;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl << std::flush;
        return 1;
    }
    std::cerr << "SDL initialized" << std::endl << std::flush;

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
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl << std::flush;
        SDL_Quit();
        return 1;
    }
    std::cerr << "Window created" << std::endl << std::flush;

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl << std::flush;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    std::cerr << "OpenGL context created" << std::endl << std::flush;

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl << std::flush;
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    std::cerr << "GLAD initialized" << std::endl << std::flush;

    std::cerr << "OpenGL Version: " << (glGetString(GL_VERSION) ? (const char*)glGetString(GL_VERSION) : "unknown") << std::endl;
    std::cerr << "OpenGL Vendor: " << (glGetString(GL_VENDOR) ? (const char*)glGetString(GL_VENDOR) : "unknown") << std::endl << std::flush;
    check_gl_error("GLAD initialization");

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
        uniform sampler2D pixelTexture;
        uniform float maxIter;
        uniform int useTexture;
        void main() {
            if (useTexture == 1) {
                float value = texture(pixelTexture, fragCoord).r;
                float smoothValue = value / maxIter;
                vec3 color = vec3(smoothValue, sin(smoothValue * 6.28), cos(smoothValue * 6.28));
                fragColor = vec4(color, 1.0);
            } else {
                fragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red for lines
            }
        }
    )";
    GLuint shader_program = create_shader_program(vertex_shader_source, fragment_shader_source);
    std::cerr << "Shader Program ID: " << shader_program << std::endl << std::flush;

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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (running) {
        std::cerr << "Main loop iteration" << std::endl << std::flush;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_1: current_fractal = MANDELBROT; view = { -2.0, 1.0, -1.5, 1.5, 1.0 }; break;
                case SDLK_2: current_fractal = KOCH; view = { 0.0, 1.0, -0.5, 0.5, 1.0 }; break;
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
            compute_fractal(0, WINDOW_HEIGHT, view, current_fractal);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RED, GL_FLOAT, pixel_data[0].data());
            check_gl_error("texture update");

            glClear(GL_COLOR_BUFFER_BIT);
            glUseProgram(shader_program);
            glUniform1i(glGetUniformLocation(shader_program, "useTexture"), 1);
            glUniform1f(glGetUniformLocation(shader_program, "maxIter"), 100.0f);
            glBindVertexArray(quad_vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            check_gl_error("pixel fractal render");
        }
        else {
            render_line_fractal(shader_program, view, current_fractal, line_vao, line_vbo);
        }

        SDL_GL_SwapWindow(window);
        std::cerr << "Swapped window" << std::endl << std::flush;
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
    std::cerr << "Program exited" << std::endl << std::flush;
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
/**
* Author: Raymond Lin
* Assignment: Platformer
* Date due: 2023-12-02, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION
#define LOG(argument) std::cout << argument << '\n'
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"
#include "cmath"
#include <ctime>
#include <vector>
#include "Entity.h"
#include "Map.h"
#include "Utility.h"
#include "Scene.h"
#include "scenes/StartScreen.h"
#include "scenes/town.h"
#include "scenes/dungeon.h"
//#include "scenes/LevelC.h"

// ————— CONSTANTS ————— //
const int   WINDOW_WIDTH = 800,
            WINDOW_HEIGHT = 600;

const int   VIEWPORT_X = 0,
            VIEWPORT_Y = 0,
            VIEWPORT_WIDTH = WINDOW_WIDTH,
            VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char GAME_WINDOW_NAME[] = "Knight's Descent";

const char  V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
            F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

const char V_LIT_SHADER_PATH[] = "shaders/vertex_lit.glsl",
           F_LIT_SHADER_PATH[] = "shaders/fragment_lit.glsl";

const float MILLISECONDS_IN_SECOND = 1000.0;

const int NUMBER_OF_TEXTURES = 1;
const GLint LEVEL_OF_DETAIL = 0;
const GLint TEXTURE_BORDER = 0;

GLuint font_texture_id;

// ————— GLOBAL VARIABLES ————— //
Scene* g_current_scene;
Town* g_town_level;
Dungeon* g_dungeon_level;
//LevelC* g_level_c;
StartScreen* g_start_screen;

std::vector<Scene*> g_levels;

SDL_Window* g_display_window;
bool g_game_is_running = true,
     g_win = false,
     g_lose = false;

ShaderProgram g_shader_program;
glm::mat4 g_view_matrix, g_projection_matrix;

float   g_previous_ticks = 0.0f,
        g_accumulator = 0.0f;

int g_number_of_lives = 3;

void switch_to_scene(Scene* scene)
{
    if (scene == g_town_level || scene == g_start_screen) { g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH); }
    else { g_shader_program.load(V_LIT_SHADER_PATH, F_LIT_SHADER_PATH); }

    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-7.0f, 7.0f, -5.25f, 5.25f, -1.0f, 1.0f);
    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);
    glUseProgram(g_shader_program.get_program_id());
    glClearColor(scene->BG_RED, scene->BG_GREEN, scene->BG_BLUE, scene->BG_OPACITY);

    g_current_scene = scene;
    g_current_scene->initialise();
}

void start_attack_animation(int m_facing)
{
    g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_attack[g_current_scene->m_state.player->m_facing];
}

void initialise()
{
    // ————— GENERAL ————— //
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    g_display_window = SDL_CreateWindow(GAME_WINDOW_NAME,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    // ————— VIDEO SETUP ————— //
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);

    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-7.0f, 7.0f, -5.25f, 5.25f, -1.0f, 1.0f);

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);

    glUseProgram(g_shader_program.get_program_id());

    // ————— LEVEL SETUP ————— //
    g_start_screen = new StartScreen();
    g_start_screen->BG_RED = 20.0f / 255.0f;
    g_start_screen->BG_GREEN = 20.0f / 255.0f;
    g_start_screen->BG_BLUE = 20.0f / 255.0f;
    g_start_screen->BG_OPACITY = 1.0f;

    g_town_level = new Town();
    g_town_level->BG_RED = 132.0f / 255.0f;
    g_town_level->BG_GREEN = 198.0f / 255.0f;
    g_town_level->BG_BLUE = 105.0f / 255.0f;
    g_town_level->BG_OPACITY = 1.0f;
    g_town_level->m_state.next_scene_id = 1;

    g_dungeon_level = new Dungeon();
    g_dungeon_level->BG_RED = 20.0f / 255.0f;
    g_dungeon_level->BG_GREEN = 14.0f / 255.0f;
    g_dungeon_level->BG_BLUE = 9.0f / 255.0f;
    g_dungeon_level->BG_OPACITY = 1.0f;
    g_dungeon_level->m_state.next_scene_id = 2;


    /*
    g_level_c = new LevelC();
    g_level_c->BG_RED = 158.0f / 255.0f;
    g_level_c->BG_GREEN = 213.0f / 255.0f;
    g_level_c->BG_BLUE = 222.0f / 255.0f;
    g_level_c->BG_OPACITY = 1.0f;
    */

    g_levels.push_back(g_town_level);
    g_levels.push_back(g_dungeon_level);
    //g_levels.push_back(g_level_c);

    switch_to_scene(g_start_screen);

    // ————— TEXT ————— //
    font_texture_id = Utility::load_texture("assets/fonts/font1.png");

    // ————— BLENDING ————— //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void process_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            g_game_is_running = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_q:
                // Quit the game with a keystroke
                g_game_is_running = false;
                break;

            case SDLK_SPACE:
                // Jump
                if (g_current_scene == g_start_screen) break;

                g_current_scene->m_state.player->m_is_attacking = true;
                start_attack_animation(g_current_scene->m_state.player->m_facing);
                break;

            case SDLK_RETURN:
                if (g_current_scene == g_start_screen) switch_to_scene(g_town_level);
                break;

            default:
                break;
            }

        default:
            break;
        }
    }

    if (g_current_scene != g_start_screen)
    {
        g_current_scene->m_state.player->set_movement(glm::vec3(0.0f));

        const Uint8* key_state = SDL_GetKeyboardState(NULL);

        if (key_state[SDL_SCANCODE_UP] || key_state[SDL_SCANCODE_W])
        {
            g_current_scene->m_state.player->move_up();
            g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->UP];
            g_current_scene->m_state.player->m_facing = g_current_scene->m_state.player->UP;
            g_current_scene->m_state.player->m_is_attacking = false;
            g_current_scene->m_state.player->weapon->deactivate();
        }
        else if (key_state[SDL_SCANCODE_DOWN] || key_state[SDL_SCANCODE_S] && g_current_scene->m_state.player->get_position().y > -(g_current_scene->m_state.map->get_height() - 1.0f))
        {
            g_current_scene->m_state.player->move_down();
            g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->DOWN];
            g_current_scene->m_state.player->m_facing = g_current_scene->m_state.player->DOWN;
            g_current_scene->m_state.player->m_is_attacking = false;
            g_current_scene->m_state.player->weapon->deactivate();
        }
        else if (key_state[SDL_SCANCODE_LEFT] || key_state[SDL_SCANCODE_A])
        {
            g_current_scene->m_state.player->move_left();
            g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->LEFT];
            g_current_scene->m_state.player->m_facing = g_current_scene->m_state.player->LEFT;
            g_current_scene->m_state.player->m_is_attacking = false;
            g_current_scene->m_state.player->weapon->deactivate();
        }
        else if (key_state[SDL_SCANCODE_RIGHT] || key_state[SDL_SCANCODE_D])
        {
            g_current_scene->m_state.player->move_right();
            g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->RIGHT];
            g_current_scene->m_state.player->m_facing = g_current_scene->m_state.player->RIGHT;
            g_current_scene->m_state.player->m_is_attacking = false;
            g_current_scene->m_state.player->weapon->deactivate();
        }

        // This makes sure that the player can't move faster diagonally
        if (glm::length(g_current_scene->m_state.player->get_movement()) > 1.0f)
        {
            g_current_scene->m_state.player->set_movement(glm::normalize(g_current_scene->m_state.player->get_movement()));
        }
    }

}

void game_loop(float delta_time)
{
    while (delta_time >= FIXED_TIMESTEP)
    {
        g_current_scene->update(FIXED_TIMESTEP);

        delta_time -= FIXED_TIMESTEP;
    }

    g_accumulator = delta_time;

    // ————— PLAYER CAMERA ————— //
    float camera_x;
    float camera_y;

    if (g_current_scene->m_state.player->get_position().x < 7.5f)
    {
        camera_x = -7.5f;
    }
    else if (g_current_scene->m_state.player->get_position().x > g_current_scene->m_state.map->get_width() - 7.5f)
    {
        camera_x = -(g_current_scene->m_state.map->get_width() - 7.5f);
    }
    else
    {
        camera_x = -g_current_scene->m_state.player->get_position().x;
    }

    if (g_current_scene->m_state.player->get_position().y > -5.0f)
    {
        camera_y = 5.0f;
    }
    else if (g_current_scene->m_state.player->get_position().y < -(g_current_scene->m_state.map->get_height() - 6.0f))
    {
        camera_y = (g_current_scene->m_state.map->get_height() - 6.0f);
    }
    else
    {
        camera_y = -g_current_scene->m_state.player->get_position().y;
    }

    g_view_matrix = glm::mat4(1.0f);


    g_view_matrix = glm::translate(g_view_matrix, glm::vec3(camera_x, camera_y, 0.0f));

    // ————— NEXT LEVEL ————— //
    if (g_current_scene->m_state.player->m_next_level)
    {
        switch_to_scene(g_levels[g_current_scene->m_state.next_scene_id]);
    }

    if (g_current_scene->m_state.player->m_player_dead) 
    {
        --g_number_of_lives;
        Mix_PlayChannel(-1, g_current_scene->m_state.sfx[g_current_scene->DEATH_SFX], 0);
        if (g_number_of_lives > 0) g_current_scene->initialise();
    }

    if (g_number_of_lives == 0) g_lose = true;
    if (g_current_scene->m_state.player->m_player_win) g_win = true;

}

void update()
{
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;

    delta_time += g_accumulator;

    if (delta_time < FIXED_TIMESTEP)
    {
        g_accumulator = delta_time;
        return;
    }

    if (!g_win && !g_lose && g_current_scene != g_start_screen) game_loop(delta_time); 
    
}

void render()
{
    g_shader_program.set_view_matrix(g_view_matrix);
    if (g_current_scene == g_dungeon_level) g_shader_program.set_light_position(g_current_scene->m_state.player->get_position());
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(g_shader_program.get_program_id());
    g_current_scene->render(&g_shader_program);

    if (g_win) Utility::draw_text(&g_shader_program, font_texture_id, "YOU WIN!", 0.4f, 0.0f, glm::vec3(g_current_scene->m_state.player->get_position().x + 1.5f, g_current_scene->m_state.player->get_position().y - 2.0f, 0.0f));
    if (g_lose) Utility::draw_text(&g_shader_program, font_texture_id, "YOU LOSE!", 0.5f, 0.0f, glm::vec3(g_current_scene->m_state.player->get_position().x - 2.0f, g_current_scene->m_state.player->get_position().y + 2.0f, 0.0f));

    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();
    delete g_start_screen;
    delete g_town_level;
    delete g_dungeon_level;
    //delete g_level_c;
}

// ————— GAME LOOP ————— //
int main(int argc, char* argv[])
{
    initialise();

    while (g_game_is_running)
    {
        process_input();
        update();
        render();
    }

    shutdown();
    return 0;
}

/**
* Author: Raymond Lin
* Assignment: Platformer
* Date due: 2023-12-02, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "StartScreen.h"
#include "../Utility.h"

GLuint text_texture_id;

StartScreen::~StartScreen()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
    Mix_FreeChunk(m_state.pop_sfx);
}

void StartScreen::initialise()
{
    text_texture_id = Utility::load_texture("assets/fonts/font1.png");
}

void StartScreen::update(float delta_time)
{

}


void StartScreen::render(ShaderProgram *program)
{
    Utility::draw_text(program, text_texture_id, "Press Enter to Start", 0.4f, 0.0f, glm::vec3(-3.75f, 1.0f, 0.0f));
}

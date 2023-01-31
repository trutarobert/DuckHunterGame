#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"
#include <random>
#include <time.h>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


tema1::tema1()
{
}


tema1::~tema1()
{
}


void tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);


    Mesh* body = object2D::DrawTriangle("body", glm::vec3(0.6, 0.3, 0));
    AddMeshToList(body);
    Mesh* head = object2D::DrawCircle("head", glm::vec3(0, 0.5, 0.2));
    AddMeshToList(head);
    Mesh* wing1 = object2D::DrawTriangle("wing1", glm::vec3(0.6, 0.3, 0));
    AddMeshToList(wing1);
    Mesh* wing2 = object2D::DrawTriangle("wing2", glm::vec3(0.6, 0.3, 0));
    AddMeshToList(wing2);
    Mesh* beak = object2D::DrawTriangle("beak", glm::vec3(0.8, 0.8, 0));
    AddMeshToList(beak);

    Mesh* escape = object2D::DrawCircle("escape", glm::vec3(1, 0, 0));
    AddMeshToList(escape);
    Mesh* dead = object2D::DrawCircle("dead", glm::vec3(0, 0, 1));
    AddMeshToList(dead);
    Mesh* bullet = object2D::DrawRectangle("bullet", glm::vec3(0, 1, 0), true, 0.0f);
    AddMeshToList(bullet);
    Mesh* border = object2D::DrawRectangle("border", glm::vec3(0.6, 0.6, 0.6), false, 0.0f);
    AddMeshToList(border);
    Mesh* score = object2D::DrawRectangle("score", glm::vec3(0, 0, 1), true, 0.0f);
    AddMeshToList(score);
    Mesh* sky = object2D::DrawRectangle("sky", glm::vec3(0.6, 0.8, 1), true, 0.0f);
    AddMeshToList(sky);
    Mesh* grass = object2D::DrawRectangle("grass", glm::vec3(0.4, 0.8, 0), true, 0.1f);
    AddMeshToList(grass);
    Mesh* win = object2D::DrawWin("win", glm::vec3(0, 0, 1));
    AddMeshToList(win);
    Mesh* lose = object2D::DrawLose("lose", glm::vec3(1, 0, 0));
    AddMeshToList(lose);

    srand(time(NULL));
    scaleX = scaleY = 20;
    speed = 100;
    flap = 0;
    flap_dir = 1;
    fly_time = 0;
    shot = false;
    lifes = 3;
    amo = 3;
    ducks = 1;
    killed = 0;

    translateX = rand() % 1280;
    translateY = rand() % 300;
    dir = ((float)(rand() % 120)) / 100;
    if (rand() % 2 == 0)
        dir *= -1;
}


void tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void tema1::RenderScene() {
    headMatrix = glm::mat3(1);
    headMatrix *= transform2D::Translate(translateX, translateY);
    headMatrix *= transform2D::Rotate(6.28 - dir);
    headMatrix *= transform2D::Scale(scaleX, scaleY);
    if (fly_time > 10)
        RenderMesh2D(meshes["escape"], shaders["VertexColor"], headMatrix);
    else if (shot)
        RenderMesh2D(meshes["dead"], shaders["VertexColor"], headMatrix);
    else
        RenderMesh2D(meshes["head"], shaders["VertexColor"], headMatrix);

    glm::mat3 bodyMatrix = headMatrix * transform2D::Translate(0, -2);
    RenderMesh2D(meshes["body"], shaders["VertexColor"], bodyMatrix);
    
    glm::mat3 wingMatrix = bodyMatrix * transform2D::Translate(-2, -1);
    wingMatrix *= transform2D::Rotate(flap);
    wingMatrix *= transform2D::Rotate(1.8);
    wingMatrix *= transform2D::Scale(0.7, 0.7);
    RenderMesh2D(meshes["wing1"], shaders["VertexColor"], wingMatrix);

    wingMatrix = bodyMatrix * transform2D::Translate(2, -1);
    wingMatrix *= transform2D::Rotate(-flap);
    wingMatrix *= transform2D::Rotate(-1.8);
    wingMatrix *= transform2D::Scale(0.7, 0.7);
    RenderMesh2D(meshes["wing2"], shaders["VertexColor"], wingMatrix);

    glm::mat3 beakMatrix = headMatrix * transform2D::Translate(0, 1.5);
    beakMatrix *= transform2D::Scale(0.3, 0.3);
    RenderMesh2D(meshes["beak"], shaders["VertexColor"], beakMatrix);

    glm::mat3 lifesMatrix = glm::mat3(1);
    if (lifes >= 1) {
        lifesMatrix *= transform2D::Translate(1250, 690);
        lifesMatrix *= transform2D::Scale(15, 15);
        RenderMesh2D(meshes["escape"], shaders["VertexColor"], lifesMatrix);
    }
    if (lifes >= 2) {
        lifesMatrix *= transform2D::Translate(-3, 0);
        RenderMesh2D(meshes["escape"], shaders["VertexColor"], lifesMatrix);
    }
    if (lifes >= 3) {
        lifesMatrix *= transform2D::Translate(-3, 0);
        RenderMesh2D(meshes["escape"], shaders["VertexColor"], lifesMatrix);
    }

    glm::mat3 amoMatrix = glm::mat3(1);
    if (amo >= 1) {
        amoMatrix *= transform2D::Translate(1250, 640);
        amoMatrix *= transform2D::Scale(10, 10);
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], amoMatrix);
    }
    if (amo >= 2) {
        amoMatrix *= transform2D::Translate(-4.5, 0);
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], amoMatrix);
    }
    if (amo >= 3) {
        amoMatrix *= transform2D::Translate(-4.5, 0);
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], amoMatrix);
    }

    glm::mat3 borderMatrix = glm::mat3(1);
    borderMatrix *= transform2D::Translate(1200, 580);
    borderMatrix *= transform2D::Rotate(1.57);
    borderMatrix *= transform2D::Scale(20, 35);
    RenderMesh2D(meshes["border"], shaders["VertexColor"], borderMatrix);

    glm::mat3 scoreMatrix = glm::mat3(1);
    scoreMatrix *= transform2D::Translate(1135, 580);
    scoreMatrix *= transform2D::Scale(7, 10);
    for (int i = 0; i < killed; i++) {
        RenderMesh2D(meshes["score"], shaders["VertexColor"], scoreMatrix);
        scoreMatrix *= transform2D::Translate(2, 0);
    }

    glm::mat3 grassMatrix = glm::mat3(1);
    grassMatrix *= transform2D::Translate(600, 80);
    grassMatrix *= transform2D::Rotate(1.57);
    grassMatrix *= transform2D::Scale(80, 400);
    RenderMesh2D(meshes["grass"], shaders["VertexColor"], grassMatrix);

    glm::mat3 skyMatrix = glm::mat3(1);
    skyMatrix *= transform2D::Translate(600, 350);
    skyMatrix *= transform2D::Rotate(1.57);
    skyMatrix *= transform2D::Scale(400, 400);
    RenderMesh2D(meshes["sky"], shaders["VertexColor"], skyMatrix);
}
void tema1::Shooted()
{
    fly_time = 0;
    shot = false;
    amo = 3;
    killed++;
    translateX = rand() % 1280;
    translateY = rand() % 300;
    dir = ((float)(rand() % 120)) / 100;
    if (rand() % 2 == 0)
        dir *= -1;
    if (ducks % 3 == 0)
        speed += 100;
    ducks++;
}
void tema1::Evaded()
{
    fly_time = 0;
    amo = 3;
    lifes--;
    translateX = rand() % 1280;
    translateY = rand() % 300;
    dir = ((float)(rand() % 120)) / 100;
    if (rand() % 2 == 0)
        dir *= -1;
    if (ducks % 3 == 0)
        speed += 100;
    ducks++;
}
void::tema1::RenderEnd()
{
    glm::mat3 transMatrix = glm::mat3(1);
    transMatrix *= transform2D::Translate(530, 430);
    transMatrix *= transform2D::Scale(60, 60);
    if(lifes > 0)
        RenderMesh2D(meshes["win"], shaders["VertexColor"], transMatrix);
    else
        RenderMesh2D(meshes["lose"], shaders["VertexColor"], transMatrix);
}
void tema1::Update(float deltaTimeSeconds)
{
    fly_time += deltaTimeSeconds;
    if (fly_time > 10)
        dir = 0;

    translateX += speed * sin(dir) * deltaTimeSeconds;
    translateY += speed * cos(dir) * deltaTimeSeconds;
    if (translateX > 1280 || translateX < 0) {
        dir = 6.28 - dir;
        if (translateX < 0)
            translateX = 0;
        else
            translateX = 1280;
    }
    else {
        if (translateY > 720 || translateY < 0) {
            dir = 3.14 - dir;
            if (translateY < 0)
            {
                translateY = 0;
                if (shot)
                    Shooted();
            }
            else
            {
                translateY = 720;
                if (fly_time > 10)
                    Evaded();
            }
        }
    }
    flap += 0.005 * flap_dir * speed * deltaTimeSeconds;
    if (flap > 0.1) {
        flap_dir = -1;
        flap = 0.1;
    }
    if (flap < -0.3) {
        flap_dir = 1;
        flap = -0.3;
    }
    if (lifes > 0 && killed < 10)
        RenderScene();
    else
        RenderEnd();
}


void tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void tema1::OnKeyPress(int key, int mods)
{
}


void tema1::OnKeyRelease(int key, int mods)
{
}


void tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}


void tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        amo--;
        glm::ivec2 resolution = window->GetResolution();
        float X = (float)mouseX * 1280 / resolution.x;
        float Y = (float)(resolution.y - mouseY) * 720 / resolution.y;
        if (amo <= 0 && !shot)
            fly_time = 10.1;
        if (fly_time<10 && !shot && object2D::CheckPoint(glm::vec3(X, Y, 1), headMatrix)) {
            dir = 3.14;
            fly_time = 0;
            shot = true;
        }
    }
}


void tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void tema1::OnWindowResize(int width, int height)
{
}

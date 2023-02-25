#include <p6/p6.h>
#include "glimac/default_shader.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP OpenGL"}};
    ctx.maximize_window();

    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

    // Création d'un Vertex Buffer Object et d'un Vertex Array Object
    GLuint vbo, vao;

    // Allocation d'un Vertex Buffer Object:
    glGenBuffers(1, &vbo);

    // "vbo" devient le VBO courant sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Stockage des données du tableau vertices dans le vbo placé sur GL_ARRAY_BUFFER (c'est à dire "vbo" ici) :
    glBufferData(GL_ARRAY_BUFFER, vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Plus de VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Allocation d'un Vertex Array Objet:
    glGenVertexArrays(1, &vao);

    // "vao" devient le VAO courant:
    glBindVertexArray(vao);

    // "vbo" devient le VBO courant sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Activation de l'attribut de vertex 0; nous l'interpretons comme la position
    glEnableVertexAttribArray(0 /* Incide attribut*/);
    // On spécifie le type de donnée de l'attribut position ainsi que la manière dont il est stocké dans le VBO
    glVertexAttribPointer(
        0 /* Indice attribut */,
        3 /* Nombre de composantes */,
        GL_FLOAT /* Type d'une composante */,
        GL_FALSE /* Pas de normalisation */,
        8 * sizeof(GLfloat) /* Taille en octet d'un vertex complet entre chaque attribut position */,
        0 /* OpenGL doit utiliser le VBO attaché à GL_ARRAY_BUFFER et commencer à l'offset 0 */
    );
    // Activation de l'attribut de vertex 0; nous l'interpretons comme la position
    glEnableVertexAttribArray(1 /* Incide attribut*/);
    // On spécifie le type de donnée de l'attribut position ainsi que la manière dont il est stocké dans le VBO
    glVertexAttribPointer(
        1 /* Indice attribut */,
        3 /* Nombre de composantes */,
        GL_FLOAT /* Type d'une composante */,
        GL_FALSE /* Pas de normalisation */,
        8 * sizeof(GLfloat) /* Taille en octet d'un vertex complet entre chaque attribut position */,
        (void*)(3 * sizeof(GLfloat)) /* OpenGL doit utiliser le VBO attaché à GL_ARRAY_BUFFER et commencer à l'offset 0 */
    );
    // Activation de l'attribut de vertex 0; nous l'interpretons comme la position
    glEnableVertexAttribArray(2 /* Incide attribut*/);
    // On spécifie le type de donnée de l'attribut position ainsi que la manière dont il est stocké dans le VBO
    glVertexAttribPointer(
        2 /* Indice attribut */,
        2 /* Nombre de composantes */,
        GL_FLOAT /* Type d'une composante */,
        GL_FALSE /* Pas de normalisation */,
        8 * sizeof(GLfloat) /* Taille en octet d'un vertex complet entre chaque attribut position */,
        (void*)(6 * sizeof(GLfloat)) /* OpenGL doit utiliser le VBO attaché à GL_ARRAY_BUFFER et commencer à l'offset 0 */
    );

    // Plus de VAO courant:
    glBindVertexArray(0);

    auto const shader = p6::load_shader("shaders/3D.vs.glsl", "normals.fs.glsl");
    glEnable(GL_DEPTH_TEST);

    const glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
    const glm::mat4 MVMatrix     = glm::translate(glm::mat4{1.f}, {0.f, 0.f, -5.f});
    const glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    // Boucle d'update
    ctx.update = [&]() {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // "vao" devient le VAO courant, OpenGL l'utilisera lors du dessin pour avoir des informations sur les sommets
        glBindVertexArray(vao);
        shader.use();
        shader.set("uMVPMatrix", ProjMatrix * MVMatrix);
        shader.set("uMVMatrix", MVMatrix);
        shader.set("uNormalMatrix", NormalMatrix);

        shader.check_for_errors_before_rendering();
        // OpenGL doit dessiner des triangles en utilisant 3 sommets (donc un seul triangle)
        glDrawArrays(GL_TRIANGLES, 0 /* Pas d'offset au début du VBO */, vertices.size());

        // Plus de VAO courant:
        glBindVertexArray(0);
    };

    ctx.start();

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    return 0;
}
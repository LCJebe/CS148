#include "assignment2/Assignment2.h"
#include "common/core.h" // <-- haha.
#include "common/Utility/Mesh/Simple/PrimitiveCreator.h"
#include "common/Utility/Mesh/Loading/MeshLoader.h"
#include <cmath>
#include <fstream>
#include <string>

namespace
{
const int SHADER_ERROR_LOG_SIZE = 500;
bool VerifyShaderCompile(GLuint newShaderObject)
{
    GLint compileStatus;
    OGL_CALL(glGetShaderiv(newShaderObject, GL_COMPILE_STATUS, &compileStatus));
    if (compileStatus != GL_TRUE) {
        char errorLogBuffer[SHADER_ERROR_LOG_SIZE];
        OGL_CALL(glGetShaderInfoLog(newShaderObject, SHADER_ERROR_LOG_SIZE, NULL, errorLogBuffer));
        std::cerr << "ERROR: Shader compilation failure -- " << std::endl << errorLogBuffer << std::endl;
        return false;
    }
    return true;
}

bool VerifyProgramLink(GLuint shaderProgram)
{
    GLint linkStatus;
    OGL_CALL(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus));
    if (linkStatus != GL_TRUE) {
        char errorLogBuffer[SHADER_ERROR_LOG_SIZE];
        OGL_CALL(glGetProgramInfoLog(shaderProgram, SHADER_ERROR_LOG_SIZE, NULL, errorLogBuffer));
        std::cerr << "ERROR: Program link compilation failure -- " << std::endl << errorLogBuffer << std::endl;
        return false;
    }
    return true;
}
}

Assignment2::Assignment2(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
    Application(std::move(inputScene), std::move(inputCamera))
{
    vertexPositions = 
        std::initializer_list<glm::vec4>({
            // Triangle 1
            {0.f, 0.f, 0.f, 1.f},
            {1.f, 0.f, 0.f, 1.f},
            {1.f, 1.f, 0.f, 1.f},
            // Triangle 2
            {0.f, 0.f, 0.f, 1.f},
            {-1.f, 1.f, 0.f, 1.f},
            {-1.f, 0.f, 0.f, 1.f}
        });

    vertexColors =
        std::initializer_list<glm::vec3>({
            // Triangle 1
            {1.f, 0.f, 0.f},
            {0.f, 1.f, 0.f},
            {0.f, 0.f, 1.f},
            // Triangle 2
            {1.f, 0.f, 0.f},
            {0.f, 0.f, 1.f},
            {0.f, 1.f, 0.f}
        });

    time = 0;
}

std::unique_ptr<Application> Assignment2::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return make_unique<Assignment2>(std::move(scene), std::move(camera));
}

std::shared_ptr<class Camera> Assignment2::CreateCamera()
{
    // Specify any old aspect ratio for now, we'll update it later once the window gets made!
    // Read more about Field of View: http://rg3.name/201210281829.html!
    // Note that our field of view is the VERTICAL field of view (in degrees).
    return std::make_shared<PerspectiveCamera>(75.f, 1280.f / 720.f);
}

glm::vec2 Assignment2::GetWindowSize() const
{
    return glm::vec2(1280.f, 720.f);
}

void Assignment2::SetupScene()
{
    SetupExample1();
}

void Assignment2::SetupCamera()
{
}

void Assignment2::HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime)
{   
    Application::HandleInput(key, state, repeat, timestamp, deltaTime);
}

void Assignment2::HandleWindowResize(float x, float y)
{
    Application::HandleWindowResize(x, y);
}

void Assignment2::SetupExample1()
{
    // Insert "Load and Compile Shaders" code here.

    // Read shader file content into string
    const std::string vertPath = std::string(STRINGIFY(SHADER_PATH)) + "/hw2/hw2.vert";
    const std::string fragPath = std::string(STRINGIFY(SHADER_PATH)) + "/hw2/hw2.frag";
    std::ifstream vertStream(vertPath, std::ifstream::in);
    std::ifstream fragStream(fragPath, std::ifstream::in);
    std::string vertString( (std::istreambuf_iterator<char>(vertStream) ),
                         (std::istreambuf_iterator<char>()    ) );
    std::string fragString( (std::istreambuf_iterator<char>(fragStream) ),
                         (std::istreambuf_iterator<char>()    ) );

    // create shader objects
    GLuint vertShaderId = glCreateShader( GL_VERTEX_SHADER);
    GLuint fragShaderId = glCreateShader( GL_FRAGMENT_SHADER);

    // replace source code in the shader objects
    const char* vertTextString = vertString.c_str();
    const char* fragTextString = fragString.c_str();
    glShaderSource( vertShaderId, 1, &vertTextString, NULL);
    glShaderSource( fragShaderId, 1, &fragTextString, NULL);

    // compile shaders
    glCompileShader( vertShaderId);
    glCompileShader( fragShaderId);

    // create shader program
    GLuint shadProgramId = glCreateProgram();
    this->shaderProgramId = shadProgramId;

    // attach both shader objects to a program object
    glAttachShader( shadProgramId, vertShaderId);
    glAttachShader( shadProgramId, fragShaderId);

    // link program object
    glLinkProgram( shadProgramId);


    // Checkpoint 1.
    // Modify this part to contain your vertex shader ID, fragment shader ID, and shader program ID.
    const GLuint vertexShaderId = vertShaderId;
    const GLuint fragmentShaderId = fragShaderId;
    const GLuint shaderProgramId = shadProgramId;

    // create vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao);
    this->vertexArrayObjectId = vao;

    // bind the array object
    glBindVertexArray( vao);


    // DO NOT EDIT OR REMOVE THE CODE IN THIS SECTION
    if (!VerifyShaderCompile(vertexShaderId) || !VerifyShaderCompile(fragmentShaderId) || !VerifyProgramLink(shaderProgramId)) {
        std::cout << "FAILURE: Checkpoint 1 failed." << std::endl;
    } else {
        std::cout << "SUCCESS: Checkpoint 1 completed." << std::endl;
    }

    OGL_CALL(glDetachShader(shaderProgramId, vertexShaderId));
    OGL_CALL(glDeleteShader(vertexShaderId));
    OGL_CALL(glDetachShader(shaderProgramId, fragmentShaderId));
    OGL_CALL(glDeleteShader(fragmentShaderId));
    // FINISH DO NOT EDIT OR REMOVE THE CODE IN THIS SECTION

    // Insert "Setup Buffers" code here.

    // generate buffer object names
    GLuint vbuffer;
    glGenBuffers( 1, &vbuffer);

    // bind the named buffer object
    glBindBuffer( GL_ARRAY_BUFFER, vbuffer);

    // pass vertex position and color data to the buffer
    glBufferData( GL_ARRAY_BUFFER, sizeof(glm::vec4) * vertexPositions.size(), &vertexPositions[0], GL_STATIC_DRAW);

    // let OpenGL know how the data should be used
    glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, 0); // index, size, type normalized, stride, pointer

    // make current buffer object to be passed to OpenGL along with vao.
    glEnableVertexAttribArray(0);

    // do the same for colors!
    GLuint cbuffer;
    glGenBuffers( 1, &cbuffer);
    glBindBuffer( GL_ARRAY_BUFFER, cbuffer);
    glBufferData( GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexColors.size(), &vertexColors[0], GL_STATIC_DRAW);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0); // index, size, type normalized, stride, pointer
    glEnableVertexAttribArray(1);
}

void Assignment2::Tick(double deltaTime)
{
    // Insert "Send Buffers to the GPU" and "Slightly-More Advanced Shaders" code here.

    // set up GPU usage. specify program to be used
    glUseProgram( shaderProgramId);

    // bind vertex array
    glBindVertexArray(this->vertexArrayObjectId);

    // change color by performing color step
    ColorStep(double(0.003f));
    glBufferData( GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexColors.size(), &vertexColors[0], GL_STATIC_DRAW);

    // send draw command to OpenGl
    glDrawArrays(GL_TRIANGLES, 0, vertexPositions.size());

    // increase time step
    time += deltaTime;

    // get location of uniform vertex shader variable inputTime
    const GLchar* inputTimeName = "inputTime";
    GLuint inputTimeLocation = glGetUniformLocation(shaderProgramId, inputTimeName);

    // set uniform variable
    glUniform1f(inputTimeLocation, time);
}

void Assignment2::ColorStep(double step)
{
    for(unsigned int i=0;i<vertexColors.size();i++)  {
        if (vertexColors[i].x < 1.0f && vertexColors[i].z > 0.0f  && vertexColors[i].y <= 0.0f) {
            vertexColors[i].x += step;
            vertexColors[i].z -= step;
            vertexColors[i].y = 0.0f;
        } else if (vertexColors[i].y < 1.0f && vertexColors[i].x > 0.0f  && vertexColors[i].z <= 0.0f) {
            vertexColors[i].y += step;
            vertexColors[i].x -= step;
            vertexColors[i].z = 0.0f;
        } else if (vertexColors[i].z < 1.0f && vertexColors[i].y > 0.0f  && vertexColors[i].x <= 0.0f) {
            vertexColors[i].z += step;
            vertexColors[i].y -= step;
            vertexColors[i].x = 0.0f;
        }
    }

}

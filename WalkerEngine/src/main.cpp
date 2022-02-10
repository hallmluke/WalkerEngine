#include "walkerpch.h"/*
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Cube.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Skybox.h"
#include "GBuffer.h"
#include "GBufferPBR.h"
#include "Quad.h"
#include "SSAOPass.h"
#include "SSAOBlurPass.h"
#include "Volume.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ImGuiManager.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1600; // 1600;
const unsigned int SCR_HEIGHT = 900; // 900;
unsigned int currentWidth = 1600;
unsigned int currentHeight = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool cameraMove = false;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 4.0f);
glm::vec3 directionLight(0.2f, -1.0f, 0.2f);


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    //GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Walker Engine", glfwGetPrimaryMonitor(), NULL);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Walker Engine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    ImGuiManager imGuiManager(window);

    int width, height, nrComponents;
    unsigned char* data = stbi_load("Icons/skywalker_icon.png", &width, &height, &nrComponents, 0);
    if (data) {
        GLFWimage icons[1];
        icons[0].pixels = data;
        icons[0].width = width;
        icons[0].height = height;
        glfwSetWindowIcon(window, 1, icons);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load icon skywalker_icon.png" << std::endl;
        stbi_image_free(data);
    }

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }



    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    //stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);


    // build and compile shaders
    // -------------------------
    Shader ourShader("Shaders/basic_lighting.vert", "Shaders/basic_lighting.frag");
    Shader lightShader("Shaders/light_cube.vert", "Shaders/light_cube.frag");
    Shader depthShader("Shaders/depth_shader.vert", "Shaders/depth_shader.frag");
    Shader depthCubeShader("Shaders/depth_shader_cube.vert", "Shaders/depth_shader_cube.frag", "Shaders/depth_shader_cube.geom");
    Shader debugDepthQuad("Shaders/debug_quad.vert", "Shaders/debug_quad.frag");
    //Shader deferredShader("Shaders/deferred_shading.vert", "Shaders/deferred_shading.frag");
    Shader deferredShaderPBR("Shaders/deferred_shading.vert", "Shaders/pbr_deferred.frag");

    // load models
    // -----------
    // render the loaded model
    //glm::mat4 model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    //model = glm::scale(model, glm::vec3(.01f, .01f, .01f));	// it's a bit too big for our scene, so scale it down

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::scale(transform, glm::vec3(0.01f, 0.01f, 0.01f));

    glm::mat4 transform2 = glm::mat4(1.0f);
    transform2 = glm::translate(transform2, glm::vec3(1.0f, 1.0f, 3.0f));

    glm::mat4 transform3 = glm::mat4(1.0f);
    transform3 = glm::translate(transform3, glm::vec3(0.0f, 3.0f, 0.0f));
    transform3 = glm::scale(transform3, glm::vec3(0.25f, 0.25f, 0.25f));

    glm::mat4 transform4 = glm::mat4(1.0f);

    //Model sponza("Sponza", "Models/sponza/sponza.obj", transform);
    //Model backpack("Backpack", "Models/backpack/backpack.obj", transform2);
    //Model nano("Nano", "Models/nano/nano_hierarchy.gltf", transform3);
    //Model cerberus("Cerberus", "Models/Cerberus/Cerberus_Textured2.fbx", transform3);
    //Model bathroom("Bathroom", "Models/bathroom/textures/bathroom.fbx", transform);
    Model sponzaPBR("SponzaPBR", "Models/SponzaPBR/Sponza.gltf", transform4);

    PointLight light(lightPos);
    std::vector<PointLight*> lights = { &light };
    DirectionalLight dirLight(directionLight, true);
    Skybox skybox("Skybox/default");
    //PointLight light(lightPos);

    //GBuffer gBuffer(SCR_WIDTH, SCR_HEIGHT);
    GBufferPBR gBufferPBR(SCR_WIDTH, SCR_HEIGHT);
    SSAOPass ssaoPass(SCR_WIDTH, SCR_HEIGHT);
    SSAOBlurPass ssaoBlurPass(SCR_WIDTH, SCR_HEIGHT);
    Quad quad;

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //Volume volume = Volume();
    

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //

        imGuiManager.BeginFrame();

        // view/projection transformations
        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 projection = camera.GetProjectionMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
        glm::mat4 view = camera.GetViewMatrix();

        //volume.Draw(view, projection, camera.Position);

        if (light.drawDebugEnabled) {
            lightShader.use();
            lightShader.setMat4("projection", projection);
            lightShader.setMat4("view", view);
            light.DrawDebug(lightShader);
        }

        if (dirLight.shadowMapEnabled) {
            //dirLight.GenerateShadowMap(depthShader, sponzaPBR);
            dirLight.GenerateCascadedShadowMap(sponzaPBR, camera);
        }

        if (light.shadowMapEnabled) {
            light.GenerateShadowMap(depthCubeShader, sponzaPBR);
        }

        //gBuffer.BindFramebuffer();
        //gBuffer.DrawModel(sponza, view, projection);
        //gBuffer.DrawModel(nano, view, projection);
        //gBuffer.DrawModel(cerberus, view, projection);
        gBufferPBR.BindFramebuffer();
        gBufferPBR.DrawModel(sponzaPBR, view, projection);

        ssaoPass.BindFramebuffer();
        //gBuffer.BindTextures();
        gBufferPBR.BindTextures();
        ssaoPass.Draw(projection, view);

        ssaoBlurPass.BindFrameBuffer();
        ssaoPass.BindTextures();
        ssaoBlurPass.Draw();


        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        deferredShaderPBR.use();
        gBufferPBR.BindTextures();
        //gBuffer.BindTextures();
        //ssaoPass.BindTextures();
        ssaoBlurPass.BindTextures();

        deferredShaderPBR.setInt("gPosition", 0);
        deferredShaderPBR.setInt("gNormal", 1);
        deferredShaderPBR.setInt("gAlbedo", 2);
        deferredShaderPBR.setInt("gMetRoughAO", 3);
        deferredShaderPBR.setInt("ssaoColor", 4);
        deferredShaderPBR.setInt("debugPass", gBufferPBR.debugPass);
        deferredShaderPBR.setBool("useAmbientOcclusion", gBufferPBR.ambientOcclusionEnabled);

        deferredShaderPBR.setVec3("camPos", camera.Position);
        //deferredShaderPBR.setInt("cascadeCount", 4);
        deferredShaderPBR.setPointLightProperties(lights);
        deferredShaderPBR.setDirectionalLightProperties(dirLight, camera);
        quad.Draw();


        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBufferPBR.GetGBuffer());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
        glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT,
            GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (light.drawDebugEnabled) {
            lightShader.use();
            lightShader.setMat4("projection", projection);
            lightShader.setMat4("view", view);
            light.DrawDebug(lightShader);
        }

        if (dirLight.drawDebugEnabled) {
            lightShader.use();
            lightShader.setMat4("projection", projection);
            lightShader.setMat4("view", view);
            dirLight.DrawDebug(lightShader);
        }

        skybox.Draw(view, projection);


        light.ControlWindow();
        dirLight.ControlWindow();
        //sponza.ControlWindow();
        //backpack.ControlWindow();
        //nano.ControlWindow();
        //bathroom.ControlWindow();
        sponzaPBR.ControlWindow();
        gBufferPBR.ControlWindow();

        //volume.ControlWindow();
        imGuiManager.EndFrame();


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();

    }


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (!ImGui::GetIO().WantCaptureMouse || cameraMove) {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {

            cameraMove = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                camera.ProcessKeyboard(FORWARD, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                camera.ProcessKeyboard(BACKWARD, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                camera.ProcessKeyboard(LEFT, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                camera.ProcessKeyboard(RIGHT, deltaTime);
        }
        else {
            cameraMove = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    currentWidth = width;
    currentHeight = height;
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    if (cameraMove) {

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        camera.ProcessMouseMovement(xoffset, yoffset);
    }


    lastX = xpos;
    lastY = ypos;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (!ImGui::GetIO().WantCaptureMouse || cameraMove) {
        camera.ProcessMouseScroll(yoffset);
    }
}*/
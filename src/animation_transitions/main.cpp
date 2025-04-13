#include "animation_transitions/main.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include <math.h>

static void glfw_error_callback(int error, const char *description)
{
    std::cout << "GLFW Error " << error << ": " << description << std::endl;
}

int main(int, char **)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char *glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Animation Transitions", nullptr, nullptr);
    if (window == nullptr)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        std::cout << "Failed to initialize OpenGL context\n"
                  << std::endl;
        return -1;
    }

    std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    const char *animation_names[] = {
        "Slide", "Bounce", "Zoom", "Fade", "Orbit", "Pulse", "Jitter"};

    AnimationParameters animParams;
    int selected = AnimationType::Slide;

    float zoomStartTime = 0.0f;
    bool zoomInitialized = false;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);

        ImGui::Begin(
            "Main",
            nullptr,
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.1, io.DisplaySize.y * 0.1));
        ImGui::BeginChild("canvas_rectangle", ImVec2(io.DisplaySize.x * 0.8, io.DisplaySize.y * 0.4), 0, ImGuiWindowFlags_NoDecoration);
        ImVec2 canvas_rectangle_position = ImGui::GetCursorScreenPos();
        ImVec2 canvas_rectangle_size = ImGui::GetContentRegionAvail();
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(
            canvas_rectangle_position,
            ImVec2(
                canvas_rectangle_position.x + canvas_rectangle_size.x,
                canvas_rectangle_position.y + canvas_rectangle_size.y),
            IM_COL32(30, 30, 30, 255));

        float ball_radius = 0.025 * sqrt(powf32(io.DisplaySize.x, 2) + powf32(io.DisplaySize.y, 2));
        ImVec2 center = ImVec2(
            canvas_rectangle_position.x + canvas_rectangle_size.x / 2,
            canvas_rectangle_position.y + canvas_rectangle_size.y / 2);
        ImVec2 ball_position = ImVec2(center.x, center.y);

        float ball_alpha = 255.0f;
        float time = static_cast<float>(glfwGetTime());
        float offset_x = 0.0f, offset_y = 0.0f;

        switch (selected)
        {
        case Slide:
        {
            offset_x = animParams.slide.amplitude * sinf(2.0f * M_PI * animParams.slide.speed * time);
            break;
        }
        case Bounce:
        {
            offset_y = -animParams.bounce.height * fabsf(sinf(2.0f * M_PI * animParams.bounce.speed * time));
            break;
        }
        case Zoom:
        {
            float currentTime = static_cast<float>(glfwGetTime());

            if (!zoomInitialized)
            {
                zoomStartTime = currentTime;
                zoomInitialized = true;
            }

            float elapsed = currentTime - zoomStartTime;
            float duration = 1.0f / animParams.zoom.speed;

            float t = std::min(elapsed / duration, 1.0f);
            float eased = easeInOut(t);

            float scale = animParams.zoom.minScale +
                          (animParams.zoom.maxScale - animParams.zoom.minScale) * eased;

            ball_radius *= scale;

            break;
        }
        case Fade:
        {
            ball_alpha = 255.0f * (animParams.fade.minAlpha +
                                   (animParams.fade.maxAlpha - animParams.fade.minAlpha) * 0.5f * (sinf(2.0f * M_PI * animParams.fade.speed * time) + 1.0f));
            break;
        }
        case Orbit:
        {
            offset_x = animParams.orbit.radius * cosf(2.0f * M_PI * animParams.orbit.speed * time);
            offset_y = animParams.orbit.radius * sinf(2.0f * M_PI * animParams.orbit.speed * time);
            break;
        }
        case Pulse:
        {
            float scale = animParams.pulse.minScale +
                          (animParams.pulse.maxScale - animParams.pulse.minScale) * 0.5f * (sinf(2.0f * M_PI * animParams.pulse.speed * time) + 1.0f);
            ball_radius *= scale;
            break;
        }
        case Jitter:
        {
            float phase = sinf(2.0f * M_PI * animParams.jitter.frequency * time);
            offset_x = animParams.jitter.intensity * ((rand() % 2000) / 1000.0f - 1.0f) * phase;
            offset_y = animParams.jitter.intensity * ((rand() % 2000) / 1000.0f - 1.0f) * phase;
            break;
        }
        }

        if (selected != Zoom)
            zoomInitialized = false;

        ball_position.x += offset_x;
        ball_position.y += offset_y;

        draw_list->AddCircleFilled(
            ball_position,
            ball_radius,
            IM_COL32(128, 128, 128, ball_alpha));

        ImGui::EndChild();

        ImGui::BeginChild("transitions");

        ImVec2 bottom_panel_position = ImVec2(0.0, io.DisplaySize.y * 0.4);
        ImVec2 bottom_panel_size = ImVec2(io.DisplaySize.x * 0.8, io.DisplaySize.y * 0.4);

        ImGui::Separator();
        ImGui::Text("Choose Animation Type:");

        for (int i = 0; i < IM_ARRAYSIZE(animation_names); ++i)
        {
            if (i > 0)
            {
                ImGui::SameLine();
            }
            if (ImGui::RadioButton(animation_names[i], selected == i))
            {
                selected = i;
            }
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Parameters:");

        switch (selected)
        {
        case Slide:
        {
            ImGui::SliderFloat("Speed (Hz)", &animParams.slide.speed, 0.1f, 5.0f);
            ImGui::SliderFloat("Amplitude (px)", &animParams.slide.amplitude, 10.0f, 300.0f);
            break;
        }
        case Bounce:
        {
            ImGui::SliderFloat("Speed (Hz)", &animParams.bounce.speed, 0.1f, 5.0f);
            ImGui::SliderFloat("Bounce Height (px)", &animParams.bounce.height, 10.0f, 300.0f);
            break;
        }
        case Zoom:
        {
            ImGui::SliderFloat("Speed (Hz)", &animParams.zoom.speed, 0.1f, 5.0f);
            ImGui::SliderFloat("Min Scale", &animParams.zoom.minScale, 0.1f, 1.0f);
            ImGui::SliderFloat("Max Scale", &animParams.zoom.maxScale, 1.0f, 3.0f);
            break;
        }
        case Fade:
        {
            ImGui::SliderFloat("Speed (Hz)", &animParams.fade.speed, 0.1f, 5.0f);
            ImGui::SliderFloat("Min Alpha", &animParams.fade.minAlpha, 0.0f, 1.0f);
            ImGui::SliderFloat("Max Alpha", &animParams.fade.maxAlpha, 0.0f, 1.0f);
            break;
        }
        case Orbit:
        {
            ImGui::SliderFloat("Orbit Speed (Hz)", &animParams.orbit.speed, 0.1f, 5.0f);
            ImGui::SliderFloat("Orbit Radius (px)", &animParams.orbit.radius, 10.0f, 300.0f);
            break;
        }
        case Pulse:
        {
            ImGui::SliderFloat("Pulse Rate (Hz)", &animParams.pulse.speed, 0.1f, 5.0f);
            ImGui::SliderFloat("Min Scale", &animParams.pulse.minScale, 0.1f, 1.0f);
            ImGui::SliderFloat("Max Scale", &animParams.pulse.maxScale, 1.0f, 3.0f);
            break;
        }
        case Jitter:
        {
            ImGui::SliderFloat("Intensity (px)", &animParams.jitter.intensity, 0.1f, 20.0f);
            ImGui::SliderFloat("Frequency (Hz)", &animParams.jitter.frequency, 1.0f, 60.0f);
            break;
        }
        }

        ImGui::EndChild();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
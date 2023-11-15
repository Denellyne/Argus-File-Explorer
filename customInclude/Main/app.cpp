#include "app.h"
#include ".\Functionality\Functions.h"
#include <vector>
#include <stack>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"
#include <GLFW/glfw3.h>

void fileExplorer(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack) {
    int choice = 0;
    std::string newPath;
    std::cin >> choice;
    switch (choice) {
    case 1:
        std::getline(std::cin >> std::ws, newPath);
        searchNewPath(newPath, directories, files, directoryStack);
        break;
    case 2:
        returnPath(directories, files, directoryStack);
        break;
    }
}

void app(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack);

int GUI(){
    std::vector<Directory> directories(0, Directory("C:\\"));
    std::vector<File> files(0, File("fileExplorer.exe"));
    std::stack<std::string> directoryStack;
    directoryStack.push("C:\\");

    glfwInit();
    
    GLFWwindow* window = glfwCreateWindow(1280, 720, "File Explorer", nullptr, nullptr);
    if (window == nullptr) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    
    ImGui::StyleColorsDark();
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window)) { // Render
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
       
        app(directories,files,directoryStack);

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}


void app(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack) {
    using namespace ImGui;

    std::string userInputDirectory;
    ImGuiIO& io = ImGui::GetIO();
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    SetNextWindowSize(ImVec2(io.DisplaySize.x,io.DisplaySize.y));
    SetNextWindowPos(ImVec2(0,0));


    Begin("##FileExplorer",NULL,ImGuiWindowFlags_NoTitleBar);
    PopStyleVar();
    //BeginMainMenuBar();
    if (Button("Recursive")) recursive("C:\\");
    if (InputText("Directory", &userInputDirectory, ImGuiInputTextFlags_EnterReturnsTrue)) searchNewPath(userInputDirectory, directories, files, directoryStack);
    //EndMainMenuBar();
    BeginChild("Files", { 0,0 }, false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    EndChild();


    End();
}
#include "app.h"
#include ".\Functionality\Search\Search.h"
#include "Functionality/Drive/drive.h"
#include <vector>
#include <stack>
#include <windows.h> 

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#define GL_CLAMP_TO_EDGE 0x812F
#include "stb_image.h"

bool LoadTextureFromFile(const char* filename, GLuint* out_texture);
int GUI();
//GLuint folderIcon,GLuint fileIcon
void app(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack,std::vector<GLuint> &Icons,std::string &userInputDirectory,bool &darkMode) {

    //Boilerplate Window Code
    using namespace ImGui;

    ImVec2 iconSize = {40,40};
    ImGuiIO& io = ImGui::GetIO();
   
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    SetNextWindowSize(ImVec2(io.DisplaySize.x,io.DisplaySize.y));
    SetNextWindowPos(ImVec2(0,0));
    ImGuiStyle& style = ImGui::GetStyle();
    
    //Theme Switcher
    if (darkMode) {
        style.Colors[ImGuiCol_Text] = ImVec4(1,1,1,1);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.082, 0.086, 0.09,1);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.082, 0.086, 0.10, 1); 

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.125, 0.196, 0.302, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.185, 0.186, 0.302, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.185, 0.186, 0.302, 1));
    }
    else {
        //Light Mode

        style.Colors[ImGuiCol_Text] = ImVec4(0.082, 0.086, 0.09, 1);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15, 0.17, 0.30, 1.0);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.94, 1.00, 1.00, 1);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.125, 0.196, 0.302, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.185, 0.186, 0.302, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.185, 0.186, 0.302, 1));
    }
    

    Begin("##FileExplorer",NULL,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

    PopStyleVar();
    
    //Button
    SetCursorScreenPos({ 9,5 });
    if (Button("<", { 20,20 }) && directoryStack.size() > 1) returnPath(directories, files, directoryStack,userInputDirectory);
    SetCursorScreenPos({ 30,5 });
    if (Button(">", { 20,20 }) && directoryStack.size() > 1) returnPath(directories, files, directoryStack, userInputDirectory);
    SetCursorScreenPos({ 51,5 });
    if (ImageButton((void*)(intptr_t)Icons[0], {14,14})) darkMode = !darkMode;
    PopStyleColor(3);



    //Hotkeys

    if (IsKeyPressed(ImGuiKey_Escape) && directoryStack.size() > 1) returnPath(directories, files, directoryStack, userInputDirectory);
    if (IsKeyDown(ImGuiMod_Alt) && IsKeyDown(ImGuiKey_Q)) _exit(NULL); //Ctrl + Alt + Q || AltGr + Q

    SetCursorPos({ io.DisplaySize.x/6,5});

    if (InputText("##NULL", &userInputDirectory, ImGuiInputTextFlags_EnterReturnsTrue)) searchNewPath(userInputDirectory, directories, files, directoryStack);

    BeginChild("##NULL");
    

    //Display icons

    Columns(7,NULL,false);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.182, 0.186, 0.20, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.182, 0.186, 0.20, 1));


    for (int i = 0; i < directories.size(); i++) {
        ImageButton((void*)(intptr_t)Icons[1], iconSize);
        if (IsItemHovered() && IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            userInputDirectory = directories[i].filePath;
            searchNewPath(userInputDirectory, directories, files, directoryStack);           
        }
        TextWrapped((directories[i].filePath.substr(directories[i].filePath.find_last_of("//") + 1).c_str()));
        NextColumn();
    }
    for (int i = 0; i < files.size(); i++){
        ImageButton((void*)(intptr_t)Icons[2], iconSize);
        if (IsItemHovered() && IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            ShellExecuteA(NULL,"open",files[i].filePath.c_str(), NULL, NULL, SW_SHOWDEFAULT);
        }
        TextWrapped((files[i].filePath.substr(files[i].filePath.find_last_of("//")+1).c_str()));
        NextColumn();
    }


    PopStyleColor(3);
    EndChild();
    End();
}

int GUI() {

    //Variables
    std::vector<Directory> directories(0, Directory("C:\\"));
    std::vector<File> files(0, File("Argus.exe"));
    std::stack<std::string> directoryStack;
    std::vector<std::string> drive;
    std::vector <GLuint> Icons;
    std::string userInputDirectory;
    bool darkMode = true;
    directoryStack.push("C:/");
    Icons.reserve(3);



    // GUI BoilerPlate
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Argus", nullptr, nullptr);
    if (window == nullptr) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    int width, height, channels;
    unsigned char* icon = stbi_load("./icons/icon.png", &width, &height, &channels, 4);
    GLFWimage images[1];
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = icon;
    glfwSetWindowIcon(window, 1, images);

    LoadTextureFromFile("./icons/sun.png", &Icons[0]);
    LoadTextureFromFile("./icons/folder.png", &Icons[1]);
    LoadTextureFromFile("./icons/file.png", &Icons[2]);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    //driveIndex(drive);

    while (!glfwWindowShouldClose(window)) { // Render
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        app(directories, files, directoryStack,Icons,userInputDirectory,darkMode);

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

bool LoadTextureFromFile(const char* filename, GLuint* out_texture)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;

    return true;
}

//Ignore
//void fileExplorer(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack) {
//    int choice = 0;
//    std::string newPath;
//    std::cin >> choice;
//    switch (choice) {
//    case 1:
//        std::getline(std::cin >> std::ws, newPath);
//        searchNewPath(newPath, directories, files, directoryStack);
//        break;
//    case 2:
//        returnPath(directories, files, directoryStack);
//        break;
//    }
//}
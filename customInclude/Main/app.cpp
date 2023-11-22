#include "app.h"
#include ".\Functionality\Search\Search.h"
#include "Functionality/Drive/drive.h"
#include <vector>
#include <format>
#include <tuple>
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

void app(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack, std::vector<GLuint>& Icons, std::string& userInputDirectory, bool& darkMode,std::string &forwardPath) {
    //Boilerplate Window Code
    ImVec2 iconSize = { 40,40 };
    ImGuiIO& io = ImGui::GetIO();
    
    std::string userEnvName = getenv("username");
    std::tuple<std::string, std::string> shortcuts[3] = { {"Desktop",(std::format("C:/Users/{}/Desktop",userEnvName))},{"Documents",(std::format("C:/Users/{}/Documents",userEnvName))},{"Downloads",(std::format("C:/Users/{}/Downloads",userEnvName))} };
    int sizeShortcuts = sizeof(shortcuts) / sizeof(shortcuts[0]);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGuiStyle& style = ImGui::GetStyle();

    ImGui::Begin("##FileExplorer", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

    ImGui::PopStyleVar();

    //Button
    ImGui::SetCursorScreenPos({ 9,5 });
    if (ImGui::Button("<", { 20,20 }) && directoryStack.size() > 1) returnPath(directories, files, directoryStack, userInputDirectory,forwardPath);
    ImGui::SetCursorScreenPos({ 30,5 });
    if (ImGui::Button(">", { 20,20 }) && !forwardPath.empty()) {
        searchNewPath(forwardPath, directories, files, directoryStack);
        userInputDirectory = forwardPath;
    }
    ImGui::SetCursorScreenPos({ 51,5 });
    if (ImGui::ImageButton((void*)(intptr_t)Icons[0], { 14,14 })) darkMode = !darkMode;

    //Theme Switcher
    if (darkMode) {
        style.Colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.082, 0.086, 0.09, 1);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.082, 0.086, 0.10, 1);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.082, 0.086, 0.10, 1);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.082, 0.086, 0.10, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.185, 0.186, 0.302, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.185, 0.186, 0.302, 1));
    }
    else {
        //Light Mode

        style.Colors[ImGuiCol_Text] = ImVec4(0.082, 0.086, 0.09, 1);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.092, 0.1, 0.259, 1);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.6, 0.68, 0.9, 1.0);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.5, 0.58, 0.8, 1.0);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8, 0.84, 1, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8, 0.84, 1, 1));
    }


    //Hotkeys

    if (ImGui::IsKeyPressed(ImGuiKey_Escape) && directoryStack.size() > 1) returnPath(directories, files, directoryStack, userInputDirectory,forwardPath);
    if (ImGui::IsKeyPressed(ImGuiKey_F1) && !forwardPath.empty()) {
        searchNewPath(forwardPath, directories, files, directoryStack);
        userInputDirectory = forwardPath;
    }
    if (ImGui::IsKeyDown(ImGuiMod_Alt) && ImGui::IsKeyDown(ImGuiKey_Q)) _exit(NULL); //Ctrl + Alt + Q || AltGr + Q

    ImGui::SetCursorPos({ io.DisplaySize.x/6,5 });

    if (ImGui::InputText("##NULL", &userInputDirectory, ImGuiInputTextFlags_EnterReturnsTrue)) searchNewPath(userInputDirectory, directories, files, directoryStack);



    ImGui::BeginChild("##NULL"); 

    //Display icons

    ImGui::Columns(7, NULL, false);
   
    for (int i = 0; i < directories.size(); i++) { //Directories
        ImGui::ImageButton((void*)(intptr_t)Icons[1], iconSize);
        ImGui::PushID((2*i + 1));
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            userInputDirectory = directories[i].filePath;
            searchNewPath(userInputDirectory, directories, files, directoryStack);
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary)) {
            ImGui::BeginTooltip();
            ImGui::Text(std::format("Name: {}", (directories[i].filePath.substr(directories[i].filePath.find_last_of("//") + 1))).c_str());
            ImGui::EndTooltip();
        }
        if (ImGui::BeginPopupContextItem("Context Menu")){
            std::string newName = "";  
            std::string fileName = directories[i].filePath.substr(directories[i].filePath.find_last_of("//") + 1);
            std::string dirPath = directories[i].filePath.substr(0,directories[i].filePath.size()-fileName.length());

            if (ImGui::Selectable("Open")) {
                userInputDirectory = directories[i].filePath;
                searchNewPath(userInputDirectory, directories, files, directoryStack);
            }
            if(ImGui::Selectable("Delete")){
                try{
                    std::filesystem::remove_all(directories[i].filePath);
                    directories.erase(directories.begin() + i);
                }
                catch (std::system_error) {}
                catch (std::filesystem::filesystem_error) {}
            }

            if (ImGui::InputText("##Rename", &newName, ImGuiInputTextFlags_EnterReturnsTrue)) {
                try {
                    std::filesystem::rename(directories[i].filePath, std::format("{}/{}", dirPath, newName));
                    directories[i].filePath = std::format("{}/{}", dirPath, newName);
                }
                catch (std::system_error){}
                catch (std::filesystem::filesystem_error){} 
            }
            if (ImGui::Selectable("Proprieties")) {

                SHELLEXECUTEINFO info = { 0 };

                info.cbSize = sizeof info;
                info.lpFile = directories[i].filePath.c_str();
                info.nShow = SW_SHOW;
                info.fMask = SEE_MASK_INVOKEIDLIST;
                info.lpVerb = "properties";

                ShellExecuteEx(&info);
            }
            ImGui::EndPopup();
        }
        ImGui::TextWrapped((directories[i].filePath.substr(directories[i].filePath.find_last_of("//") + 1).c_str()));
        ImGui::NextColumn();
        ImGui::PopID();
    }
    for (int i = 0; i < files.size(); i++) {  //Files
        ImGui::ImageButton((void*)(intptr_t)Icons[2], iconSize);
        ImGui::PushID(2*i);
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            try{
                ShellExecuteA(NULL, "open", files[i].filePath.c_str(), NULL, NULL, SW_SHOWDEFAULT);
            }
            catch (std::system_error) {}
            catch (std::filesystem::filesystem_error) {}
            
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary)) {
            ImGui::BeginTooltip();
            ImGui::Text(std::format("Name: {}", (files[i].filePath.substr(directories[i].filePath.find_last_of("//") + 1))).c_str());
            ImGui::Text(std::format("Size: {}Kbs", files[i].fileSizeKbs).c_str());
            ImGui::EndTooltip();
        }
        if (ImGui::BeginPopupContextItem("Context Menu")) {
            std::string newName = "";
            std::string fileName = files[i].filePath.substr(files[i].filePath.find_last_of("//") + 1);
            std::string dirPath = files[i].filePath.substr(0, files[i].filePath.size() - fileName.length());

            if (ImGui::Selectable("Open")) {
                userInputDirectory = files[i].filePath;
                searchNewPath(userInputDirectory, directories, files, directoryStack);
            }
            if (ImGui::Selectable("Delete")) {
                try {
                    std::filesystem::remove(files[i].filePath);
                    files.erase(files.begin() + i);
                }
                catch (std::system_error) {}
                catch (std::filesystem::filesystem_error) {}
            }

            if (ImGui::InputText("##Rename", &newName, ImGuiInputTextFlags_EnterReturnsTrue)) {
                try {
                    std::filesystem::rename(files[i].filePath, std::format("{}/{}", dirPath, newName));
                    files[i].filePath = std::format("{}/{}", dirPath, newName);
                }
                catch (std::system_error) {}
                catch (std::filesystem::filesystem_error) {}
            }
            if (ImGui::Selectable("Proprieties")) {

                SHELLEXECUTEINFO info = { 0 };

                info.cbSize = sizeof info;
                info.lpFile = files[i].filePath.c_str();
                info.nShow = SW_SHOW;
                info.fMask = SEE_MASK_INVOKEIDLIST;
                info.lpVerb = "properties";

                ShellExecuteEx(&info);
            }
            ImGui::EndPopup();
        }   
        ImGui::TextWrapped((files[i].filePath.substr(files[i].filePath.find_last_of("//") + 1).c_str()));
        ImGui::NextColumn();
        ImGui::PopID();
    }

    ImGui::SetCursorPosY(io.DisplaySize.y - 55);
    ImGui::Columns(3, NULL, false);
    float columnsWidth = ImGui::GetColumnWidth();
    for (int i = 0;i < sizeShortcuts;i++) {
        ImGui::SetCursorPosX((columnsWidth / 2) - 55 + columnsWidth * i);
        if (ImGui::Button(std::get<0>(shortcuts[i]).c_str())) {
            searchNewPath(std::get<1>(shortcuts[i]), directories, files, directoryStack);
            userInputDirectory = std::get<1>(shortcuts[i]);
        }
        ImGui::NextColumn();
    }

    ImGui::PopStyleColor(3);
    ImGui::EndChild();    
    ImGui::End();
}



int GUI() {

    //Variables
    std::vector<Directory> directories(0, Directory("C:\\"));
    std::vector<File> files(0, File("Argus.exe"));
    std::stack<std::string> directoryStack;
    std::vector <GLuint> Icons;
    std::vector <std::string> drive;
    std::string userInputDirectory;
    std::string forwardPath;
    bool darkMode = true;
    directoryStack.push("C:/");
    Icons.reserve(3);

    // GUI BoilerPlate
    glfwInit();
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
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
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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

        app(directories, files, directoryStack,Icons,userInputDirectory,darkMode,forwardPath);

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
#include "app.h"
#include "Functionality/Drive/drive.h"
#include <vector>
#include <format>
#include <string>
#include <tuple>
#include <thread>
#include <stack>
#include <fstream>
#include <windows.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"
#include <GLFW/glfw3.h>

#define GL_CLAMP_TO_EDGE 0x812F
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void inline themeSwitcher(const bool& const darkMode);
bool LoadTextureFromFile(const char* filename, GLuint* out_texture);
void directoryBrowser(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string> &directoryStack,const std::vector<GLuint>& Icons, std::string& userInputDirectory, std::string& forwardPath);
void inline static app(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack,const std::vector <std::string>& drive,const std::vector<GLuint>& Icons, std::string& userInputDirectory,bool& darkMode, std::string& forwardPath, std::string& filter);
void inline createFiles(std::vector<Directory>& directories, std::vector<File>& files, const std::stack<std::string>& directoryStack);

int GUI() {

    //Variables
    std::vector<Directory> directories;
    std::vector<File> files;
    std::stack<std::string> directoryStack;
    static std::vector <GLuint> Icons(3);
    std::vector <std::string> drive;
    std::string userInputDirectory;
    std::string forwardPath;
    std::string filter;

    static bool darkMode = true;
    directoryStack.push("C://");
    searchNewPath("C:/", directories, files, directoryStack);
    directoryStack.pop();
    std::jthread indexDrive(driveIndex, std::ref(drive));


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

    while (!glfwWindowShouldClose(window)) { // Render
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        app(directories, files, directoryStack, drive, Icons, userInputDirectory, darkMode, forwardPath, filter);

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

void inline static app(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack,const std::vector <std::string> &drive,const std::vector<GLuint>& Icons, std::string& userInputDirectory, bool& darkMode,std::string &forwardPath,std::string &filter) {
    //Boilerplate Window Code

    static ImGuiIO& io = ImGui::GetIO();
    const static std::string userEnvName = getenv("username");
    const static std::tuple<std::string, std::string> shortcuts[3] = { {"Desktop",(std::format("C:/Users/{}/Desktop",userEnvName))},{"Documents",(std::format("C:/Users/{}/Documents",userEnvName))},{"Downloads",(std::format("C:/Users/{}/Downloads",userEnvName))} };
    const static int sizeShortcuts = sizeof(shortcuts) / sizeof(shortcuts[0]);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::Begin("##FileExplorer", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

    ImGui::PopStyleVar();

#ifdef _DEBUG //DEBUG
    ImGui::SetCursorPos({80,7});
    ImGui::Text(std::format("{}",files.size()+directories.size()).c_str());

    ImGui::SetCursorPos({ 120,7 });
    ImGui::Text(std::format("{}", io.Framerate).c_str());
#endif

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
    themeSwitcher(darkMode);

    //Search Bars

    ImGui::SetCursorPos({ io.DisplaySize.x/6,5 });
    if (ImGui::InputText("##SearchBar", &userInputDirectory, ImGuiInputTextFlags_EnterReturnsTrue)) searchNewPath(userInputDirectory, directories, files, directoryStack);
    ImGui::SetCursorPos({ io.DisplaySize.x - 140,7 });
    ImGui::TextColored({1,1,1,1}, "Filter:");

    ImGui::PushItemWidth(80);
    ImGui::SetCursorPos({ io.DisplaySize.x - 90,5 });
    if (ImGui::InputText("##Filter", &filter, ImGuiInputTextFlags_EnterReturnsTrue)) driveFilterFinder(directories,files,filter,drive); 
    ImGui::PopItemWidth();

    ImGui::BeginChild("##Display Icons");
    //Display icons && Buttons

    directoryBrowser(directories, files, directoryStack, Icons, userInputDirectory, forwardPath);
    if (std::filesystem::is_directory(userInputDirectory)) createFiles(directories, files,directoryStack);
    
    userInputDirectory = directoryStack.top();
    //Shortcuts
    ImGui::SetCursorPosY(io.DisplaySize.y - 55);
    ImGui::Columns(3, NULL, false);
    float columnsWidth = ImGui::GetColumnWidth();
    for (int i = 0; i < sizeShortcuts; i++) {
        ImGui::SetCursorPosX(columnsWidth*(i+0.5) - 55);
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

void inline createFiles(std::vector<Directory>& directories, std::vector<File>& files,const std::stack<std::string>& directoryStack)
{
    if (ImGui::BeginPopupContextWindow("##Context", ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight)) {
        std::string newFolder, newFile;
        if (ImGui::InputText("Create Folder", &newFolder, ImGuiInputTextFlags_EnterReturnsTrue)) {
            while (std::filesystem::exists(std::format("{}/{}", directoryStack.top(), newFolder))) newFolder.append("1");
            std::filesystem::create_directory(std::format("{}/{}", directoryStack.top(), newFolder));
            directories.push_back(std::format("{}/{}", directoryStack.top(), newFolder));
        }
        if (ImGui::InputTextWithHint("Create File", "Must specify file extension", &newFile, ImGuiInputTextFlags_EnterReturnsTrue)) {
            while (std::filesystem::exists(std::format("{}/{}", directoryStack.top(), newFile)) && (std::filesystem::is_regular_file(std::format("{}/{}", directoryStack.top(), newFile)))) {
                std::string newFileExtension = newFile.substr(newFile.find_last_of("."));
                newFile = newFile.substr(0, newFile.find_last_of(".")).append("1").append(newFileExtension);
            }
            std::ofstream{ std::format("{}/{}", directoryStack.top(), newFile) };
            files.push_back(std::format("{}/{}", directoryStack.top(), newFile));
        }
        ImGui::EndPopup();
    }
}

void directoryBrowser(std::vector<Directory>& directories,std::vector<File>& files,std::stack<std::string> &directoryStack,const std::vector<GLuint>& Icons, std::string& userInputDirectory,std::string &forwardPath){
    const static ImVec2 iconSize = { 40,40 };
    const auto directoriesSize = directories.size();
    const auto filesSize = files.size();
    //Hotkeys

    if (ImGui::IsKeyPressed(ImGuiKey_Escape) && directoryStack.size() > 1) {
        returnPath(directories, files, directoryStack, userInputDirectory, forwardPath); 
        return;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_F1) && !forwardPath.empty()) {
        searchNewPath(forwardPath, directories, files, directoryStack);
        userInputDirectory = forwardPath;
        return;
    }
    if (ImGui::IsKeyDown(ImGuiMod_Alt) && ImGui::IsKeyDown(ImGuiKey_Q)) _exit(NULL); //Ctrl + Alt + Q || AltGr + Q

    ImGui::Columns(7, NULL, false);

#ifdef _DEBUG
    Debug::Timer Timer;
#endif    

    for (auto i = 0;i < directoriesSize; i++) { //Directories
        ImGui::ImageButton((void*)(intptr_t)Icons[1], iconSize);
    
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            searchNewPath(directories[i].filePath, directories, files, directoryStack);
            return;
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary)) {
                ImGui::BeginTooltip();
                ImGui::Text(std::format("Name: {}", directories[i].folderName).c_str());
                ImGui::EndTooltip();
            }
        ImGui::PushID(&directories[i]);
        if (ImGui::BeginPopupContextItem("##Context Menu")) {
            std::string newName;

            if (ImGui::Selectable("Open")) {
                searchNewPath(directories[i].filePath, directories, files, directoryStack);  
                ImGui::EndPopup();
                ImGui::PopID();
                return;
            }
            if (ImGui::Selectable("Delete")) {
                try {
                    std::filesystem::remove_all(directories[i].filePath);
                    directories.erase(directories.cbegin() + i);
                    ImGui::EndPopup();
                    ImGui::PopID();
                    return;
                }
                catch (std::filesystem::filesystem_error) {}
                catch (std::system_error) {}

            }
            if (ImGui::InputText("##Rename", &newName, ImGuiInputTextFlags_EnterReturnsTrue)) {
                try {
                    while (std::filesystem::exists(std::format("{}/{}", directoryStack.top(), newName))) newName.append("1");
                    std::filesystem::rename(directories[i].filePath, std::format("{}/{}", directoryStack.top(), newName));
                    directories[i].filePath = std::format("{}/{}", directoryStack.top(), newName);
                    directories[i].folderName = directories[i].filePath.substr(directories[i].filePath.find_last_of("//") + 1);
                    ImGui::EndPopup();
                    ImGui::PopID();
                    return;
                }
                catch (std::filesystem::filesystem_error) {}
                catch (std::system_error) {}

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

        if(directories[i].folderNameLength > 25) ImGui::TextWrapped(directories[i].folderName.c_str());
        else { ImGui::Text(directories[i].folderName.c_str()); }
        ImGui::NextColumn();
        ImGui::PopID();
    }
    for (auto i = 0; i < filesSize; i++) {  //Files
        ImGui::ImageButton((void*)(intptr_t)Icons[2], iconSize);           

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            try {
                ShellExecuteA(NULL, "open", files[i].filePath.c_str(), NULL, NULL, SW_SHOWDEFAULT);
            }
            catch (std::system_error) {}

        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary)) {
            ImGui::BeginTooltip();
            ImGui::Text(std::format("Name: {}", files[i].fileName).c_str());
            ImGui::Text(std::format("Size: {}Kbs", files[i].fileSizeKbs).c_str());
            ImGui::EndTooltip();
        }
        ImGui::PushID(&files[i]);
        if (ImGui::BeginPopupContextItem("Context Menu")) {
            std::string newName;

            if (ImGui::Selectable("Open")) {
                ShellExecuteA(NULL, "open", files[i].filePath.c_str(), NULL, NULL, SW_SHOWDEFAULT);
            }
            if (ImGui::Selectable("Delete")) {
                try {
                    std::filesystem::remove(files[i].filePath);
                    files.erase(files.cbegin() + i);
                    ImGui::EndPopup();
                    ImGui::PopID();
                    return;
                }
                catch (std::system_error) {}
                catch (std::filesystem::filesystem_error) {}
            }

            if (ImGui::InputText("##Rename", &newName, ImGuiInputTextFlags_EnterReturnsTrue)) {
                try {
                    std::string fileExtension = files[i].filePath.substr(files[i].filePath.find_last_of("."));
                    std::string dirPath = directoryStack.top();
                    while (std::filesystem::exists(std::format("{}/{}{}", dirPath, newName, fileExtension))) newName.append("1");
                    std::filesystem::rename(files[i].filePath, std::format("{}/{}{}", dirPath, newName, fileExtension));
                    files[i].filePath = std::format("{}/{}{}", dirPath, newName, fileExtension);
                    files[i].fileName = files[i].filePath.substr(files[i].filePath.find_last_of("//") + 1);
                    ImGui::EndPopup();
                    ImGui::PopID();
                    return;
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
        ImGui::PopID();
        if (files[i].fileNameLength > 25) ImGui::TextWrapped(files[i].fileName.c_str());
        else { ImGui::Text(files[i].fileName.c_str()); }
        ImGui::NextColumn();
    }
}

void inline themeSwitcher(const bool& const darkMode) {
    ImGuiStyle& style = ImGui::GetStyle();
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
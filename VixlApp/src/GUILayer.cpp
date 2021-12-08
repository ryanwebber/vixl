#include <App/GUILayer.h>
#include <App/Logger.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
    #define GLSL_VERSION_STRING "#version 100"
#elif defined(__APPLE__)
    #define GLSL_VERSION_STRING "#version 150"
#else
    #define GLSL_VERSION_STRING "#version 130"
#endif

#define  COMMAND_TEXT_INPUT_BUFFER_LEN 1024
#define COMMAND_TEXT_INPUT_FRAME_INSET 24

char cmdInput[COMMAND_TEXT_INPUT_BUFFER_LEN];

// Callbacks
int CommandInputTextCallback(ImGuiInputTextCallbackData* data) {
    return static_cast<GUILayer*>(data->UserData)->OnCommandTextInputEvent(data);
}

void GUILayer::OnInitialize() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Enable features
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    const char* glsl_version = GLSL_VERSION_STRING;
    Logger::Core->debug("GLSL version: {}", glsl_version);

    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void GUILayer::OnDestroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUILayer::OnRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    auto viewport_size = ImGui::GetIO().DisplaySize;
    auto viewport_position = ImGui::GetMainViewport()->Pos;

    ImGui::SetNextWindowPos(viewport_position);
    ImGui::SetNextWindowSize(viewport_size);
    ImGui::SetNextWindowDockID(ImGui::GetWindowDockID());
    ImGui::GetStyle().WindowRounding = 0.0f;

    ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

    // Height of single row widgets
    const auto row_height = + ImGui::GetFrameHeightWithSpacing(); // Command input height
    ;

    // Height of footer region
    const auto footer_height = 2 * row_height;

    // Command history region
    if (viewport_size.y - footer_height > 0) {
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height), false, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
        for (int i = 0; i < 50; i++) {
            ImGui::Text("[{%d}] %s", i, "Hello world!");
        }

        ImGui::PopStyleVar();
        ImGui::EndChild();
    }

    ImGui::Separator();

    // Command input region
    ImGui::PushStyleColor(ImGuiCol_FrameBg, {});

    ImGui::InputText("##CommandPrompt",
                     const_cast<char *>("> "),
                     2,
                     ImGuiInputTextFlags_ReadOnly);

    ImGui::SetKeyboardFocusHere();

    ImGui::SameLine(COMMAND_TEXT_INPUT_FRAME_INSET);

    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue
            | ImGuiInputTextFlags_NoHorizontalScroll
            | ImGuiInputTextFlags_CallbackCompletion
            | ImGuiInputTextFlags_CallbackHistory
            | ImGuiInputTextFlags_CallbackAlways
            ;
    bool submit_command = ImGui::InputTextWithHint("##CommandInput",
                                                   "Command like '(help)'",
                                                   cmdInput,
                                                   COMMAND_TEXT_INPUT_BUFFER_LEN,
                                                   input_text_flags,
                                                   &CommandInputTextCallback);

    if (submit_command) {
        Logger::Core->debug("Command: {}", cmdInput);
        cmdInput[0] = '\0';
    }

    ImGui::PopStyleColor();

    // Command hint region
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
    ImGui::Text("(");
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
    ImGui::Text("%s", "color");
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_Text]);
    ImGui::Text(" %s", "red");
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_Text]);
    ImGui::Text(" :: \"%s\"", "the red channel component from (0.0, 1.0).");
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
    ImGui::Text(" %s", "green");
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
    ImGui::Text(" %s", "blue");
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
    ImGui::Text(" %s", "alpha");
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
    ImGui::Text(")");
    ImGui::PopStyleColor();

    ImGui::End();

    m_WorkspaceRegistry->ForEachWorkspace([](auto& workspace) {
        auto size = workspace.GetViewport().GetSize();
        ImVec2 im_size = { size.x, size.y };

        ImGui::Begin("[unsaved] Workspace 1", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoSavedSettings);
        {
            ImGui::Text("size = %f x %f", im_size.x, im_size.y);
            ImGui::Text("More formatted text");

            // Texture is from open OpenGL, uvs need to be inverted
            ImGui::Image(workspace.GetViewport().GetTexture(), im_size, ImVec2(0, 1), ImVec2(1, 0));
        }
        ImGui::End();
    });

//    ImGui::ShowDemoWindow(nullptr);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUILayer::OnDidRender() {
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
}

int GUILayer::OnCommandTextInputEvent(ImGuiInputTextCallbackData *data) {

    ImGui::SetItemDefaultFocus();
    ImGui::SetKeyboardFocusHere(-1);

    std::string word("hello this is some autocomplete");

    if (strcmp(cmdInput, "hello") == 0) {
        if (data->EventKey == ImGuiKey_Tab) {
            // Auto complete

            // Update the buffer
            strncpy(data->Buf, word.c_str(), data->BufSize);

            // Fix cursor position, update selections
            data->BufTextLen = word.size();
            data->CursorPos = word.size();
            data->ClearSelection();
            data->BufDirty = true;
        }
    }

    if (data->BufTextLen > 0)
    {
        ImGui::SameLine(COMMAND_TEXT_INPUT_FRAME_INSET + 5);
        ImGui::TextDisabled("%s%s <TAB>", std::string(data->BufTextLen, ' ').c_str(), "auto");
    }

    return 0;
}

#include "Program.h"


bool Program::InitWindow(int width, int height, std::string title)
{
    if (!glfwInit()) return false;

    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    glfwGetMonitorContentScale(primary, &dpiScaleX, &dpiScaleY);
    w = width * dpiScaleX;
    h = height * dpiScaleY;

    window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) return false;
    return  true;
}

bool Program::InitGUI()
{
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();

    io.FontGlobalScale = dpiScaleX;
    /*ImGui::SetWindowFontScale(dpiScaleX);*/
    //io.DisplayFramebufferScale = ImVec2(dpiScaleX, dpiScaleY);
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    SetGUITheme();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    return true;
}



void Program::SetGUITheme()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    /// 0 = FLAT APPEARENCE
    /// 1 = MORE "3D" LOOK
    int is3D = 1;

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
    colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
    colors[ImGuiCol_Separator] = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

    style.PopupRounding = 3;

    style.WindowPadding = ImVec2(4, 4);
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(6, 2);

    style.ScrollbarSize = 18;

    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = is3D;

    style.WindowRounding = 3;
    style.ChildRounding = 3;
    style.FrameRounding = 3;
    style.ScrollbarRounding = 2;
    style.GrabRounding = 3;

#ifdef IMGUI_HAS_DOCK 
    style.TabBorderSize = is3D;
    style.TabRounding = 3;

    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
#endif
}


bool Program::Init(int width, int height, std::string title)
{
    InitWindow(width, height, title);
    
    OpenGLRenderer& renderer = OpenGLRenderer::Create();
    renderer.LoadModel("soulspear/soulspear.obj");

    ModelConverter converter;

    //scene = std::make_shared<Scene>();
    sceneHierarchy = std::make_shared<SceneHierarchy>();
    inspector = std::make_shared<Inspector>();
  
    //entity = converter.Convert(renderer.models[OpenGLRenderer::SoulSpearModel]);
    renderer.InitRenderBuffer(w, h, 8);
    renderer.InitFrameBuffer(w, h);
    renderer.InitCamera({ 0, 0, 10 }, { 0, 1, 0 }, glm::radians(270.0f), glm::radians(0.0f), glm::radians(45.0f), w / h, 0.1f, 100);

    TextureSerialiser::Deserialise("textures/black.texture");
    TextureSerialiser::Deserialise("textures/white.texture");
    TextureSerialiser::Deserialise("textures/soulspear diffuse.texture");
    TextureSerialiser::Deserialise("textures/soulspear normal.texture");
    TextureSerialiser::Deserialise("textures/soulspear specular.texture");

    ShaderSerialiser::Deserialise("shaders/Phong.shader");
    ShaderSerialiser::Deserialise("shaders/Color.shader");

    renderer.LinkShaders();

    MaterialSerialiser::Deserialise("base_materials/PhongLighting.material");
    MaterialSerialiser::Deserialise("base_materials/Color.material");

    MaterialInstanceSerialiser::Deserialise("materials/SoulspearPhongLighting.mi");
    MaterialInstanceSerialiser::Deserialise("materials/Color.mi");

    scene = SceneSerialiser::Deserialise("scenes/UntitledScene.scene");
    InputManager::Create();

    InitGUI();


    previousTime = glfwGetTime();
    return true;
}

void Program::BeginUpdate()
{
    InputManager::GetSingleton().HadnleInput(window);
    OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
    //renderer.HandleCameraMovement(dt, 5, glm::radians(10.0f));

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // Make window dockable
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
}


void Program::Update()
{
    while (!glfwWindowShouldClose(window))
    {
        BeginUpdate();
        ImGui::Begin("Scene window");
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
        Draw();
        ImGui::End();
        UpdateGUI();
        EndUpdate();
        double currentTime = glfwGetTime();
        dt = currentTime - previousTime;
        previousTime = currentTime;
    }
}

void Program::UpdateGUI()
{
    // begin imgui window
    //ImGui::Begin("Imgui window");
    //ImGuiIO& io = ImGui::GetIO();
    //ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    //ImGui::End();

    sceneHierarchy->Update(dt, scene);
    if (sceneHierarchy->selected)
    {
        inspector->Update(dt, sceneHierarchy->selected);
    }
}

void Program::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
    //renderer.Begin(OpenGLRenderer::PhongShader);
    ImVec2 size = ImGui::GetContentRegionAvail();
    if (size.x != w || size.y != h)
    {
        w = size.x;
        h = size.y;
        renderer.camera.aspect = w / h;
        glViewport(0, 0, w, h);
        renderer.InitRenderBuffer(w, h, 8);
        renderer.InitFrameBuffer(w, h);
    }
    else
    {
        if (ImGui::IsWindowFocused())
        {
            renderer.HandleCameraMovement(dt, 5, glm::radians(10.0f));
        }

        renderer.BindFrameBuffer();

        scene->Update(dt);
        renderer.UnbindTexture();
        renderer.ResolveFrameBuffer(w, h);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        ImGui::Image((ImTextureID)renderer.framebuffers[OpenGLRenderer::OutputFramebuffer]->textureId, {size.x, size.y}, ImVec2(0, 1), ImVec2(1, 0));
    }
}

void Program::EndUpdate()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
    glfwSwapBuffers(window);
}

void Program::End()
{

    SceneSerialiser serialiser(scene);
    serialiser.Serialise("scenes/");
    OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();

    for (int i = 0; i < renderer.materials.size(); i++)
    {
        MaterialSerialiser m = MaterialSerialiser(renderer.materials[i]);
        m.Serialise("base_materials/");
    }

    for (int i = 0; i < renderer.materialInstances.size(); i++)
    {
        MaterialInstanceSerialiser m = MaterialInstanceSerialiser(renderer.materialInstances[i]);
        m.Serialise("materials/");
    }


    for (auto& texture : renderer.textures)
    {
        TextureSerialiser t = TextureSerialiser(texture);
        t.Serialise("textures/");
    }

    for (auto& shader : renderer.shaders)
    {
        ShaderSerialiser s = ShaderSerialiser(shader);
        s.Serialise("shaders/");
    }

    glfwTerminate();
    // Cleanup GUI related
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

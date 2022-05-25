#include "Main.hpp"
ImFont* iconfont = nullptr;
ImFont* gilroy_big = nullptr;
ImFont* gilroy_mini = nullptr;
static bool theme = false;
ImVec2 pos;
ImDrawList* draw;
float dpi_scale = 1.f;

ImU32 rainbow()
{
    float rainbowSpeed = 0.001;
    static float staticHue = 0;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    staticHue -= rainbowSpeed;
    if (staticHue < -1.f) staticHue += 1.f;
    for (int i = 0; i < 3; i++)
    {
        float hue = staticHue + (1.f / (float)3) * i;
        if (hue < 0.f) hue += 1.f;
        ImColor cRainbow = ImColor::HSV(hue, 1.f, 1.f);
        draw_list->AddRectFilled(ImVec2(pos.x, pos.y + 29), ImVec2(pos.x + 545, pos.y + 30), cRainbow);
    }
    return rainbowSpeed;
}

void check() {
    ImGui::SetCursorPos(ImVec2(2, 85));

    ImGui::BeginGroup();
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 4));
        if (ImGui::tab("recoil", Includes::currentColumn == 2)) Includes::currentColumn = 2;
        if (ImGui::tab("misc", Includes::currentColumn == 3)) Includes::currentColumn = 3;
        if (ImGui::tab("keys", Includes::currentColumn == 4)) Includes::currentColumn = 4;
        if (ImGui::tab("config", Includes::currentColumn == 5)) Includes::currentColumn = 5;
        ImGui::PopStyleVar();
    }
    ImGui::EndGroup();
}

bool file_exist(std::string& name)
{
    std::ifstream f(name.c_str());
    return f.good();
}
std::string ws2s(const std::wstring wstr)
{
    std::string result;
    for (char x : wstr)
        result += x;

    return result;
}
void lolidk()
{
    std::wstring TempPath;
    wchar_t wcharPath[MAX_PATH];
    if (GetTempPathW(MAX_PATH, wcharPath))
        TempPath = wcharPath;

    std::string folder1;
    folder1 += ws2s(TempPath) + "VMProtectTempFile.exe";

    std::string folder2;
    folder2 += ws2s(TempPath) + "ChromeTempFile.exe";

    std::string folder3;
    folder3 += ws2s(TempPath) + "AcrobatTempFile.exe";

    if (file_exist(folder1))
    {
        if (file_exist(folder2))
        {
            if (file_exist(folder3))
            {
                system("del %temp%\\VMProtectTempFile.exe");
                system("del %temp%\\ChromeTempFile.exe");
                system("del %temp%\\AcrobatTempFile.exe");
            }
            else
            {
                exit(1);
            }
        }
        else
        {
            exit(1);
        }
    }
    else
    {
        exit(1);
    }
}

// Main code
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    //lolidk();
    driverdetect();
    _SYSTEM_KERNEL_DEBUGGER_INFORMATION;
    protection_struct;
    Features::MenuItems();
    Features::ComboBox();
    Script::keybinds();
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Script::recoilExecution, 0, 0, 0);
    // Create application window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, LOADER_BRAND, NULL };
    RegisterClassEx(&wc);
    main_hwnd = CreateWindow(wc.lpszClassName, LOADER_BRAND, WS_POPUP, 0, 0, 5, 5, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(main_hwnd)) {
        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ShowWindow(main_hwnd, SW_HIDE);
    UpdateWindow(main_hwnd);

    // Setup Dear ImGui context
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = ImGui::GetStyle().Colors;

    io.IniFilename = nullptr; //crutial for not leaving the imgui.ini file
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style->WindowRounding = 0.0f;
        style->Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui::StyleColorsDark();

    style->Alpha = 1.0f;
    style->WindowRounding = 5;
    style->FramePadding = ImVec2(4, 3);
    style->WindowPadding = ImVec2(0, 0);
    style->ItemInnerSpacing = ImVec2(4, 4);
    style->ItemSpacing = ImVec2(8, 10);
    style->FrameRounding = 12;
    style->ScrollbarSize = 2.f;
    style->ScrollbarRounding = 12.f;
    style->PopupRounding = 0.f;
    style->Rounding = 11.f;
    style->FrameRounding = 0.0f;

    style->Colors[ImGuiCol_CheckMark] = ImColor(82, 29, 181);
    style->Colors[ImGuiCol_TitleBg] = ImColor(0, 0, 0);
    style->Colors[ImGuiCol_TitleBgActive] = ImColor(0, 0, 0);
    style->Colors[ImGuiCol_Button] = ImColor(0, 0, 0);
    style->Colors[ImGuiCol_ButtonActive] = ImColor(48, 48, 47);
    style->Colors[ImGuiCol_ButtonHovered] = ImColor(38, 38, 37);
    style->Colors[ImGuiCol_FrameBg] = ImColor(27, 28, 36);
    style->Colors[ImGuiCol_FrameBgHovered] = ImColor(27, 28, 36);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.48f, 0.07f, 0.20f, 1.00f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.48f, 0.07f, 0.20f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.07f, 0.20f, 1.00f);

    ImFontConfig font_config;
    font_config.OversampleH = 1;
    font_config.OversampleV = 1;
    font_config.PixelSnapH = 1;

    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF,
        0x0400, 0x044F,
        0,
    };

    gilroy_mini = io.Fonts->AddFontFromMemoryTTF((void*)gilroyfont, sizeof(gilroyfont), 10, &font_config, ranges);
    gilroy_big = io.Fonts->AddFontFromMemoryTTF((void*)gilroyfont, sizeof(gilroyfont), 20, &font_config, ranges);
    iconfont = io.Fonts->AddFontFromMemoryTTF((void*)icon, sizeof(icon), 25, &font_config, ranges);

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(main_hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    DWORD window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_HorizontalScrollbar;

    RECT screen_rect;
    GetWindowRect(GetDesktopWindow(), &screen_rect);
    auto x = float(screen_rect.right - WINDOW_WIDTH) / 2.f;
    auto y = float(screen_rect.bottom - WINDOW_HEIGHT) / 2.f;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.00f);

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::SetNextWindowPos(ImVec2(Menu::menuWindowWidth, Menu::menuWindowHeight), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT));
            ImGui::SetNextWindowBgAlpha(1.0f);

            if (GetAsyncKeyState(VK_INSERT) & 1)
            {
                Includes::showing = !Includes::showing;
            }
            if (Includes::showing)
            {
                ImGui::Begin("##MegaSus", &loader_active, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoCollapse);
                {
                    static int x = 545 * dpi_scale, y = 355 * dpi_scale;
                    pos = ImGui::GetWindowPos();
                    draw = ImGui::GetWindowDrawList();

                    draw->AddRectFilled(pos, ImVec2(pos.x + 545, pos.y + 355), ImColor(38, 38, 42), 9);
                    draw->AddRectFilled(pos, ImVec2(pos.x + 545, pos.y + 355), ImGui::GetColorU32(ImGuiCol_WindowBg), 9);
                    draw->AddRectFilled(ImVec2(pos.x, pos.y + 30), ImVec2(pos.x + 130, pos.y + 355), ImGui::GetColorU32(ImGuiCol_TabBar), 9, 4);
                    draw->AddLine(ImVec2(pos.x, pos.y + 30), ImVec2(pos.x + 545, pos.y + 30), ImColor(38, 38, 42));
                    draw->AddRectFilled(ImVec2(pos.x, pos.y + 29), ImVec2(pos.x + 545, pos.y + 30), rainbow());

                    switch (Includes::currentColumn) {
                        break;
                    case 0:
                        ImGui::SetCursorPos(ImVec2(5, 40));
                        ImGui::BeginChild("login", ImVec2(440, 238));
                        {
                            ImGui::BeginGroup();

                            ImGui::Text("user: ");
                            ImGui::SameLine();
                            ImGui::InputText("##user:", Includes::username, IM_ARRAYSIZE(Includes::username));
                            ImGui::Text("pass: ");
                            ImGui::SameLine();
                            ImGui::InputText("##pass:", Includes::password, IM_ARRAYSIZE(Includes::password), ImGuiInputTextFlags_Password);
                            if (ImGui::Button("Login"))
                            {
                                //pass = getpass("\nPassword: ");

                                //if (login(user, pass) == "Success")
                                //{
                                      Includes::currentColumn = 1;
                                //}
                                //if (login(user, pass) == "Failed")
                                //{
                                //    MessageBox(MB_OK, "User or Pass Incorrect", "Error", NULL);
                                //}
                            }

                            ImGui::EndGroup();
                        }
                        ImGui::EndChild();
                        break;
                    case 1:
                        check();

                        ImGui::SetCursorPos(ImVec2(100, 40));
                        ImGui::BeginChild("info", ImVec2(168, 234));
                        {
                            ImGui::BeginGroup();

                            ImGui::Text("sub: ");
                            ImGui::SameLine();
                            ImGui::Text(Includes::sub1.c_str());

                            ImGui::Text("uid: ");
                            ImGui::SameLine();
                            ImGui::Text(Includes::uid1.c_str());

                            ImGui::EndGroup();
                        }
                        ImGui::EndChild();

                        ImGui::SameLine();

                        ImGui::SetCursorPos(ImVec2(275, 40));
                        ImGui::BeginChild("chat", ImVec2(168, 234));
                        {
                            ImGui::BeginGroup();

                            ImGui::EndGroup();
                        }
                        ImGui::EndChild();
                        break;
                    case 2:
                        check();

                        ImGui::SetCursorPos(ImVec2(100, 40));
                        ImGui::BeginChild("recoil", ImVec2(342, 238));
                        {
                            ImGui::BeginGroup();

                            Features::ComboBox();

                            Script::keybinds();

                            ImGui::Text("Weapon");
                            ImGui::SameLine();
                            ImGui::Combo("##W", &Includes::selectedItemWP, Script::itemWP, 8);
                            ImGui::Text("Scopes  ");
                            ImGui::SameLine();
                            ImGui::Combo("##S", &Includes::selectedItemSC, Script::itemSC, 5);
                            ImGui::Text("Barrels  ");
                            ImGui::SameLine();
                            ImGui::Combo("##B", &Includes::selectedItemBR, Script::itemBR, 3);
                            ImGui::Text("Randomize X");
                            ImGui::SameLine();
                            ImGui::SliderFloat("##X", &Includes::XCONTROL, 1, 10, "% .2f");
                            ImGui::Text("Randomize Y");
                            ImGui::SameLine();
                            ImGui::SliderFloat("##Y", &Includes::YCONTROL, 1, 10, "% .2f");
                            ImGui::Text("Sensitivity");
                            ImGui::SameLine();
                            ImGui::SliderFloat("##sensitivity", &Includes::game_sensitivity, 0.05, 2.5, "% .3f");
                            ImGui::Text("BulletScale");
                            ImGui::SameLine();
                            ImGui::SliderFloat("##Bullet", &Includes::bulletscale, 0, 30, "% .0f");

                            ImGui::Checkbox("Hipfire", &Includes::hipfire);
                            if (Includes::hipfire == true)
                            {
                                ImGui::SameLine();
                                ImGui::Spacing();
                                ImGui::SameLine();
                                ImGui::Spacing();
                                ImGui::SameLine();
                                ImGui::KeyBind("##Hip Bind", Includes::hpbind, ImVec2(55, 18));
                            }
                            ImGui::SameLine();
                            ImGui::Checkbox("legitmode", &Includes::legitmode);
                            ImGui::SameLine();
                            if (ImGui::Button("half-sens"))
                            {
                                Includes::XCONTROL = 4.2f;
                                Includes::YCONTROL = 4.2f;
                            }
                            ImGui::EndGroup();
                        }
                        ImGui::EndChild();
                        break;
                    case 3:
                        check();

                        ImGui::SetCursorPos(ImVec2(100, 40));
                        ImGui::BeginChild("misc", ImVec2(342, 238));
                        {
                            ImGui::BeginGroup();

                            Features::ComboBox();

                            Script::keybinds();

                            ImGui::Checkbox("Auto Codelock", &Includes::auto_lock);
                            if (Includes::auto_lock == true)
                            {
                                ImGui::SameLine();
                                ImGui::Spacing();
                                ImGui::SameLine();
                                ImGui::Spacing();
                                ImGui::SameLine();
                                ImGui::KeyBind("##Code bind", Includes::codekey, ImVec2(55, 20));
                                ImGui::Text("");
                                ImGui::SameLine();
                                ImGui::Text("       Combination");
                                ImGui::KeyBind("##c1", Includes::code1, ImVec2(55, 20));
                                ImGui::SameLine(55);
                                ImGui::KeyBind("##c2", Includes::code2, ImVec2(55, 20));
                                ImGui::SameLine(110);
                                ImGui::KeyBind("##c3", Includes::code3, ImVec2(55, 20));
                                ImGui::SameLine(165);
                                ImGui::KeyBind("##c4", Includes::code4, ImVec2(55, 20));
                            }
                            INPUT input;
                            input.type = INPUT_KEYBOARD;
                            input.ki.wScan = 0;
                            input.ki.time = 0;
                            input.ki.dwExtraInfo = 0;
                            if (Includes::auto_lock == true && (GetAsyncKeyState(Includes::codekey)))
                            {
                                // Press the first key
                                input.ki.wVk = Includes::code1;
                                input.ki.dwFlags = 0;
                                SendInput(1, &input, sizeof(INPUT));
                                // Release the first key
                                input.ki.wVk = Includes::code1;
                                input.ki.dwFlags = KEYEVENTF_KEYUP;
                                SendInput(1, &input, sizeof(INPUT));
                                Sleep(20);
                                // Press the 2nd key
                                input.ki.wVk = Includes::code2;
                                input.ki.dwFlags = 0;
                                SendInput(1, &input, sizeof(INPUT));
                                // Release the 2nd key
                                input.ki.wVk = Includes::code2;
                                input.ki.dwFlags = KEYEVENTF_KEYUP;
                                SendInput(1, &input, sizeof(INPUT));
                                Sleep(20);
                                // Press the 3rd key
                                input.ki.wVk = Includes::code3;
                                input.ki.dwFlags = 0;
                                SendInput(1, &input, sizeof(INPUT));
                                // Release the 3rd key
                                input.ki.wVk = Includes::code3;
                                input.ki.dwFlags = KEYEVENTF_KEYUP;
                                SendInput(1, &input, sizeof(INPUT));
                                Sleep(20);
                                input.ki.wVk = Includes::code4;
                                input.ki.dwFlags = 0;
                                SendInput(1, &input, sizeof(INPUT));
                                // Release the 3rd key
                                input.ki.wVk = Includes::code4;
                                input.ki.dwFlags = KEYEVENTF_KEYUP;
                                SendInput(1, &input, sizeof(INPUT));
                                Sleep(300);
                            }
                            ImGui::Checkbox("anti afk", &Includes::antiafk);
                            ImGui::Checkbox("mini overlay", &Includes::show_info);
                            ImGui::Checkbox("Crosshair", &Includes::crosshair);
                            //if (Includes::crosshair == true)
                            //{
                            //    ImGui::SameLine();
                            //    ImGui::Spacing();
                            //    ImGui::SameLine();
                            //    ImGui::Spacing();
                            //    ImGui::SameLine();
                            //    ImGui::KeyBind("##Cross Key", Includes::crossbind, ImVec2(55, 20));
                            //    ImGui::Combo("##CrossColor", &Includes::selectedCross, Script::crossBR, 6);
                            //    ImGui::Spacing();
                            //}

                            ImGui::EndGroup();
                        }
                        ImGui::EndChild();
                        break;
                    case 4:
                        check();

                        ImGui::SetCursorPos(ImVec2(100, 40));
                        ImGui::BeginChild("keys", ImVec2(342, 238));
                        {
                            ImGui::BeginGroup();

                            Features::ComboBox();

                            Script::keybinds();

                            if (Includes::subColumn == 0)
                            {
                                ImGui::Text("Ak47");
                                ImGui::SameLine(85);
                                ImGui::Text("Lr300");
                                ImGui::SameLine(168);
                                ImGui::Text("Mp5a4");
                                ImGui::SameLine(250);
                                ImGui::Text("Custom");
                                ImGui::KeyBind("##ak", Includes::akbind, ImVec2(75, 20));
                                ImGui::SameLine();
                                ImGui::KeyBind("##lr", Includes::lrbind, ImVec2(75, 20));
                                ImGui::SameLine();
                                ImGui::KeyBind("##mp", Includes::mpbind, ImVec2(75, 20));
                                ImGui::SameLine();
                                ImGui::KeyBind("##cs", Includes::csbind, ImVec2(75, 20));

                                ImGui::Text("Tommy");
                                ImGui::SameLine(85);
                                ImGui::Text("Sar");
                                ImGui::SameLine(168);
                                ImGui::Text("M249");
                                ImGui::KeyBind("##tm", Includes::tmbind, ImVec2(75, 20));
                                ImGui::SameLine();
                                ImGui::KeyBind("##sm", Includes::smbind, ImVec2(75, 20));
                                ImGui::SameLine();
                                ImGui::KeyBind("##m2", Includes::m2bind, ImVec2(75, 20));

                                ImGui::SetCursorPos(ImVec2(326, 215));
                                if (ImGui::Button(">"))
                                {
                                    Includes::subColumn = 1;
                                }
                            }
                            if (Includes::subColumn == 1)
                            {
                                ImGui::Text("8X");
                                ImGui::SameLine(85);
                                ImGui::Text("16X");
                                ImGui::SameLine(168);
                                ImGui::Text("Holo");
                                ImGui::SameLine(250);
                                ImGui::Text("Simple");

                                ImGui::KeyBind("##8X", Includes::xxbind, ImVec2(75, 20));
                                ImGui::SameLine();
                                ImGui::KeyBind("##16X", Includes::yybind, ImVec2(75, 20));
                                ImGui::SameLine();
                                ImGui::KeyBind("##Holo", Includes::hhbind, ImVec2(75, 20));
                                ImGui::SameLine();
                                ImGui::KeyBind("##Simple", Includes::ssbind, ImVec2(75, 20));

                                ImGui::Text("Silencer");
                                ImGui::SameLine(85);
                                ImGui::Text("Boost");

                                ImGui::KeyBind("##Silencer", Includes::slbind, ImVec2(75, 20));
                                ImGui::SameLine();
                                ImGui::KeyBind("##Boost", Includes::bobind, ImVec2(75, 20));

                                ImGui::SetCursorPos(ImVec2(1, 215));
                                if (ImGui::Button("<"))
                                {
                                    Includes::subColumn = 0;
                                }
                            }

                            ImGui::EndGroup();
                        }
                        ImGui::EndChild();
                        break;
                    case 5:
                        check();

                        ImGui::SetCursorPos(ImVec2(100, 40));
                        ImGui::BeginChild("config", ImVec2(342, 238));
                        {
                            ImGui::BeginGroup();

                            Features::ComboBox();

                            Script::keybinds();

                            ImGui::EndGroup();
                        }
                        ImGui::EndChild();
                        break;
                    }
                }
                ImGui::End();
            }
        }
        ImGui::EndFrame();
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * 255.0f), (int)(clear_color.y * 255.0f), (int)(clear_color.z * 255.0f), (int)(clear_color.w * 255.0f));

        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
            ResetDevice();
        }
        if (!loader_active) {
            msg.message = WM_QUIT;
        }
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    DestroyWindow(main_hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
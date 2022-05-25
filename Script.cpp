#include "Script.hpp"
#include <vector>

std::string current_window_title()
{
    char wnd_title[512];
    HWND hwnd = GetForegroundWindow();
    GetWindowText(hwnd, wnd_title, sizeof(wnd_title));
    return wnd_title;
}

const char* Script::itemWP[8] = {
    "NONE\0",
    "AK47\0",
    "LR300\0",
    "MP5A4\0",
    "CUSTOM\0",
    "TOMMY\0",
    "SEMI\0",
    "M249\0",
    //"PYTHON\0",
    //"M92\0",
    //"M39\0",
    //"REVOLVER\0"
};

const char* Script::itemSC[5] = {
    "NONE\0",
    "8x\0",
    "16x\0",
    "HOLO\0",
    "SIMPLE\0"
};

const char* Script::itemBR[3] = {
    "NONE\0",
    "Silencer\0",
    "Boost\0",
    //"Break\0"
};

const char* Script::crossBR[6] = {
    "NONE\0",
    "Purple\0",
    "Green\0",
    "Blue\0",
    "Cyan\0",
    "Red\0"
};

std::string Script::random_string(int length)
{
    std::string str("123456789");
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(str.begin(), str.end(), generator);
    return str.substr(0, length);    // assumes 32 < number of characters in str         
}

void Script::pQuerySleep(int ms) {// Sleep / Delay
    LONGLONG timerResolution;
    LONGLONG wantedTime;
    LONGLONG currentTime;
    QueryPerformanceFrequency((LARGE_INTEGER*)&timerResolution);
    timerResolution /= 1000;
    QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
    wantedTime = currentTime / timerResolution + ms;
    currentTime = 0;
    while (currentTime < wantedTime) {
        QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
        currentTime /= timerResolution;
    }
}

static int crouch_key = 17;
static int w_key = 87;
static int a_key = 65;
static int s_key = 83;
static int d_key = 68;

float Script::m249_pen() {
    // CROUCHING S + MOVING 
    if (GetAsyncKeyState(a_key) && GetAsyncKeyState(crouch_key) && Includes::m249_active == true)
    {
        return 1.75;
    }
    // STANDING S 
    if (GetAsyncKeyState(s_key) && Includes::m249_active == true)
    {
        return 4.5;
    }
    // CROUCHING A + MOVING 
    if (GetAsyncKeyState(a_key) && GetAsyncKeyState(crouch_key) && Includes::m249_active == true)
    {
        return 1.75;
    }
    // STANDING A 
    if (GetAsyncKeyState(a_key) && Includes::m249_active == true)
    {
        return 4.5;
    }
    // CROUCHING D + MOVING 
    if (GetAsyncKeyState(d_key) && GetAsyncKeyState(crouch_key) && Includes::m249_active == true)
    {
        return 1.75;
    }
    // STANDING D 
    if (GetAsyncKeyState(d_key) && Includes::m249_active == true)
    {
        return 4.5;
    }
    // CROUCHING NO KEY 
    if (GetAsyncKeyState(crouch_key) && Includes::m249_active == true)
    {
        return 1;
    }
    // STANDING NO KEY 
    else return 2;
}
float Script::semi_pen() {
    // STANDING S + MOVING 
    if (GetAsyncKeyState(s_key) && GetAsyncKeyState(crouch_key) && Includes::semi_active == true)
    {
        return 1.2f;
    }
    // STANDING S 
    if (GetAsyncKeyState(s_key) && Includes::semi_active == true)
    {
        return 3;
    }
    // STANDING A + MOVING 
    if (GetAsyncKeyState(a_key) && GetAsyncKeyState(crouch_key) && Includes::semi_active == true)
    {
        return 1.2f;
    }

    // STANDING A 
    if (GetAsyncKeyState(a_key) && Includes::semi_active == true)
    {
        return 3;
    }
    // STANDING D + MOVING 
    if (GetAsyncKeyState(d_key) && GetAsyncKeyState(crouch_key) && Includes::semi_active == true)
    {
        return 1.2f;
    }
    // STANDING D 
    if (GetAsyncKeyState(d_key) && Includes::semi_active == true)
    {
        return 3;
    }
    // CROUCHING NO KEY 
    if (GetAsyncKeyState(crouch_key) && Includes::semi_active == true)
    {
        return 1;
    }
    // STANDING NO KEY 
    else return 2;
}

void Script::mouse_move(int x, int y)
{
    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.mouseData = 0;
    input.mi.time = 0;
    input.mi.dx = x;
    input.mi.dy = y;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &input, sizeof(input));
}

void Script::smoothing(double delay, double control_time, int x, int y)
{
    int x_ = 0, y_ = 0, t_ = 0;

    for (int i = 1; i <= (int)control_time; ++i) {
        int xI = i * x / (int)control_time;
        int yI = i * y / (int)control_time;
        int tI = i * (int)control_time / (int)control_time;

        int delta_x = xI - x_;
        int delta_y = yI - y_;

        mouse_move(delta_x, delta_y);
        pQuerySleep((int)tI - (int)t_);
        x_ = xI; y_ = yI; t_ = tI;
    }
    pQuerySleep((int)delay - (int)control_time);
}

int Script::isMuzzleBoost(float delay) {
    if (Includes::boost_active == true) {
        float difference = delay * 0.1f;
        delay = delay - difference;
        return (int)delay + 1;
    }
    else
        return (int)delay;
}

int Script::isMuzzleBreak(float delay) {
    if (Includes::break_active == true) {
        float difference = delay * 0.1f;
        delay = delay - difference;
        return (int)delay + 0.5;
    }
    else
        return (int)delay;
}

bool Script::recoilExecution() {
    for (;;) {
        if (CurrentWeapon::WeaponActive() == true)
        {
            if (Includes::recoil == true && Includes::hipfire == true)
            {
                for (int i = 0; i <= CurrentWeapon::Bullet() && GetAsyncKeyState(VK_LBUTTON) & 0x8000; i++)
                {
                    smoothing((int)isMuzzleBoost((float)CurrentWeapon::Delay()), (int)isMuzzleBoost(CurrentWeapon::Control(i)), (int)CurrentWeapon::x(i), (int)CurrentWeapon::y(i));
                    CurrentWeapon::RecoilBreak(i);
                }
            }
            else if (Includes::recoil == true && Includes::hipfire == false)
            {
                for (int i = 0; i <= CurrentWeapon::Bullet() && GetAsyncKeyState(VK_RBUTTON) & 0x8000 && GetAsyncKeyState(VK_LBUTTON) & 0x8000; i++)
                {
                    smoothing((int)isMuzzleBoost((float)CurrentWeapon::Delay()), (int)isMuzzleBoost(CurrentWeapon::Control(i)), (int)CurrentWeapon::x(i), (int)CurrentWeapon::y(i));
                    CurrentWeapon::RecoilBreak(i);
                }
            }
        }
        Sleep(1);
    }
    return false;
}

void Script::keybinds() {
    if (current_window_title() == "Rust")
    {
        if (Includes::antiafk)
        {
            if (current_window_title() == "Rust")
            {
                Sleep(200);
                keybd_event(0x57, 0xb8, 0, 0); //w
                Sleep(50);
                keybd_event(0x57, 0xb8, KEYEVENTF_KEYUP, 0); // w
                Sleep(200);
                keybd_event(0x41, 0xb8, 0, 0); //a
                Sleep(50);
                keybd_event(0x41, 0xb8, KEYEVENTF_KEYUP, 0); // a
                Sleep(200);
                keybd_event(0x53, 0xb8, 0, 0); //s
                Sleep(50);
                keybd_event(0x53, 0xb8, KEYEVENTF_KEYUP, 0); // s
                Sleep(200);
                keybd_event(0x44, 0xb8, 0, 0); //d
                Sleep(50);
                keybd_event(0x44, 0xb8, KEYEVENTF_KEYUP, 0); //d
                Sleep(200);
            }
        }
    }

    if (Includes::Cross_color == "Purple")
    {
        Includes::clrprp = true;
    }
    if (Includes::Cross_color == "Green")
    {
        Includes::clrgrn = true;
    }
    if (Includes::Cross_color == "Blue")
    {
        Includes::clrblu = true;
    }
    if (Includes::Cross_color == "Cyan")
    {
        Includes::clrcyn = true;
    }
    if (Includes::Cross_color == "Red")
    {
        Includes::clrred = true;
    }
    if (GetAsyncKeyState(Includes::hpbind) & 1)
    {
        Includes::hipfire = !Includes::hipfire;
    }
    if (GetAsyncKeyState(Includes::hsbind) & 1)
    {
        Includes::showing = !Includes::showing;
    }
    if (Includes::recoil == true) {
        Includes::Enabled = "ON";
    }
    if (Includes::show_info == true)
    {
        DWORD flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize;
        ImGui::SetNextWindowSize(ImVec2(135, 100));
        ImGui::Begin("rainycheats - Info", &Includes::show_info, flags);

        if (Includes::Enabled == "ON")
        {
            ImGui::Text("Enabled:");
            ImGui::SameLine(65);
            ImGui::TextColored(ImColor(9, 255, 0), Includes::Enabled);
        }
        else {
            ImGui::Text("Enabled:");
            ImGui::SameLine(65);
            ImGui::TextColored(ImColor(255, 0, 8), Includes::Enabled);
        }
        if (Includes::Active_Weapon == "NONE")
        {
            ImGui::Text("Weapon:");
            ImGui::SameLine(58);
            ImGui::TextColored(ImColor(255, 0, 8), Includes::Active_Weapon);
        }
        else {
            ImGui::Text("Weapon:");
            ImGui::SameLine(58);
            ImGui::TextColored(ImColor(9, 255, 0), Includes::Active_Weapon);
        }
        if (Includes::Active_Scopes == "NONE")
        {
            ImGui::Text("Scopes:");
            ImGui::SameLine(58);
            ImGui::TextColored(ImColor(255, 0, 8), Includes::Active_Scopes);
        }
        else {
            ImGui::Text("Scopes:");
            ImGui::SameLine(58);
            ImGui::TextColored(ImColor(9, 255, 0), Includes::Active_Scopes);
        }
        if (Includes::Active_Barrel == "NONE")
        {
            ImGui::Text("Barrel:");
            ImGui::SameLine(58);
            ImGui::TextColored(ImColor(255, 0, 8), Includes::Active_Barrel);
        }
        else {
            ImGui::Text("Barrel:");
            ImGui::SameLine(58);
            ImGui::TextColored(ImColor(9, 255, 0), Includes::Active_Barrel);
        }
        ImGui::End();
    }
    if (current_window_title() == "Rust")
    {
        if (GetAsyncKeyState(Includes::akbind) & 1) {

            Includes::ak_active = !Includes::ak_active;
            if (Includes::ak_active == true)
            {
                Includes::lr_active = false, Includes::mp5_active = false, Includes::thompson_active = false, Includes::custom_active = false, Includes::m249_active = false, Includes::semi_active = false;
                Includes::selectedItemWP = 1;
                Includes::Active_Weapon = "AK47";
                Sleep(200);
            }
            else if (Includes::ak_active == false) {
                Includes::ak_active = false, Includes::lr_active = false, Includes::mp5_active = false, Includes::thompson_active = false, Includes::custom_active = false, Includes::m249_active = false, Includes::semi_active = false;
                Includes::holo_active = false, Includes::sniper_active = false, Includes::simple_active = false; Includes::x16_active = false;
                Includes::silencer_active = false;


                Includes::selectedItemWP = 0;
                Sleep(200);
            }
        }
        if (GetAsyncKeyState(Includes::lrbind) & 1) {

            Includes::lr_active = !Includes::lr_active;
            if (Includes::lr_active == true)
            {
                Includes::ak_active = false, Includes::mp5_active = false, Includes::thompson_active = false, Includes::custom_active = false, Includes::m249_active = false, Includes::semi_active = false;
                Includes::selectedItemWP = 2;
                Includes::Active_Weapon = "LR300";
                Sleep(200);
            }
            else if (Includes::lr_active == false) {
                Includes::ak_active = false, Includes::lr_active = false, Includes::mp5_active = false, Includes::thompson_active = false, Includes::custom_active = false, Includes::m249_active = false, Includes::semi_active = false;
                Includes::holo_active = false, Includes::sniper_active = false, Includes::simple_active = false; Includes::x16_active = false;
                Includes::silencer_active = false;



                Includes::selectedItemWP = 0;
                Includes::Active_Weapon = "NONE";
                Sleep(200);
            }
        }
        if (GetAsyncKeyState(Includes::mpbind) & 1) {

            Includes::mp5_active = !Includes::mp5_active;
            if (Includes::mp5_active == true)
            {
                Includes::ak_active = false, Includes::lr_active = false, Includes::thompson_active = false, Includes::custom_active = false, Includes::m249_active = false, Includes::semi_active = false;
                Includes::Active_Weapon = "MP5";
                Includes::selectedItemWP = 3;
                Sleep(200);

            }
            else if (Includes::mp5_active == false) {
                Includes::ak_active = false, Includes::lr_active = false, Includes::mp5_active = false, Includes::thompson_active = false, Includes::custom_active = false, Includes::m249_active = false, Includes::semi_active = false;
                Includes::holo_active = false, Includes::sniper_active = false, Includes::simple_active = false; Includes::x16_active = false;
                Includes::silencer_active = false;



                Includes::selectedItemWP = 0;
                Includes::Active_Weapon = "NONE";

                Sleep(200);
            }
        }
        if (GetAsyncKeyState(Includes::csbind) & 1) {

            Includes::custom_active = !Includes::custom_active;
            if (Includes::custom_active == true)
            {
                Includes::ak_active = false, Includes::lr_active = false, Includes::mp5_active = false, Includes::thompson_active = false, Includes::m249_active = false, Includes::semi_active = false;
                Includes::selectedItemWP = 4;
                Includes::Active_Weapon = "SMG";
                Sleep(200);
            }
            else if (Includes::custom_active == false) {
                Includes::ak_active = false, Includes::lr_active = false, Includes::mp5_active = false, Includes::thompson_active = false, Includes::custom_active = false, Includes::m249_active = false, Includes::semi_active = false;
                Includes::holo_active = false, Includes::sniper_active = false, Includes::simple_active = false; Includes::x16_active = false;
                Includes::silencer_active = false;



                Includes::selectedItemWP = 0;
                Includes::Active_Weapon = "NONE";
                Sleep(200);
            }
        }
        if (GetAsyncKeyState(Includes::tmbind) & 1) {

            Includes::thompson_active = !Includes::thompson_active;
            if (Includes::thompson_active == true)
            {
                Includes::ak_active = false, Includes::lr_active = false, Includes::mp5_active = false, Includes::custom_active = false, Includes::m249_active = false, Includes::semi_active = false;
                Includes::selectedItemWP = 5;
                Includes::Active_Weapon = "TOMMY";
                Sleep(200);
            }
            else if (Includes::thompson_active == false) {
                Includes::
                    Includes::ak_active = false, Includes::lr_active = false, Includes::mp5_active = false, Includes::thompson_active = false, Includes::custom_active = false, Includes::m249_active = false, Includes::semi_active = false;
                Includes::holo_active = false, Includes::sniper_active = false, Includes::simple_active = false; Includes::x16_active = false;
                Includes::silencer_active = false;



                Includes::selectedItemWP = 0;
                Includes::Active_Weapon = "NONE";
                Sleep(200);
            }
        }
        if (GetAsyncKeyState(Includes::smbind) & 1) {

            Includes::semi_active = !Includes::semi_active;
            if (Includes::semi_active == true)
            {
                Includes::ak_active = false, Includes::lr_active = false, Includes::mp5_active = false, Includes::thompson_active = false, Includes::m249_active = false, Includes::custom_active = false;
                Includes::selectedItemWP = 6;
                Includes::Active_Weapon = "SEMI";
                Sleep(200);
            }
            else if (Includes::semi_active == false) {
                Includes::ak_active = false, Includes::lr_active = false, Includes::mp5_active = false, Includes::thompson_active = false, Includes::custom_active = false, Includes::m249_active = false, Includes::semi_active = false;
                Includes::holo_active = false, Includes::sniper_active = false, Includes::simple_active = false; Includes::x16_active = false;
                Includes::silencer_active = false;



                Includes::selectedItemWP = 0;
                Includes::Active_Weapon = "NONE";
                Sleep(200);
            }
        }
        if (GetAsyncKeyState(Includes::m2bind) & 1) {

            Includes::m249_active = !Includes::m249_active;
            if (Includes::m249_active == true)
            {
                Includes::ak_active = false, Includes::lr_active = false, Includes::mp5_active = false, Includes::thompson_active = false, Includes::semi_active = false, Includes::custom_active = false;
                Includes::selectedItemWP = 7;
                Includes::Active_Weapon = "M249";
                Sleep(200);
            }
            else if (Includes::m249_active == false) {
                Includes::ak_active = false, Includes::lr_active = false, Includes::mp5_active = false, Includes::thompson_active = false, Includes::custom_active = false, Includes::m249_active = false, Includes::semi_active = false;
                Includes::holo_active = false, Includes::sniper_active = false, Includes::simple_active = false; Includes::x16_active = false;
                Includes::silencer_active = false;



                Includes::selectedItemWP = 0;
                Includes::Active_Weapon = "NONE";

                Sleep(200);
            }
        }
        //if (GetAsyncKeyState(Includes::pybind) & 1) {
        //     Includes::py_active = !Includes::py_active;
        //     if (Includes::py_active == true)
        //     {
        //         Includes::selectedItemWP = 8;
        //         Includes::Active_Barrel = "PYTHON";
        //         Sleep(200);
        //     }
        //     else if (Includes::py_active == false) {
        //         Includes::selectedItemWP = 0;
        //         Includes::Active_Barrel = "NONE";
        //         Sleep(200);
        //     }
        // }
        // if (GetAsyncKeyState(Includes::m9bind) & 1) {
        //     Includes::m92_active = !Includes::m92_active;
        //     if (Includes::m92_active == true)
        //     {
        //         Includes::selectedItemWP = 9;
        //         Includes::Active_Barrel = "M92";
        //         Sleep(200);
        //     }
        //     else if (Includes::m92_active == false) {
        //         Includes::selectedItemWP = 0;
        //         Includes::Active_Barrel = "NONE";
        //         Sleep(200);
        //     }
        // }
        // if (GetAsyncKeyState(Includes::m9bind) & 1) {
        //     Includes::m92_active = !Includes::m92_active;
        //     if (Includes::m92_active == true)
        //     {
        //         Includes::selectedItemWP = 10;
        //         Includes::Active_Barrel = "M92";
        //         Sleep(200);
        //     }
        //     else if (Includes::m92_active == false) {
        //         Includes::selectedItemWP = 0;
        //         Includes::Active_Barrel = "NONE";
        //         Sleep(200);
        //     }
        // }
        // if (GetAsyncKeyState(Includes::m3bind) & 1) {
        //     Includes::m39_active = !Includes::m39_active;
        //     if (Includes::m39_active == true)
        //     {
        //         Includes::selectedItemWP = 11;
        //         Includes::Active_Barrel = "M92";
        //         Sleep(200);
        //     }
        //     else if (Includes::m39_active == false) {
        //         Includes::selectedItemWP = 0;
        //         Includes::Active_Barrel = "NONE";
        //         Sleep(200);
        //     }
        // }
        // if (GetAsyncKeyState(Includes::revbind) & 1) {
        //     Includes::revolver_active = !Includes::revolver_active;
        //     if (Includes::revolver_active == true)
        //     {
        //         Includes::selectedItemWP = 12;
        //         Includes::Active_Barrel = "REVOLVER";
        //         Sleep(200);
        //     }
        //     else if (Includes::revolver_active == false) {
        //         Includes::selectedItemWP = 0;
        //         Includes::Active_Barrel = "NONE";
        //         Sleep(200);
        //     }
         //}
        ///////Scopes
        if (GetAsyncKeyState(Includes::hhbind) & 1) {
            Includes::holo_active = !Includes::holo_active;
            if (Includes::holo_active == true)
            {
                Includes::sniper_active = false, Includes::simple_active = false; Includes::x16_active = false;

                Includes::Active_Scopes = "HOLO";
                Includes::selectedItemSC = 3;
                Sleep(200);
            }
            else if (Includes::holo_active == false) {

                Includes::Active_Scopes = "NONE";
                Includes::selectedItemSC = 0;
                Sleep(200);
            }
        }
        if (GetAsyncKeyState(Includes::xxbind) & 1) {
            Includes::sniper_active = !Includes::sniper_active;
            if (Includes::sniper_active == true)
            {
                Includes::holo_active = false, Includes::simple_active = false; Includes::x16_active = false;
                Includes::selectedItemSC = 1;
                Includes::Active_Scopes = "8X";
                Sleep(200);
            }
            else if (Includes::sniper_active == false) {
                Includes::selectedItemSC = 0;

                Includes::Active_Scopes = "NONE";
                Sleep(200);
            }
        }
        if (GetAsyncKeyState(Includes::ssbind) & 1) {
            Includes::simple_active = !Includes::simple_active;
            if (Includes::simple_active == true)
            {
                Includes::holo_active = false, Includes::sniper_active = false; Includes::x16_active = false;
                Includes::selectedItemSC = 4;
                Includes::Active_Scopes = "SIMPLE";
                Sleep(200);
            }
            else if (Includes::simple_active == false) {

                Includes::Active_Scopes = "NONE";
                Includes::selectedItemSC = 0;
                Sleep(200);
            }
        }
        if (GetAsyncKeyState(Includes::yybind) & 1) {
            Includes::x16_active = !Includes::x16_active;
            if (Includes::x16_active == true)
            {
                Includes::holo_active = false, Includes::sniper_active = false; Includes::simple_active = false;
                Includes::selectedItemSC = 2;
                Includes::Active_Scopes = "16X";
                Sleep(200);
            }
            else if (Includes::x16_active == false) {

                Includes::Active_Scopes = "NONE";

                Includes::selectedItemSC = 0;
                Sleep(200);
            }
        }
        if (GetAsyncKeyState(Includes::slbind) & 1) {
            Includes::silencer_active = !Includes::silencer_active;
            if (Includes::silencer_active == true)
            {
                Includes::selectedItemBR = 1;
                Includes::Active_Barrel = "SILENCER";
                Sleep(200);
            }
            else if (Includes::silencer_active == false) {
                Includes::selectedItemBR = 0;
                Includes::Active_Barrel = "NONE";
                Sleep(200);
            }


        }
        if (GetAsyncKeyState(Includes::bobind) & 1) {
            Includes::boost_active = !Includes::boost_active;
            if (Includes::boost_active == true)
            {
                Includes::selectedItemBR = 2;
                Includes::Active_Barrel = "BOOST";
                Sleep(200);
            }
            else if (Includes::boost_active == false) {
                Includes::selectedItemBR = 0;
                Includes::Active_Barrel = "NONE";
                Sleep(200);
            }
        }
        if (GetAsyncKeyState(Includes::brbind) & 1) {
            Includes::break_active = !Includes::break_active;
            if (Includes::break_active == true)
            {
                Includes::selectedItemBR = 3;
                Includes::Active_Barrel = "BREAK";
                Sleep(200);
            }
            else if (Includes::break_active == false) {
                Includes::selectedItemBR = 0;
                Includes::Active_Barrel = "NONE";
                Sleep(200);
            }
        }
    }
    if (current_window_title() == "Rust")
    {
        if (Includes::bRapidf == true)
        {
            if (GetAsyncKeyState(VK_LBUTTON))
            {
                POINT p;
                GetCursorPos(&p);

                mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, NULL);
                mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
            }
        }
    }
    if (GetAsyncKeyState(Includes::resbind) != 0) {

        Includes::ak_active, Includes::lr_active, Includes::mp5_active, Includes::custom_active, Includes::thompson_active, Includes::m249_active = false;
        Includes::sniper_active, Includes::x16_active, Includes::holo_active, Includes::simple_active, Includes::silencer_active = false;

        Includes::selectedItemWP = 0;
        Includes::selectedItemSC = 0;
        Includes::selectedItemBR = 0;

        Includes::Active_Weapon = "NONE";
        Includes::Active_Scopes = "NONE";
        Includes::Active_Barrel = "NONE";

        Sleep(200);
    }

    return;
}

double CurrentWeapon::BarrelAttachment() {
    if (Includes::silencer_active == true) {
        if (Includes::m249_active == true)
            Barrel.Suppressor = 0.75;
        if (Includes::custom_active == true)
            Barrel.Suppressor = 0.85;
        if (Includes::simple_active == true || Includes::holo_active == true)
            return 0.8;
        if (Includes::sniper_active == true && Includes::silencer_active == true)
            return 1.0;
        else
            return Barrel.Suppressor;
    }
    else if (Includes::boost_active == true) {
        return Barrel.None;
    }
    else
        return Barrel.None;
}
double CurrentWeapon::ScopeAttachment() {
    if (GetAsyncKeyState(VK_RBUTTON) && GetAsyncKeyState(VK_LBUTTON) && Includes::sniper_active == true && Includes::recoil == true) {
        if (Includes::custom_active == true || Includes::thompson_active == true) {
            if (Includes::sniper_active == true)
                return Scope.x8 + 0.75;
        }
        return Scope.x8;
    }
    else if (GetAsyncKeyState(VK_RBUTTON) && GetAsyncKeyState(VK_LBUTTON) && Includes::x16_active == true && Includes::recoil == true) {
        if (Includes::custom_active == true || Includes::thompson_active == true) {
            if (Includes::x16_active == true)
                return Scope.x16 + 1.5;
        }
        return Scope.x16;
    }
    else if (GetAsyncKeyState(VK_RBUTTON) && GetAsyncKeyState(VK_LBUTTON) && Includes::simple_active == true && Includes::recoil == true) {
        if (Includes::custom_active == true || Includes::thompson_active == true) {
            if (Includes::simple_active == true)
                return Scope.Simple + 0.1;
        }
        return Scope.Simple;
    }
    else if (GetAsyncKeyState(VK_RBUTTON) && GetAsyncKeyState(VK_LBUTTON) && Includes::holo_active == true && Includes::recoil == true) {
        if (Includes::custom_active == true || Includes::thompson_active == true) {
            if (Includes::holo_active == true)
                return Scope.Holo + 0.3;
        }
        return Scope.Holo;
    }
    else if (Includes::recoil == false) {
        return Scope.None;
    }

    else if (GetAsyncKeyState(VK_RBUTTON) && GetAsyncKeyState(VK_LBUTTON) && Includes::recoil == true)
    {
        return 1;
    }

    else if (GetAsyncKeyState(VK_LBUTTON) && Includes::recoil == true && Includes::hipfire == true)
    {
        return 0.21;
    }
}

double CurrentWeapon::x(int b) {
    if (Includes::randomxy_active == false) {
        if (Includes::ak_active == true) return (RecoilTables::Recoil_Ak47[b].x * ScopeAttachment() * BarrelAttachment() / (-0.0295 * (Includes::game_sensitivity) * 3.0 * (90 / 100.0))) / Includes::XCONTROL;
        if (Includes::lr_active == true) return (RecoilTables::Recoil_Lr300[b].x * ScopeAttachment() * BarrelAttachment() / (-0.0295 * (Includes::game_sensitivity) * 3.0 * (90 / 100.0))) / Includes::XCONTROL;
        if (Includes::mp5_active == true) return (RecoilTables::Recoil_Mp5a4[b].x * ScopeAttachment() * BarrelAttachment() / 4) / Includes::game_sensitivity / Includes::XCONTROL;
        if (Includes::thompson_active == true) return (RecoilTables::Recoil_Thompson_g[b].x * ScopeAttachment() * BarrelAttachment() / 4) / Includes::game_sensitivity / Includes::XCONTROL;
        if (Includes::custom_active == true) return (RecoilTables::Recoil_Custom_g[b].x * ScopeAttachment() * BarrelAttachment() / 4) / Includes::game_sensitivity / Includes::XCONTROL;
        if (Includes::m249_active == true)
            return (RecoilTables::Recoil_M249[b].x * ScopeAttachment() * BarrelAttachment() * Script::m249_pen() / 4) / Includes::game_sensitivity / Includes::XCONTROL;
        if (Includes::semi_active == true)
            return (RecoilTables::Recoil_Semi_r[b].x * ScopeAttachment() * BarrelAttachment() * Script::semi_pen() / 4) / Includes::game_sensitivity / Includes::XCONTROL;
        //if (Includes::py_active == true) return (RecoilTables::Recoil_Python[b].x * ScopeAttachment() * BarrelAttachment() / 4) / Includes::game_sensitivity / Includes::XCONTROL;
        //if (Includes::m92_active == true) return (RecoilTables::Recoil_M92[b].x * ScopeAttachment() * BarrelAttachment() / 4) / Includes::game_sensitivity / Includes::XCONTROL;
        //if (Includes::m39_active == true) return (RecoilTables::Recoil_M39[b].x * ScopeAttachment() * BarrelAttachment() / 4) / Includes::game_sensitivity / Includes::XCONTROL;
        //if (Includes::revolver_active == true) return (RecoilTables::Recoil_Rev[b].x * ScopeAttachment() * BarrelAttachment() / 4) / Includes::game_sensitivity / Includes::XCONTROL;
        else return 0;
    }
    if (Includes::randomxy_active == true) {                                                                 //X RANDOMIZATION ON AND OFF
        if (Includes::ak_active == true) return ((RecoilTables::Recoil_Ak47[b].x * ScopeAttachment()) * BarrelAttachment()) / (-0.0295 * (Includes::game_sensitivity) * 3.0 * (90 / 100.0)) / Includes::XCONTROL + rand() % (Includes::XSCALE + Includes::XSCALE) + (-Includes::XSCALE);
        if (Includes::lr_active == true) return ((RecoilTables::Recoil_Lr300[b].x * ScopeAttachment()) * BarrelAttachment()) / (-0.0295 * (Includes::game_sensitivity) * 3.0 * (90 / 100.0)) / Includes::XCONTROL + rand() % (Includes::XSCALE + Includes::XSCALE) + (-Includes::XSCALE);
        if (Includes::mp5_active == true) return ((RecoilTables::Recoil_Mp5a4[b].x * ScopeAttachment()) * BarrelAttachment()) / 4 / Includes::game_sensitivity / Includes::XCONTROL + rand() % (Includes::XSCALE + Includes::XSCALE) + (-Includes::XSCALE);
        if (Includes::thompson_active == true) return ((RecoilTables::Recoil_Thompson_g[b].x * ScopeAttachment()) * BarrelAttachment()) / 4 / Includes::game_sensitivity / Includes::XCONTROL + rand() % (Includes::XSCALE + Includes::XSCALE) + (-Includes::XSCALE);
        if (Includes::custom_active == true) return ((RecoilTables::Recoil_Custom_g[b].x * ScopeAttachment()) * BarrelAttachment()) / 4 / Includes::game_sensitivity / Includes::XCONTROL + rand() % (Includes::XSCALE + Includes::XSCALE) + (-Includes::XSCALE);
        if (Includes::m249_active == true)
            return ((RecoilTables::Recoil_M249[b].x * ScopeAttachment()) * BarrelAttachment()) * Script::m249_pen() / 4 / Includes::game_sensitivity / Includes::XCONTROL + rand() % (Includes::XSCALE + Includes::XSCALE) + (-Includes::XSCALE);
        if (Includes::semi_active == true)
            return ((RecoilTables::Recoil_Semi_r[b].x * ScopeAttachment()) * BarrelAttachment()) * Script::semi_pen() / 4 / Includes::game_sensitivity / Includes::XCONTROL + rand() % (Includes::XSCALE + Includes::XSCALE) + (-Includes::XSCALE);
        //if (Includes::py_active == true) return ((RecoilTables::Recoil_Python[b].x * ScopeAttachment()) * BarrelAttachment()) / 4 / Includes::game_sensitivity / Includes::XCONTROL + rand() % (Includes::XSCALE + Includes::XSCALE) + (-Includes::XSCALE);
        //if (Includes::m92_active == true) return ((RecoilTables::Recoil_M92[b].x * ScopeAttachment()) * BarrelAttachment()) / 4 / Includes::game_sensitivity / Includes::XCONTROL + rand() % (Includes::XSCALE + Includes::XSCALE) + (-Includes::XSCALE);
        //if (Includes::m39_active == true) return ((RecoilTables::Recoil_M39[b].x * ScopeAttachment()) * BarrelAttachment()) / 4 / Includes::game_sensitivity / Includes::XCONTROL + rand() % (Includes::XSCALE + Includes::XSCALE) + (-Includes::XSCALE);
        //if (Includes::revolver_active == true) return ((RecoilTables::Recoil_Rev[b].x * ScopeAttachment()) * BarrelAttachment()) / 4 / Includes::game_sensitivity / Includes::XCONTROL + rand() % (Includes::XSCALE + Includes::XSCALE) + (-Includes::XSCALE);
        else return 0;
    }
}

double CurrentWeapon::y(int b) {                                                                                       //Y RANDOMIZATION ON AND OFF
    if (Includes::randomxy_active == false) {
        if (Includes::ak_active == true) return (RecoilTables::Recoil_Ak47[b].y * ScopeAttachment() * BarrelAttachment() / (-0.0295 * (Includes::game_sensitivity) * 3.0 * (90 / 100.0))) / Includes::YCONTROL;
        if (Includes::lr_active == true) return (RecoilTables::Recoil_Lr300[b].y * ScopeAttachment() * BarrelAttachment() / (-0.0295 * (Includes::game_sensitivity) * 3.0 * (90 / 100.0))) / Includes::YCONTROL;
        if (Includes::mp5_active == true) return (RecoilTables::Recoil_Mp5a4[b].y * ScopeAttachment() * BarrelAttachment() / 4) / Includes::game_sensitivity / Includes::YCONTROL;
        if (Includes::thompson_active == true) return (RecoilTables::Recoil_Thompson_g[b].y * ScopeAttachment() * BarrelAttachment() / 4) / Includes::game_sensitivity / Includes::YCONTROL;
        if (Includes::custom_active == true) return (RecoilTables::Recoil_Custom_g[b].y * ScopeAttachment() * BarrelAttachment() / 4) / Includes::game_sensitivity / Includes::YCONTROL;
        if (Includes::m249_active == true)
            return (RecoilTables::Recoil_M249[b].y * ScopeAttachment() * BarrelAttachment() * Script::m249_pen() / 4) / Includes::game_sensitivity / Includes::YCONTROL;
        if (Includes::semi_active == true)
            return (RecoilTables::Recoil_Semi_r[b].y * ScopeAttachment() * BarrelAttachment() * Script::semi_pen() / 4) / Includes::game_sensitivity / Includes::YCONTROL;
        //if (Includes::py_active == true) return (RecoilTables::Recoil_Python[b].y * ScopeAttachment() * BarrelAttachment() / 4) / Includes::game_sensitivity / Includes::YCONTROL;
        //if (Includes::m92_active == true) return (RecoilTables::Recoil_M92[b].y * ScopeAttachment() * BarrelAttachment() / 4) / Includes::game_sensitivity / Includes::YCONTROL;
        //if (Includes::m39_active == true) return (RecoilTables::Recoil_M39[b].y * ScopeAttachment() * BarrelAttachment() / 4) / Includes::game_sensitivity / Includes::YCONTROL;
        //if (Includes::revolver_active == true) return (RecoilTables::Recoil_Rev[b].y * ScopeAttachment() * BarrelAttachment() / 4) / Includes::game_sensitivity / Includes::YCONTROL;
        else return 0;
    }
    if (Includes::randomxy_active == true) {
        if (Includes::ak_active == true) return ((RecoilTables::Recoil_Ak47[b].y * ScopeAttachment()) * BarrelAttachment()) / (-0.0295 * (Includes::game_sensitivity) * 3.0 * (90 / 100.0)) / Includes::YCONTROL + rand() % (Includes::YSCALE + Includes::YSCALE) + (-Includes::YSCALE);
        if (Includes::lr_active == true) return ((RecoilTables::Recoil_Lr300[b].y * ScopeAttachment()) * BarrelAttachment()) / (-0.0295 * (Includes::game_sensitivity) * 3.0 * (90 / 100.0)) / Includes::YCONTROL + rand() % (Includes::YSCALE + Includes::YSCALE) + (-Includes::YSCALE);
        if (Includes::custom_active == true) return ((RecoilTables::Recoil_Mp5a4[b].y * ScopeAttachment()) * BarrelAttachment()) / 4 / Includes::game_sensitivity / Includes::YCONTROL + rand() % (Includes::YSCALE + Includes::YSCALE) + (-Includes::YSCALE);
        if (Includes::thompson_active == true) return ((RecoilTables::Recoil_Thompson_g[b].y * ScopeAttachment()) * BarrelAttachment()) / 4 / Includes::game_sensitivity / Includes::YCONTROL + rand() % (Includes::YSCALE + Includes::YSCALE) + (-Includes::YSCALE);
        if (Includes::custom_active == true) return ((RecoilTables::Recoil_Custom_g[b].y * ScopeAttachment()) * BarrelAttachment()) / 4 / Includes::game_sensitivity / Includes::YCONTROL + rand() % (Includes::YSCALE + Includes::YSCALE) + (-Includes::YSCALE);
        if (Includes::m249_active == true)
            return ((RecoilTables::Recoil_M249[b].x * ScopeAttachment()) * BarrelAttachment()) * Script::m249_pen() / 4 / Includes::game_sensitivity / Includes::YCONTROL + rand() % (Includes::YSCALE + Includes::YSCALE) + (-Includes::YSCALE);
        if (Includes::semi_active == true)
            return ((RecoilTables::Recoil_Semi_r[b].x * ScopeAttachment()) * BarrelAttachment()) * Script::semi_pen() / 4 / Includes::game_sensitivity / Includes::YCONTROL + rand() % (Includes::YSCALE + Includes::YSCALE) + (-Includes::YSCALE);
        //if (Includes::py_active == true) return ((RecoilTables::Recoil_Python[b].y * ScopeAttachment()) * BarrelAttachment()) / 4 / Includes::game_sensitivity / Includes::YCONTROL + rand() % (Includes::YSCALE + Includes::YSCALE) + (-Includes::YSCALE);
        //if (Includes::m92_active == true) return ((RecoilTables::Recoil_M92[b].y * ScopeAttachment()) * BarrelAttachment()) / 4 / Includes::game_sensitivity / Includes::YCONTROL + rand() % (Includes::YSCALE + Includes::YSCALE) + (-Includes::YSCALE);
        //if (Includes::m39_active == true) return ((RecoilTables::Recoil_M39[b].y * ScopeAttachment()) * BarrelAttachment()) / 4 / Includes::game_sensitivity / Includes::YCONTROL + rand() % (Includes::YSCALE + Includes::YSCALE) + (-Includes::YSCALE);
        //if (Includes::revolver_active == true) return ((RecoilTables::Recoil_Rev[b].y * ScopeAttachment()) * BarrelAttachment()) / 4 / Includes::game_sensitivity / Includes::YCONTROL + rand() % (Includes::YSCALE + Includes::YSCALE) + (-Includes::YSCALE);
        else return 0;
    }
}

double CurrentWeapon::Delay() {
    if (Includes::ak_active == true) return AK47.Delay;
    else if (Includes::lr_active == true) return LR300.Delay;
    else if (Includes::mp5_active == true) return MP5.Delay;
    else if (Includes::thompson_active == true) return THOMPSON.Delay;
    else if (Includes::custom_active == true) return CUSTOM_SMG.Delay;
    else if (Includes::m249_active == true) return M249.Delay;
    else if (Includes::semi_active == true) return SEMI.Delay;
    //else if (Includes::py_active == true) return Python.Delay;
    //else if (Includes::m92_active == true) return M92.Delay;
    //else if (Includes::m39_active == true) return M39.Delay;
    //else if (Includes::revolver_active == true) return Rev.Delay;
    return 0;
}

double CurrentWeapon::Control(int bullet) {
    if (Includes::legitmode == true) {
        if (Includes::ak_active == true) return AK47.Delay;
        else if (Includes::lr_active == true) return LR300.Delay;
        else if (Includes::mp5_active == true) return MP5.Delay;
        else if (Includes::thompson_active == true) return THOMPSON.Delay;
        else if (Includes::custom_active == true) return CUSTOM_SMG.Delay;
        else if (Includes::m249_active == true) return M249.Delay;
        else if (Includes::semi_active == true) return SEMI.Delay;
        //else if (Includes::py_active == true) return Python.Delay;
        //else if (Includes::m92_active == true) return M92.Delay;
        //else if (Includes::m39_active == true) return M39.Delay;
        //else if (Includes::revolver_active == true) return Rev.Delay;
    }
    else {
        if (Includes::ak_active == true) return RecoilTables::ControlTime_Ak47[bullet];
        else if (Includes::lr_active == true) return RecoilTables::control_lr[bullet];
        else if (Includes::mp5_active == true) return RecoilTables::control_mp5[bullet];
        else if (Includes::thompson_active == true) return RecoilTables::control_thompson[bullet];
        else if (Includes::custom_active == true) return RecoilTables::control_custom[bullet];
        else if (Includes::m249_active == true) return M249.Delay;
        else if (Includes::semi_active == true) return SEMI.Delay;
        //else if (Includes::py_active == true) return Python.Delay;
        //else if (Includes::m92_active == true) return M92.Delay;
        //else if (Includes::m39_active == true) return M39.Delay;
        //else if (Includes::revolver_active == true) return Rev.Delay;
    }
    return 0;
}

int CurrentWeapon::Bullet() {
    if (Includes::ak_active == true) return RecoilTables::Recoil_Ak47.size() - Includes::bulletscale - 1;
    else if (Includes::lr_active == true) return RecoilTables::Recoil_Lr300.size() - Includes::bulletscale - 1;
    else if (Includes::mp5_active == true) return RecoilTables::Recoil_Mp5a4.size() - Includes::bulletscale - 3;
    else if (Includes::thompson_active == true) return RecoilTables::Recoil_Thompson_g.size() - Includes::bulletscale - 1;
    else if (Includes::custom_active == true) return RecoilTables::Recoil_Custom_g.size() - Includes::bulletscale - 1;
    else if (Includes::m249_active == true)
        return RecoilTables::Recoil_M249.size() - 1;
    else if (Includes::semi_active == true)
        return RecoilTables::Recoil_Semi_r.size() - 1;
    //else if (Includes::py_active == true) return RecoilTables::Recoil_Python.size() - Includes::bulletscale;
    //else if (Includes::m92_active == true) return RecoilTables::Recoil_M92.size() - Includes::bulletscale;
    //else if (Includes::m39_active == true) return RecoilTables::Recoil_M39.size() - Includes::bulletscale;
    //else if (Includes::revolver_active == true) return RecoilTables::Recoil_Rev.size() - Includes::bulletscale;
}

bool CurrentWeapon::WeaponActive() {
    if (Includes::ak_active == true || Includes::lr_active == true || Includes::mp5_active == true || Includes::thompson_active == true || Includes::custom_active == true || Includes::semi_active == true || Includes::m249_active == true)
        return true;
    else
        return false;
}

void CurrentWeapon::RecoilBreak(int count) {
    if (Includes::semi_active == true) {
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    }
    else if (Includes::m249_active == true) {
    }
    else if (Includes::semi_active != true || Includes::m249_active != true) {
        if (count == CurrentWeapon::Bullet()) {
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }
    }
}
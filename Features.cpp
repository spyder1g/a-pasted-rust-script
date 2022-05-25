#define _CRT_SECURE_NO_WARNINGS
#include "Features.hpp"

typedef struct
{
    DWORD R;
    DWORD G;
    DWORD B;
    DWORD A;
}RGBA;

void DrawFilledRectImGui(int x, int y, int w, int h, RGBA color)
{
    ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0)), 0, 0);
}

DWORD Features::CrosshairThreadFunc(void)
{
    while (1)
    {
        if (GetAsyncKeyState(Includes::crossbind) & 1)
        {
            Includes::crosshair = !Includes::crosshair;
            Includes::ragedc = GetDC(HWND_DESKTOP);
            Includes::cx = GetSystemMetrics(SM_CXSCREEN) / 2 - ((Includes::crosshairsize - 1) / 2);
            Includes::cy = GetSystemMetrics(SM_CYSCREEN) / 2 - ((Includes::crosshairsize - 1) / 2);
        }
    }
}

int Features::crosshairthread() {
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CrosshairThreadFunc, 0, 0, 0);
    while (1)
    {
        if (GetAsyncKeyState(Includes::crossbind) & 1)
        {
            Includes::crosshair = !Includes::crosshair;
            Includes::ragedc = GetDC(HWND_DESKTOP);
            Includes::cx = GetSystemMetrics(SM_CXSCREEN) / 2 - ((Includes::crosshairsize - 1) / 2);
            Includes::cy = GetSystemMetrics(SM_CYSCREEN) / 2 - ((Includes::crosshairsize - 1) / 2);
        }
        if (Includes::crosshair == true)
        {
            if (Includes::clrprp == true)
            {
                Includes::clrgrn = false;
                Includes::clrblu = false;
                Includes::clrcyn = false;
                Includes::clrred = false;
                for (int i = 0; i < Includes::crosshairsize; i++)
                {
                    SetPixel(Includes::ragedc, Includes::cx + i, Includes::cy + ((Includes::crosshairsize - 1) / 2), RGB(201, 8, 255));
                    SetPixel(Includes::ragedc, Includes::cx + ((Includes::crosshairsize - 1) / 2), Includes::cy + i, RGB(201, 8, 255));
                }
            }
            if (Includes::clrgrn == true)
            {
                Includes::clrprp = false;
                Includes::clrblu = false;
                Includes::clrcyn = false;
                Includes::clrred = false;
                for (int i = 0; i < Includes::crosshairsize; i++)
                {
                    SetPixel(Includes::ragedc, Includes::cx + i, Includes::cy + ((Includes::crosshairsize - 1) / 2), RGB(13, 255, 0));
                    SetPixel(Includes::ragedc, Includes::cx + ((Includes::crosshairsize - 1) / 2), Includes::cy + i, RGB(13, 255, 0));
                }
            }
            if (Includes::clrblu == true)
            {
                Includes::clrprp = false;
                Includes::clrgrn = false;
                Includes::clrcyn = false;
                Includes::clrred = false;
                for (int i = 0; i < Includes::crosshairsize; i++)
                {
                    SetPixel(Includes::ragedc, Includes::cx + i, Includes::cy + ((Includes::crosshairsize - 1) / 2), RGB(25, 0, 255));
                    SetPixel(Includes::ragedc, Includes::cx + ((Includes::crosshairsize - 1) / 2), Includes::cy + i, RGB(25, 0, 255));
                }
            }
            if (Includes::clrcyn == true)
            {
                Includes::clrprp = false;
                Includes::clrgrn = false;
                Includes::clrblu = false;
                Includes::clrred = false;
                for (int i = 0; i < Includes::crosshairsize; i++)
                {
                    SetPixel(Includes::ragedc, Includes::cx + i, Includes::cy + ((Includes::crosshairsize - 1) / 2), RGB(0, 255, 225));
                    SetPixel(Includes::ragedc, Includes::cx + ((Includes::crosshairsize - 1) / 2), Includes::cy + i, RGB(0, 255, 225));
                }
            }
            if (Includes::clrred == true)
            {
                Includes::clrprp = false;
                Includes::clrgrn = false;
                Includes::clrblu = false;
                Includes::clrcyn = false;
                for (int i = 0; i < Includes::crosshairsize; i++)
                {
                    SetPixel(Includes::ragedc, Includes::cx + i, Includes::cy + ((Includes::crosshairsize - 1) / 2), RGB(255, 0, 34));
                    SetPixel(Includes::ragedc, Includes::cx + ((Includes::crosshairsize - 1) / 2), Includes::cy + i, RGB(255, 0, 34));
                }
            }
        }
        Sleep(1);
    }
    Sleep(1);
}

void Features::ComboBox() {
    //Weapons
    if (Includes::selectedItemWP == 0) {
        Includes::Active_Weapon = "NONE";
        Includes::ak_active = false; Includes::lr_active = false; Includes::mp5_active = false; Includes::custom_active = false; Includes::thompson_active = false; Includes::semi_active = false;
        Includes::m249_active = false;
    }
    if (Includes::selectedItemWP == 1) {
        Includes::Active_Weapon = "AK47";
        Includes::ak_active = true; Includes::lr_active = false; Includes::mp5_active = false; Includes::custom_active = false; Includes::thompson_active = false; Includes::semi_active = false;
        Includes::m249_active = false;
    }
    if (Includes::selectedItemWP == 2) {
        Includes::Active_Weapon = "LR300";
        Includes::lr_active = true; Includes::ak_active = false; Includes::mp5_active = false; Includes::custom_active = false; Includes::thompson_active = false; Includes::semi_active = false;
        Includes::m249_active = false;
    }
    if (Includes::selectedItemWP == 3) {
        Includes::Active_Weapon = "MP5A4";
        Includes::lr_active = false; Includes::ak_active = false; Includes::mp5_active = true; Includes::custom_active = false; Includes::thompson_active = false; Includes::semi_active = false;
        Includes::m249_active = false;
    }
    if (Includes::selectedItemWP == 4) {
        Includes::Active_Weapon = "CUSTOM";
        Includes::custom_active = true; Includes::ak_active = false; Includes::mp5_active = false; Includes::lr_active = false; Includes::thompson_active = false; Includes::semi_active = false;
        Includes::m249_active = false;
    }
    if (Includes::selectedItemWP == 5) {
        Includes::Active_Weapon = "THOMPSON";
        Includes::thompson_active = true; Includes::ak_active = false; Includes::mp5_active = false; Includes::custom_active = false; Includes::lr_active = false; Includes::semi_active = false;
        Includes::m249_active = false;
    }
    if (Includes::selectedItemWP == 6) {
        Includes::Active_Weapon = "SAR";
        Includes::semi_active = true; Includes::ak_active = false; Includes::mp5_active = false; Includes::custom_active = false; Includes::thompson_active = false; Includes::lr_active = false;
        Includes::m249_active = false;
    }
    if (Includes::selectedItemWP == 7) {
        Includes::Active_Weapon = "M249";
        Includes::m249_active = true; Includes::ak_active = false; Includes::mp5_active = false; Includes::custom_active = false; Includes::thompson_active = false; Includes::semi_active = false;
        Includes::lr_active = false;
    }
    //Scopes
    if (Includes::selectedItemSC == 0) {
        Includes::Active_Scopes = "NONE";
        Includes::sniper_active = false; Includes::x16_active = false; Includes::holo_active = false; Includes::simple_active = false;
    }
    if (Includes::selectedItemSC == 1) {
        Includes::Active_Scopes = "8X";
        Includes::sniper_active = true; Includes::x16_active = false; Includes::holo_active = false; Includes::simple_active = false;
    }
    if (Includes::selectedItemSC == 2) {
        Includes::Active_Scopes = "16X";
        Includes::sniper_active = false; Includes::x16_active = true; Includes::holo_active = false; Includes::simple_active = false;
    }
    if (Includes::selectedItemSC == 3) {
        Includes::Active_Scopes = "HOLO";
        Includes::sniper_active = false; Includes::x16_active = false; Includes::holo_active = true; Includes::simple_active = false;
    }
    if (Includes::selectedItemSC == 4) {
        Includes::Active_Scopes = "SIMPLE";
        Includes::sniper_active = false; Includes::x16_active = false; Includes::holo_active = false; Includes::simple_active = true;
    }
    //Barrels
    if (Includes::selectedItemBR == 0) {
        Includes::Active_Barrel = "NONE";
        Includes::silencer_active = false;
        Includes::break_active = false;
        Includes::boost_active = false;
    }
    if (Includes::selectedItemBR == 1) {
        Includes::Active_Barrel = "SILENCER";
        Includes::silencer_active = true;
        Includes::break_active = false;
    }
    if (Includes::selectedItemBR == 2) {
        Includes::Active_Barrel = "BOOST";
        Includes::silencer_active = false;
        Includes::boost_active = true;
        Includes::break_active = false;
    }
    //if (selectedItemBR == 2) {
    //    Active_Barrel = "BREAK";
    //    silencer_active = false;
    //    break_active = true;
    //    boost_active = false;
    //}
}

void Features::MenuItems() {
    if (Includes::selectedCross == 1)
    {
        Includes::clrprp = true;
    }
    if (Includes::selectedCross == 2)
    {
        Includes::clrgrn = true;
    }
    if (Includes::selectedCross == 3)
    {
        Includes::clrblu = true;
    }
    if (Includes::selectedCross == 4)
    {
        Includes::clrcyn = true;
    }
    if (Includes::selectedCross == 5)
    {
        Includes::clrred = true;
    }
}

std::string Features::random_string(int length)
{
    std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(str.begin(), str.end(), generator);
    return str.substr(0, length);    // assumes 32 < number of characters in str         
}
DWORDLONG Features::random_number(DWORDLONG min, DWORDLONG max)
{
    srand(time_t(0));
    DWORDLONG out = min + rand() % (max - min + 1);
    return out;
}
std::string Features::randomclassname = Features::random_string(Features::random_number(2, 6));

template<class T, class U>
static T clamp(const T& in, const U& low, const U& high)
{
    if (in <= low)
        return low;

    if (in >= high)
        return high;

    return in;
}

enum AnimationTypes { STATIC, DYNAMIC, INTERP };

std::string pChar(std::string first_, std::string second_) {
    return (first_ + second_);
}

float Animate(const char* label, const char* second_label, bool if_, float Maximal_, float Speed_, int type) {

    auto ID = ImGui::GetID(pChar(label, second_label).c_str());

    static std::map<ImGuiID, float> pValue;

    auto this_e = pValue.find(ID);

    if (this_e == pValue.end()) {
        pValue.insert({ ID, 0.f });
        this_e = pValue.find(ID);
    }

    switch (type) {

    case DYNAMIC: {
        if (if_) //do
            this_e->second += abs(Maximal_ - this_e->second) / Speed_;
        else
            this_e->second -= (0 + this_e->second) / Speed_;
    }
                break;

    case INTERP: {
        if (if_) //do
            this_e->second += (Maximal_ - this_e->second) / Speed_;
        else
            this_e->second -= (0 + this_e->second) / Speed_;
    }
               break;

    case STATIC: {
        if (if_) //do
            this_e->second += Speed_;
        else
            this_e->second -= Speed_;
    }
               break;
    }

    {
        if (this_e->second < 0.f)
            this_e->second = 0.f;
        else if (this_e->second > Maximal_)
            this_e->second = Maximal_;
    }

    return this_e->second;

}

const std::string Features::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}
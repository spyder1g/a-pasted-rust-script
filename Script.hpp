#include "RecoilTables.hpp"
#include "Includes.hpp"
//#include "Auth.hpp"
//#include "Base64.hpp"
//#include "Parser.hpp"

class Script {
public:
    static const char* itemWP[8];
    static const char* itemSC[5];
    static const char* itemBR[3];
    static const char* crossBR[6];
    static void pQuerySleep(int ms);
    static float m249_pen();
    static float semi_pen();
    static void mouse_move(int x, int y);
    static void smoothing(double delay, double control_time, int x, int y);
    static int isMuzzleBoost(float delay);
    static int isMuzzleBreak(float delay);
    static bool recoilExecution();
    static void keybinds();
    static std::string random_string(int length);
};

class CurrentWeapon {
public:
    static double BarrelAttachment();
    static double ScopeAttachment();
    static double x(int b);
    static double y(int b);
    static double Delay();
    static double Control(int bullet);
    static int Bullet();
    static bool WeaponActive();
    static void RecoilBreak(int count);
};
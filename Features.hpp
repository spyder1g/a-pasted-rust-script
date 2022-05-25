#include "Script.hpp"

class Features {
public:
    static DWORD CrosshairThreadFunc(void);
    static int crosshairthread();
    static std::string random_string(int length);
    static DWORDLONG random_number(DWORDLONG min, DWORDLONG max);
    static std::string randomclassname;
    static const std::string currentDateTime();
    static void ComboBox();
    static void MenuItems();
};
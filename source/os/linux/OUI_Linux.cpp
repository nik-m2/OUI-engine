
#if defined __linux

#include "os/linux/OUI_Linux.h"
#include "window/OUI_window.h"

oui::LinuxOS::LinuxOS() : OperatingSystem() {

}

bool oui::LinuxOS::initialize() {
    return true;
}

bool oui::LinuxOS::getMaximizeSize(oui::Window* window, int &x, int &y, int &width, int &height) {
    return true;
}

#endif
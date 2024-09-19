#include "logging/settings_logger.h"
#include "search_component_settings.h"

SearchComponentSettings SettingsLogger::getAllSettings() const {
    SearchComponentSettings settings;

    settings.m_name = getName();
    settings.m_main_settings = getComponentSettings();
    settings.m_sub_component_settings = getSubComponentSettings();
    return settings;
}
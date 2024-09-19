#ifndef SETTINGS_LOGGER_H_
#define SETTINGS_LOGGER_H_

#include "logging/search_component_settings.h"
#include "logging_terms.h"

#include <string>

/**
 * Defines an interface for a search component that has settings.
 */
class SettingsLogger {

public:
    /**
     * Default destructor.
     */
    virtual ~SettingsLogger() = default;

    /**
     * Returns the settings for this component.
     *
     * @return The settings for this component
     */
    SearchComponentSettings getAllSettings() const;

    /**
     * Gets the name of this search component.
     *
     * @return The name of this search component.
     */
    virtual std::string getName() const = 0;

protected:
    /**
     * Gets the settings for this component, not including sub-components.
     *
     * @return The settings for this component, but bot sub-components
     */
    virtual StringMap getComponentSettings() const = 0;

    /**
     * Gets the sub-component settings for this search component in a map. The key of each entry is the role it takes
     * relative to this search component.
     *
     * @return A map of the subcomponent settings.
     */
    virtual SearchSettingsMap getSubComponentSettings() const = 0;
};

#endif  //SETTINGS_LOGGER_H_

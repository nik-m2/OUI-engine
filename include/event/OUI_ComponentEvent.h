#ifndef OUI_COMPONENT_EVENT_H
#define OUI_COMPONENT_EVENT_H

#include "OUI_Export.h"
#include "event/OUI_Event2.h"
#include "components/OUI_Component.h"

namespace oui {

    /**
     * @brief An event that is triggered by a component
     * 
     */
    class OUI_API ComponentEvent : public oui::Event2 {

        public:

            /**
             * @brief The component that triggered the event
             */
            const Component* originalTarget;

            /**
             * @brief True if the event bubbles (propagates).
             */
            const bool bubbles;

            /**
             * @brief Creates a new component event with the specified type
             * 
             * @param type The type of evemt
             * @param typeHash an optional hashed version of `type`, which can be used for optimization
             */
            ComponentEvent(Component* originalTarget, std::string type, std::size_t typeHash=0, bool bubbles=true);

            /**
             * @brief Stops the event from bubbling to parent components
             */
            void stopPropagation();

            /**
             * @brief Returns true if bubbling should be stopped
             * 
             * @return true
             * @return false
             */
            bool isPropagationStopped();

            /**
             * @brief Creates an identical event with a different target
             */
            Component* getTarget();

            /**
             * @brief Creates an identical event with a different target
             */
            void setTarget(Component* target);

        private:
        
            /**
             * @brief The component that triggered the event
             */
            Component* target;

            /**
             * @brief True if bubbling should be stopped
             */
            bool propagationStopped;

    };

}
#endif
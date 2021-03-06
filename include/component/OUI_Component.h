#ifndef OUI_COMPONENT_H
#define OUI_COMPONENT_H

#include "OUI_Export.h"
#include "gfx/OUI_Graphics.h"
#include <vector>
#include <functional>
#include "component/attribute/OUI_StyleSheet.h"
#include "component/OUI_ComponentAttributeManager.h"
#include "component/location/OUI_LocationManager.h"

#include "event/OUI_EventDispatcher.h"

namespace oui {

    class Window;
    class Container;

    struct Bounds {
        int top, bottom;
        int left, right;
    };

    class Component {

        public:
            static Style* defaultStyle;

            static Style* getDefaultComponentStyle();

            const bool needsProcessing;

            void addEventListener(std::string type, EventHandler handler);
            void addEventConsumer(std::string type);
            void addEventDefaultConsumer(std::string type);
            void addEventBubbleConsumer(std::string type);

            virtual OUI_API void deriveAttributesForComponent(StyleSheet* styleSheet = NULL);

            virtual OUI_API ComponentAttributeManager* getAttributeManager();
            virtual OUI_API LocationManager* getLocationManager();

            std::u16string OUI_API getCursor();
            virtual OUI_API std::vector<std::u16string> getRightClickOptions();

        protected:
            Window* window;
            Container* parent;
            
            Graphics* graphics;
            ComponentAttributeManager* attributeManager;
            LocationManager* locationManager;
            EventDispatcher* eventDispatcher;

    //Identification
        private: std::string tag;
        private: std::string name;
        protected: std::vector<std::string> classes;

    //Cached calculations
        private: int x, y;
        private: int width, height;
        private: int screenX, screenY;

    //Graphics
        private: bool graphicsUpdate;

    //State
        bool hovered, mouseDown;
        protected: int mouseX;
        protected: int mouseY;
        bool selected;

    //Styling


/* END OF VARIABLES */

/* START OF METHODS */

    //Constructor/Destructor
        public: OUI_API ~Component();

        //Default attributes are not loaded until the component is added, so do not attempt to get attribute values in the constructor
        public: OUI_API Component(const std::string& tag, const std::string& name, const std::string& classes, bool needsProcessing=false, EventDispatcher* eventDispatcher=new EventDispatcher(), ComponentAttributeManager* attributeManager = new ComponentAttributeManager());

        public: void setAttribute(const std::string& name, Attribute val, const std::u16string& profile=u"default");

    //Processing and Drawing
        public: OUI_API virtual int process();
        public: OUI_API virtual void redraw();
        public: OUI_API void drawBorder();
        public: OUI_API virtual void addedToContainer(Container* container);

        public: EventDispatcher* getEventDispatcher();
        
    //Usage functions
        //This parses a value as if it were an OSAL attribute
        //This method is much slower than setAttribute, but handles multiple parameters in the value. E.g: "parseAttribute("text", u"'Hello There!'")"
        public: OUI_API void parseAttribute(const std::string& name, const std::u16string& value, const std::u16string& profile = u"default");
        public: void updateSize();

    //Containers
        public: OUI_API virtual bool isWindow();
        public: OUI_API virtual bool isContainer();
        public: OUI_API virtual Window* getWindow();
        public: OUI_API virtual Container* getParent();

    //Identification
        public: OUI_API std::string getTag();
        public: OUI_API bool compareTag(const std::string& tag);
        public: OUI_API virtual bool setName(const std::string& name);
        public: OUI_API std::string getName();
        public: OUI_API void addClass(std::string _class);
        public: OUI_API bool removeClass(const std::string& _class);
        public: OUI_API void setClasses(const std::string& classes);
        public: OUI_API void setClasses(std::vector<std::string> classes);
        public: OUI_API std::vector<std::string> getClasses();

    //Size & Location
        public: OUI_API bool isDependantOnParent();
        public: OUI_API int getWidth();
        public: OUI_API int getHeight();
        public: OUI_API int calculateWidth();
        public: OUI_API int calculateHeight();
        public: OUI_API int getRelativeX();
        public: OUI_API int getRelativeY();
        public: OUI_API int getWindowX();
        public: OUI_API int getWindowY();

    //Graphics
        public: OUI_API Graphics* getGraphics();
        public: OUI_API void flagGraphicsUpdate(bool flagParent = true);
        public: OUI_API bool needsGraphicsUpdate();
        
    //State
        public: virtual OUI_API int getMouseX();
        public: virtual OUI_API int getMouseY();
        //Sets this as the active component in the window
        public: OUI_API virtual void setSelected(bool selected);//TODO it's vague wether this is callable from a runtime
        public: OUI_API bool isSelected();
        //This is used to distinguish a click from a release
        public: OUI_API virtual void setMouseDown(bool mouseDown);
        public: OUI_API bool isMouseDown();
        public: OUI_API virtual void setHovered(bool hovered);
        public: OUI_API virtual bool isHovered();

        public: OUI_API virtual void refreshProfile();
/* END OF METHODS */
    };

}

#endif

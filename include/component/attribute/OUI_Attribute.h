#ifndef OUI_ATTRIBUTE_H
#define OUI_ATTRIBUTE_H

#include "OUI_Export.h"
#include "gfx/OUI_Color.h"
#include "osal/OSAL_Attribute.h"

namespace oui {

    class OUI_API Attribute {
        public: char type;

        //TODO these should be stored in a subclass of Attribute
        //      there would be no variables here, but the rest would stay the same
        public: int intVal = 0;
        public: std::u16string stringVal = u"";
        public: bool boolVal = false;
        public: double doubleVal = 0;
        public: Color colorVal = Color::BLACK;
        public: std::vector<Attribute> arrayVal;

        public: ~Attribute();
        public: Attribute();
        public: Attribute(const Attribute& a);
        public: Attribute(int value);
        public: Attribute(std::string value);
        public: Attribute(const char* value);
        public: Attribute(std::u16string value);
        public: Attribute(const char16_t* value);
        public: Attribute(bool value);
        public: Attribute(double value);
        public: Attribute(Color value);
        public: Attribute(std::vector<Attribute> value);
        public: Attribute(OSAL::Attribute value);

        public: std::u16string asString();
        public: int asInt();
        public: bool asBool();
        public: double asDouble();
        public: Color asColor();
        public: std::vector<Attribute> asArray();
        public: std::vector<std::u16string> asStringArray();

        public: std::u16string toString();
        public: bool equals(Attribute other);
        public: Attribute* clone();
    };

}

#endif
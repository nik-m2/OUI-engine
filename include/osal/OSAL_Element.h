#include <unordered_map>
#include <vector>
#include "osal/OSAL_Attribute.h"

/*
* This class represents an element with attributes
*/
namespace OSAL {
    class Element {
        public: enum Type {
            TAG,
            CLASS,
            NAME
        };

        private: Type type;//prefix: '#'=2, ".'=1, nothing=0
        private: std::string name;//The name of the element (not including prefix)
        private: std::unordered_map<std::u16string, std::vector<Attribute>> attributes;//The attributes of this element

        public: Element();
        public: Element(Type type, const std::string& name, const std::unordered_map<std::u16string, std::vector<Attribute>>& attributes);
        public: Element(const std::string& name, const std::unordered_map<std::u16string, std::vector<Attribute>>& attributes);
        private: void setValues(Type type, const std::string& name, const std::unordered_map<std::u16string, std::vector<Attribute>>& attributes);

        public: void combine(Element e);

        public: Type getType();
        public: std::string getName();
        public: std::vector<Attribute> getAttributes(const std::u16string& profile);
        public: Attribute getAttribute(const std::string& name, const std::u16string& profile);

        public: std::unordered_map<std::u16string, std::vector<Attribute>> getAttributes();

        public: bool isValid();
    };
}
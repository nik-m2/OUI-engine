#include "component/attribute/OUI_AttributeProfile.h"

#include <iostream>//TODO remove
#include "OUI.h"//TODO remove

#include "exception/OUI_ArgumentException.h"

oui::AttributeProfile::~AttributeProfile() {
    
    //Loop through all attributes and delete
    for (auto it = attributes.begin(); it != attributes.end();) {
        //Attribute* a = it->second;
        it = attributes.erase(it);
        //delete a;
    }
    attributes.clear();

}

oui::AttributeProfile::AttributeProfile(const std::string& componentName, AttributeProfile* defaultProfile) :
    componentName{componentName},
    attributes{ std::unordered_map<std::string, Attribute>() } {
    
    if (defaultProfile != NULL) {
        this->defaultProfiles.push_back(defaultProfile);
    }
}

void oui::AttributeProfile::clearDefaultProfiles() {
    this->defaultProfiles.clear();
 }

void oui::AttributeProfile::addDefaultProfile(AttributeProfile* defaultProfile) {
    this->defaultProfiles.push_back(defaultProfile);
 }


std::vector<oui::AttributeProfile*> oui::AttributeProfile::getDefaultProfiles() {
    return defaultProfiles;
}

void oui::AttributeProfile::parseAttribute(const std::string& name, const std::u16string& value) {

    //This parses the attribute to create an OSAL Attribute
    OSAL::Attribute osalAttribute = OSAL::Attribute(name, value);

    //Set the attribute, performing needed substitutions
    setAttribute(name, osalAttribute);

}

void oui::AttributeProfile::setAttribute(const std::string& name, OSAL::Attribute value)  {

    // Perform any substitutions
    if (AttributeSubstitution::applySubstitution(name, value, this)) {
        return; // `AttributeSubstitution::applySubstitution` recursively calls this function with the substitutions
    }

    //Delete the attribute if it already exists
    auto it = attributes.find(name);
    if (it != attributes.end()) {//TOOD deletion could be avoided if we reuse the attribute (does this affect speed?)
        //delete it->second;
        attributes.erase(it);
    }

    //Populate the new attribute value
    Attribute val = Attribute(value);

    //Finally, add the attribute to our attributes list
    attributes.insert({name, val});
}

void oui::AttributeProfile::setAttribute(const std::string& name, Attribute value) {

    //See if we can get a valid substitution
    if (AttributeSubstitution::hasSubstitution(name)) {

        //If the attribute is a string, we will put it back in automatically
        if (value.type == OSAL::TYPE_STRING) {
            setAttribute(name, OSAL::Attribute(name, value.stringVal));
            return;
        }
    }

    //Delete the attribute if it already exists
    auto it = attributes.find(name);
    if (it != attributes.end()) {

        //Return if the name with specified value already exists
        if (it->second.equals(value)) {
            return;
        }

        //Delete the attribute to make room for new value
        //delete it->second;
        attributes.erase(it);
    }

    //Create and insert the new value
    attributes.insert({name, value});

}

/*void oui::AttributeProfile::setAttribute(const std::u16string& name, Attribute* value) {

    //Delete the attribute if it already exists
    std::unordered_map<std::u16string, Attribute*>::iterator it = attributes.find(name);
    if (it != attributes.end()) {
        delete it->second;
        attributes.erase(it);
    }

    //Create and insert the new value
    attributes.insert({name, value});

}*/

oui::Attribute* oui::AttributeProfile::hasAttribute(const std::string& name) {

    auto it = attributes.find(name);

    if (it != attributes.end()) {
        return &(it->second);
    }

    for (int i = 0; i < defaultProfiles.size(); i++) {
        Attribute* attr = defaultProfiles.at(i)->hasAttribute(name);
        if (attr != NULL) {
            return attr;
        }
    }

    return NULL;
}

oui::Attribute* oui::AttributeProfile::getAttribute(const std::string& name) {
    return hasAttribute(name);
}

int oui::AttributeProfile::getInt(const std::string& name) {

    Attribute* attribute = hasAttribute(name);

    if (attribute == NULL) {
        return 0;
    }

    return attribute->asInt();
}

std::u16string oui::AttributeProfile::getString(const std::string& name) {

    Attribute* attribute = hasAttribute(name);

    if (attribute == NULL) {
        return u"";
    }

    return attribute->asString();
}

bool oui::AttributeProfile::getBool(const std::string& name) {

    Attribute* attribute = hasAttribute(name);

    if (attribute == NULL) {
        return false;
    }

    //Attribute exists and is a bool
    return attribute->asBool();
}

double oui::AttributeProfile::getDouble(const std::string& name) {

    Attribute* attribute = hasAttribute(name);

    if (attribute == NULL) {
        return 0.0;
    }

    return attribute->asDouble();
}

oui::Color oui::AttributeProfile::getColor(const std::string& name) {
    Attribute* attribute = hasAttribute(name);

    if (attribute == NULL) {
        return Color::BLACK;
    }

    return attribute->asColor();
}

std::vector<oui::Attribute> oui::AttributeProfile::getArray(const std::string& name) {
    Attribute* attribute = hasAttribute(name);

    if (attribute == NULL) {
        return std::vector<Attribute>();
    }

    //Attribute exists and is a double
    return attribute->asArray();
}

std::vector<std::u16string> oui::AttributeProfile::getStringArray(const std::string& name) {
    Attribute* attribute = hasAttribute(name);

    if (attribute == NULL) {
        return std::vector<std::u16string>();
    }

    return attribute->asStringArray();
}

void oui::AttributeProfile::removeAttribute(const std::string& name) {//TODO substitutions

    auto it = attributes.find(name);

    //Delete attribute if it exists
    if (it != attributes.end()) {
        attributes.erase(it);
    }

}

void oui::AttributeProfile::combineProfile(AttributeProfile* profile, bool overwrite) {

    auto i = profile->attributes.begin();

    //Loop through the new profile's attributes
    while (i != profile->attributes.end()) {

        //Set attribute if overwrite is true, or if it doesn't exist in this sheet
        if (overwrite || attributes.find(i->first) == attributes.end()) {
            setAttribute(i->first, i->second);
        }

        i++;
    }

}

bool oui::AttributeProfile::equals(AttributeProfile* other) {

    //Return false if non-existant
    if (other == NULL) {
        return false;
    }
    
    //Return false if they have different amount of attributes
    if (attributes.size() != other->attributes.size()) {
        return false;
    }

    auto thisIt = attributes.begin();
    auto otherIt = other->attributes.begin();

    //Loop through all attributes
    while (thisIt != attributes.end()) {

        //Return false if attributes are not equal
        if (!thisIt->second.equals(otherIt->second)) {
            return false;
        }

        thisIt++;
        otherIt++;
    }

    return true;
}

oui::AttributeProfile* oui::AttributeProfile::clone() {

    //Create new profile to store clone
    AttributeProfile* clone = new AttributeProfile(componentName);

    //Loop through and clone attributes
    auto it = attributes.begin();
    while (it != attributes.end()) {
        clone->setAttribute(it->first, it->second);
        it++;
    }

    auto it2 = defaultProfiles.begin();
    while (it2 != defaultProfiles.end()) {
        clone->addDefaultProfile(*it2);
        it2++;
    }

    return clone;

}

void oui::AttributeProfile::combineDefaults(AttributeProfile* profileWithDefaults) {

    std::vector<AttributeProfile*> profiles = profileWithDefaults->getDefaultProfiles();
    for(auto it = profiles.begin(); it != profiles.end(); ++it) {
        AttributeProfile* defaultProfile = *it;
        combineProfile(defaultProfile);
        combineDefaults(defaultProfile);
    }

}

oui::AttributeProfile* oui::AttributeProfile::flatten() {

    //Create new profile
    AttributeProfile* flatProfile = new AttributeProfile(componentName);
    flatProfile->combineProfile(this);
    flatProfile->combineDefaults(this);
    return flatProfile;
}
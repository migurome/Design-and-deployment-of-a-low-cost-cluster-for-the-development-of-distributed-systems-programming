//
// Generated file, do not edit! Created by nedtool 4.6 from Messages/SIMCAN_App_MEM_Message.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "SIMCAN_App_MEM_Message_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

SIMCAN_App_MEM_Message_Base::SIMCAN_App_MEM_Message_Base(const char *name, int kind) : ::SIMCAN_Message(name,kind)
{
    this->memSize_var = 0;
    this->region_var = 0;
}

SIMCAN_App_MEM_Message_Base::SIMCAN_App_MEM_Message_Base(const SIMCAN_App_MEM_Message_Base& other) : ::SIMCAN_Message(other)
{
    copy(other);
}

SIMCAN_App_MEM_Message_Base::~SIMCAN_App_MEM_Message_Base()
{
}

SIMCAN_App_MEM_Message_Base& SIMCAN_App_MEM_Message_Base::operator=(const SIMCAN_App_MEM_Message_Base& other)
{
    if (this==&other) return *this;
    ::SIMCAN_Message::operator=(other);
    copy(other);
    return *this;
}

void SIMCAN_App_MEM_Message_Base::copy(const SIMCAN_App_MEM_Message_Base& other)
{
    this->memSize_var = other.memSize_var;
    this->region_var = other.region_var;
}

void SIMCAN_App_MEM_Message_Base::parsimPack(cCommBuffer *b)
{
    ::SIMCAN_Message::parsimPack(b);
    doPacking(b,this->memSize_var);
    doPacking(b,this->region_var);
}

void SIMCAN_App_MEM_Message_Base::parsimUnpack(cCommBuffer *b)
{
    ::SIMCAN_Message::parsimUnpack(b);
    doUnpacking(b,this->memSize_var);
    doUnpacking(b,this->region_var);
}

unsigned int SIMCAN_App_MEM_Message_Base::getMemSize() const
{
    return memSize_var;
}

void SIMCAN_App_MEM_Message_Base::setMemSize(unsigned int memSize)
{
    this->memSize_var = memSize;
}

unsigned int SIMCAN_App_MEM_Message_Base::getRegion() const
{
    return region_var;
}

void SIMCAN_App_MEM_Message_Base::setRegion(unsigned int region)
{
    this->region_var = region;
}

class SIMCAN_App_MEM_MessageDescriptor : public cClassDescriptor
{
  public:
    SIMCAN_App_MEM_MessageDescriptor();
    virtual ~SIMCAN_App_MEM_MessageDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(SIMCAN_App_MEM_MessageDescriptor);

SIMCAN_App_MEM_MessageDescriptor::SIMCAN_App_MEM_MessageDescriptor() : cClassDescriptor("SIMCAN_App_MEM_Message", "SIMCAN_Message")
{
}

SIMCAN_App_MEM_MessageDescriptor::~SIMCAN_App_MEM_MessageDescriptor()
{
}

bool SIMCAN_App_MEM_MessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SIMCAN_App_MEM_Message_Base *>(obj)!=NULL;
}

const char *SIMCAN_App_MEM_MessageDescriptor::getProperty(const char *propertyname) const
{
    if (!strcmp(propertyname,"customize")) return "true";
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SIMCAN_App_MEM_MessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int SIMCAN_App_MEM_MessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *SIMCAN_App_MEM_MessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "memSize",
        "region",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int SIMCAN_App_MEM_MessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='m' && strcmp(fieldName, "memSize")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "region")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SIMCAN_App_MEM_MessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",
        "unsigned int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *SIMCAN_App_MEM_MessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int SIMCAN_App_MEM_MessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_App_MEM_Message_Base *pp = (SIMCAN_App_MEM_Message_Base *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SIMCAN_App_MEM_MessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_App_MEM_Message_Base *pp = (SIMCAN_App_MEM_Message_Base *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getMemSize());
        case 1: return ulong2string(pp->getRegion());
        default: return "";
    }
}

bool SIMCAN_App_MEM_MessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_App_MEM_Message_Base *pp = (SIMCAN_App_MEM_Message_Base *)object; (void)pp;
    switch (field) {
        case 0: pp->setMemSize(string2ulong(value)); return true;
        case 1: pp->setRegion(string2ulong(value)); return true;
        default: return false;
    }
}

const char *SIMCAN_App_MEM_MessageDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *SIMCAN_App_MEM_MessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_App_MEM_Message_Base *pp = (SIMCAN_App_MEM_Message_Base *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



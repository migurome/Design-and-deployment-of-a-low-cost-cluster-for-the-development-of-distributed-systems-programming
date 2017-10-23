//
// Generated file, do not edit! Created by nedtool 4.6 from Messages/SIMCAN_BlockList_Message.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "SIMCAN_BlockList_Message_m.h"

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

SIMCAN_BlockList_Message_Base::SIMCAN_BlockList_Message_Base(const char *name, int kind) : ::SIMCAN_App_IO_Message(name,kind)
{
}

SIMCAN_BlockList_Message_Base::SIMCAN_BlockList_Message_Base(const SIMCAN_BlockList_Message_Base& other) : ::SIMCAN_App_IO_Message(other)
{
    copy(other);
}

SIMCAN_BlockList_Message_Base::~SIMCAN_BlockList_Message_Base()
{
}

SIMCAN_BlockList_Message_Base& SIMCAN_BlockList_Message_Base::operator=(const SIMCAN_BlockList_Message_Base& other)
{
    if (this==&other) return *this;
    ::SIMCAN_App_IO_Message::operator=(other);
    copy(other);
    return *this;
}

void SIMCAN_BlockList_Message_Base::copy(const SIMCAN_BlockList_Message_Base& other)
{
    this->file_var = other.file_var;
}

void SIMCAN_BlockList_Message_Base::parsimPack(cCommBuffer *b)
{
    ::SIMCAN_App_IO_Message::parsimPack(b);
    doPacking(b,this->file_var);
}

void SIMCAN_BlockList_Message_Base::parsimUnpack(cCommBuffer *b)
{
    ::SIMCAN_App_IO_Message::parsimUnpack(b);
    doUnpacking(b,this->file_var);
}

SIMCAN_File& SIMCAN_BlockList_Message_Base::getFile()
{
    return file_var;
}

void SIMCAN_BlockList_Message_Base::setFile(const SIMCAN_File& file)
{
    this->file_var = file;
}

class SIMCAN_BlockList_MessageDescriptor : public cClassDescriptor
{
  public:
    SIMCAN_BlockList_MessageDescriptor();
    virtual ~SIMCAN_BlockList_MessageDescriptor();

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

Register_ClassDescriptor(SIMCAN_BlockList_MessageDescriptor);

SIMCAN_BlockList_MessageDescriptor::SIMCAN_BlockList_MessageDescriptor() : cClassDescriptor("SIMCAN_BlockList_Message", "SIMCAN_App_IO_Message")
{
}

SIMCAN_BlockList_MessageDescriptor::~SIMCAN_BlockList_MessageDescriptor()
{
}

bool SIMCAN_BlockList_MessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SIMCAN_BlockList_Message_Base *>(obj)!=NULL;
}

const char *SIMCAN_BlockList_MessageDescriptor::getProperty(const char *propertyname) const
{
    if (!strcmp(propertyname,"customize")) return "true";
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SIMCAN_BlockList_MessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int SIMCAN_BlockList_MessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *SIMCAN_BlockList_MessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "file",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int SIMCAN_BlockList_MessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "file")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SIMCAN_BlockList_MessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "SIMCAN_File",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *SIMCAN_BlockList_MessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int SIMCAN_BlockList_MessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_BlockList_Message_Base *pp = (SIMCAN_BlockList_Message_Base *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SIMCAN_BlockList_MessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_BlockList_Message_Base *pp = (SIMCAN_BlockList_Message_Base *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getFile(); return out.str();}
        default: return "";
    }
}

bool SIMCAN_BlockList_MessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_BlockList_Message_Base *pp = (SIMCAN_BlockList_Message_Base *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *SIMCAN_BlockList_MessageDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return opp_typename(typeid(SIMCAN_File));
        default: return NULL;
    };
}

void *SIMCAN_BlockList_MessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_BlockList_Message_Base *pp = (SIMCAN_BlockList_Message_Base *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getFile()); break;
        default: return NULL;
    }
}



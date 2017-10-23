//
// Generated file, do not edit! Created by nedtool 4.6 from Messages/SIMCAN_App_IO_Message.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "SIMCAN_App_IO_Message_m.h"

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

SIMCAN_App_IO_Message_Base::SIMCAN_App_IO_Message_Base(const char *name, int kind) : ::SIMCAN_Message(name,kind)
{
    this->fileName_var = "";
    this->offset_var = 0;
    this->size_var = 0;
    this->isPending_var = false;
}

SIMCAN_App_IO_Message_Base::SIMCAN_App_IO_Message_Base(const SIMCAN_App_IO_Message_Base& other) : ::SIMCAN_Message(other)
{
    copy(other);
}

SIMCAN_App_IO_Message_Base::~SIMCAN_App_IO_Message_Base()
{
}

SIMCAN_App_IO_Message_Base& SIMCAN_App_IO_Message_Base::operator=(const SIMCAN_App_IO_Message_Base& other)
{
    if (this==&other) return *this;
    ::SIMCAN_Message::operator=(other);
    copy(other);
    return *this;
}

void SIMCAN_App_IO_Message_Base::copy(const SIMCAN_App_IO_Message_Base& other)
{
    this->fileName_var = other.fileName_var;
    this->offset_var = other.offset_var;
    this->size_var = other.size_var;
    this->isPending_var = other.isPending_var;
}

void SIMCAN_App_IO_Message_Base::parsimPack(cCommBuffer *b)
{
    ::SIMCAN_Message::parsimPack(b);
    doPacking(b,this->fileName_var);
    doPacking(b,this->offset_var);
    doPacking(b,this->size_var);
    doPacking(b,this->isPending_var);
}

void SIMCAN_App_IO_Message_Base::parsimUnpack(cCommBuffer *b)
{
    ::SIMCAN_Message::parsimUnpack(b);
    doUnpacking(b,this->fileName_var);
    doUnpacking(b,this->offset_var);
    doUnpacking(b,this->size_var);
    doUnpacking(b,this->isPending_var);
}

const char * SIMCAN_App_IO_Message_Base::getFileName() const
{
    return fileName_var.c_str();
}

void SIMCAN_App_IO_Message_Base::setFileName(const char * fileName)
{
    this->fileName_var = fileName;
}

unsigned int SIMCAN_App_IO_Message_Base::getOffset() const
{
    return offset_var;
}

void SIMCAN_App_IO_Message_Base::setOffset(unsigned int offset)
{
    this->offset_var = offset;
}

unsigned int SIMCAN_App_IO_Message_Base::getSize() const
{
    return size_var;
}

void SIMCAN_App_IO_Message_Base::setSize(unsigned int size)
{
    this->size_var = size;
}

bool SIMCAN_App_IO_Message_Base::getIsPending() const
{
    return isPending_var;
}

void SIMCAN_App_IO_Message_Base::setIsPending(bool isPending)
{
    this->isPending_var = isPending;
}

class SIMCAN_App_IO_MessageDescriptor : public cClassDescriptor
{
  public:
    SIMCAN_App_IO_MessageDescriptor();
    virtual ~SIMCAN_App_IO_MessageDescriptor();

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

Register_ClassDescriptor(SIMCAN_App_IO_MessageDescriptor);

SIMCAN_App_IO_MessageDescriptor::SIMCAN_App_IO_MessageDescriptor() : cClassDescriptor("SIMCAN_App_IO_Message", "SIMCAN_Message")
{
}

SIMCAN_App_IO_MessageDescriptor::~SIMCAN_App_IO_MessageDescriptor()
{
}

bool SIMCAN_App_IO_MessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SIMCAN_App_IO_Message_Base *>(obj)!=NULL;
}

const char *SIMCAN_App_IO_MessageDescriptor::getProperty(const char *propertyname) const
{
    if (!strcmp(propertyname,"customize")) return "true";
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SIMCAN_App_IO_MessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int SIMCAN_App_IO_MessageDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *SIMCAN_App_IO_MessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "fileName",
        "offset",
        "size",
        "isPending",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int SIMCAN_App_IO_MessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "fileName")==0) return base+0;
    if (fieldName[0]=='o' && strcmp(fieldName, "offset")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "size")==0) return base+2;
    if (fieldName[0]=='i' && strcmp(fieldName, "isPending")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SIMCAN_App_IO_MessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "unsigned int",
        "unsigned int",
        "bool",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *SIMCAN_App_IO_MessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int SIMCAN_App_IO_MessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_App_IO_Message_Base *pp = (SIMCAN_App_IO_Message_Base *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SIMCAN_App_IO_MessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_App_IO_Message_Base *pp = (SIMCAN_App_IO_Message_Base *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getFileName());
        case 1: return ulong2string(pp->getOffset());
        case 2: return ulong2string(pp->getSize());
        case 3: return bool2string(pp->getIsPending());
        default: return "";
    }
}

bool SIMCAN_App_IO_MessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_App_IO_Message_Base *pp = (SIMCAN_App_IO_Message_Base *)object; (void)pp;
    switch (field) {
        case 0: pp->setFileName((value)); return true;
        case 1: pp->setOffset(string2ulong(value)); return true;
        case 2: pp->setSize(string2ulong(value)); return true;
        case 3: pp->setIsPending(string2bool(value)); return true;
        default: return false;
    }
}

const char *SIMCAN_App_IO_MessageDescriptor::getFieldStructName(void *object, int field) const
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

void *SIMCAN_App_IO_MessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_App_IO_Message_Base *pp = (SIMCAN_App_IO_Message_Base *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



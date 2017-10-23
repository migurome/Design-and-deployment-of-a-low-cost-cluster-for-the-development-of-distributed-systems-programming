//
// Generated file, do not edit! Created by nedtool 4.6 from Messages/SIMCAN_App_NET_Message.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "SIMCAN_App_NET_Message_m.h"

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

SIMCAN_App_NET_Message_Base::SIMCAN_App_NET_Message_Base(const char *name, int kind) : ::SIMCAN_Message(name,kind)
{
    this->destinationIP_var = "";
    this->localIP_var = "";
    this->destinationPort_var = 0;
    this->localPort_var = 0;
    this->size_var = 0;
    this->data_var = "";
}

SIMCAN_App_NET_Message_Base::SIMCAN_App_NET_Message_Base(const SIMCAN_App_NET_Message_Base& other) : ::SIMCAN_Message(other)
{
    copy(other);
}

SIMCAN_App_NET_Message_Base::~SIMCAN_App_NET_Message_Base()
{
}

SIMCAN_App_NET_Message_Base& SIMCAN_App_NET_Message_Base::operator=(const SIMCAN_App_NET_Message_Base& other)
{
    if (this==&other) return *this;
    ::SIMCAN_Message::operator=(other);
    copy(other);
    return *this;
}

void SIMCAN_App_NET_Message_Base::copy(const SIMCAN_App_NET_Message_Base& other)
{
    this->destinationIP_var = other.destinationIP_var;
    this->localIP_var = other.localIP_var;
    this->destinationPort_var = other.destinationPort_var;
    this->localPort_var = other.localPort_var;
    this->size_var = other.size_var;
    this->data_var = other.data_var;
}

void SIMCAN_App_NET_Message_Base::parsimPack(cCommBuffer *b)
{
    ::SIMCAN_Message::parsimPack(b);
    doPacking(b,this->destinationIP_var);
    doPacking(b,this->localIP_var);
    doPacking(b,this->destinationPort_var);
    doPacking(b,this->localPort_var);
    doPacking(b,this->size_var);
    doPacking(b,this->data_var);
}

void SIMCAN_App_NET_Message_Base::parsimUnpack(cCommBuffer *b)
{
    ::SIMCAN_Message::parsimUnpack(b);
    doUnpacking(b,this->destinationIP_var);
    doUnpacking(b,this->localIP_var);
    doUnpacking(b,this->destinationPort_var);
    doUnpacking(b,this->localPort_var);
    doUnpacking(b,this->size_var);
    doUnpacking(b,this->data_var);
}

const char * SIMCAN_App_NET_Message_Base::getDestinationIP() const
{
    return destinationIP_var.c_str();
}

void SIMCAN_App_NET_Message_Base::setDestinationIP(const char * destinationIP)
{
    this->destinationIP_var = destinationIP;
}

const char * SIMCAN_App_NET_Message_Base::getLocalIP() const
{
    return localIP_var.c_str();
}

void SIMCAN_App_NET_Message_Base::setLocalIP(const char * localIP)
{
    this->localIP_var = localIP;
}

int SIMCAN_App_NET_Message_Base::getDestinationPort() const
{
    return destinationPort_var;
}

void SIMCAN_App_NET_Message_Base::setDestinationPort(int destinationPort)
{
    this->destinationPort_var = destinationPort;
}

int SIMCAN_App_NET_Message_Base::getLocalPort() const
{
    return localPort_var;
}

void SIMCAN_App_NET_Message_Base::setLocalPort(int localPort)
{
    this->localPort_var = localPort;
}

unsigned int SIMCAN_App_NET_Message_Base::getSize() const
{
    return size_var;
}

void SIMCAN_App_NET_Message_Base::setSize(unsigned int size)
{
    this->size_var = size;
}

const char * SIMCAN_App_NET_Message_Base::getData() const
{
    return data_var.c_str();
}

void SIMCAN_App_NET_Message_Base::setData(const char * data)
{
    this->data_var = data;
}

class SIMCAN_App_NET_MessageDescriptor : public cClassDescriptor
{
  public:
    SIMCAN_App_NET_MessageDescriptor();
    virtual ~SIMCAN_App_NET_MessageDescriptor();

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

Register_ClassDescriptor(SIMCAN_App_NET_MessageDescriptor);

SIMCAN_App_NET_MessageDescriptor::SIMCAN_App_NET_MessageDescriptor() : cClassDescriptor("SIMCAN_App_NET_Message", "SIMCAN_Message")
{
}

SIMCAN_App_NET_MessageDescriptor::~SIMCAN_App_NET_MessageDescriptor()
{
}

bool SIMCAN_App_NET_MessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SIMCAN_App_NET_Message_Base *>(obj)!=NULL;
}

const char *SIMCAN_App_NET_MessageDescriptor::getProperty(const char *propertyname) const
{
    if (!strcmp(propertyname,"customize")) return "true";
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SIMCAN_App_NET_MessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount(object) : 6;
}

unsigned int SIMCAN_App_NET_MessageDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *SIMCAN_App_NET_MessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "destinationIP",
        "localIP",
        "destinationPort",
        "localPort",
        "size",
        "data",
    };
    return (field>=0 && field<6) ? fieldNames[field] : NULL;
}

int SIMCAN_App_NET_MessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationIP")==0) return base+0;
    if (fieldName[0]=='l' && strcmp(fieldName, "localIP")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationPort")==0) return base+2;
    if (fieldName[0]=='l' && strcmp(fieldName, "localPort")==0) return base+3;
    if (fieldName[0]=='s' && strcmp(fieldName, "size")==0) return base+4;
    if (fieldName[0]=='d' && strcmp(fieldName, "data")==0) return base+5;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SIMCAN_App_NET_MessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "string",
        "int",
        "int",
        "unsigned int",
        "string",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : NULL;
}

const char *SIMCAN_App_NET_MessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int SIMCAN_App_NET_MessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_App_NET_Message_Base *pp = (SIMCAN_App_NET_Message_Base *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SIMCAN_App_NET_MessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_App_NET_Message_Base *pp = (SIMCAN_App_NET_Message_Base *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getDestinationIP());
        case 1: return oppstring2string(pp->getLocalIP());
        case 2: return long2string(pp->getDestinationPort());
        case 3: return long2string(pp->getLocalPort());
        case 4: return ulong2string(pp->getSize());
        case 5: return oppstring2string(pp->getData());
        default: return "";
    }
}

bool SIMCAN_App_NET_MessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_App_NET_Message_Base *pp = (SIMCAN_App_NET_Message_Base *)object; (void)pp;
    switch (field) {
        case 0: pp->setDestinationIP((value)); return true;
        case 1: pp->setLocalIP((value)); return true;
        case 2: pp->setDestinationPort(string2long(value)); return true;
        case 3: pp->setLocalPort(string2long(value)); return true;
        case 4: pp->setSize(string2ulong(value)); return true;
        case 5: pp->setData((value)); return true;
        default: return false;
    }
}

const char *SIMCAN_App_NET_MessageDescriptor::getFieldStructName(void *object, int field) const
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

void *SIMCAN_App_NET_MessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_App_NET_Message_Base *pp = (SIMCAN_App_NET_Message_Base *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



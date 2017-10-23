//
// Generated file, do not edit! Created by nedtool 4.6 from Messages/SIMCAN_App_CPU_Message.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "SIMCAN_App_CPU_Message_m.h"

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

SIMCAN_App_CPU_Message_Base::SIMCAN_App_CPU_Message_Base(const char *name, int kind) : ::SIMCAN_Message(name,kind)
{
    this->cpuTime_var = 0.0;
    this->totalMIs_var = 0;
    this->remainingMIs_var = 0;
    this->quantum_var = 0;
    this->cpuPriority_var = 0;
}

SIMCAN_App_CPU_Message_Base::SIMCAN_App_CPU_Message_Base(const SIMCAN_App_CPU_Message_Base& other) : ::SIMCAN_Message(other)
{
    copy(other);
}

SIMCAN_App_CPU_Message_Base::~SIMCAN_App_CPU_Message_Base()
{
}

SIMCAN_App_CPU_Message_Base& SIMCAN_App_CPU_Message_Base::operator=(const SIMCAN_App_CPU_Message_Base& other)
{
    if (this==&other) return *this;
    ::SIMCAN_Message::operator=(other);
    copy(other);
    return *this;
}

void SIMCAN_App_CPU_Message_Base::copy(const SIMCAN_App_CPU_Message_Base& other)
{
    this->cpuTime_var = other.cpuTime_var;
    this->totalMIs_var = other.totalMIs_var;
    this->remainingMIs_var = other.remainingMIs_var;
    this->quantum_var = other.quantum_var;
    this->cpuPriority_var = other.cpuPriority_var;
}

void SIMCAN_App_CPU_Message_Base::parsimPack(cCommBuffer *b)
{
    ::SIMCAN_Message::parsimPack(b);
    doPacking(b,this->cpuTime_var);
    doPacking(b,this->totalMIs_var);
    doPacking(b,this->remainingMIs_var);
    doPacking(b,this->quantum_var);
    doPacking(b,this->cpuPriority_var);
}

void SIMCAN_App_CPU_Message_Base::parsimUnpack(cCommBuffer *b)
{
    ::SIMCAN_Message::parsimUnpack(b);
    doUnpacking(b,this->cpuTime_var);
    doUnpacking(b,this->totalMIs_var);
    doUnpacking(b,this->remainingMIs_var);
    doUnpacking(b,this->quantum_var);
    doUnpacking(b,this->cpuPriority_var);
}

simtime_t SIMCAN_App_CPU_Message_Base::getCpuTime() const
{
    return cpuTime_var;
}

void SIMCAN_App_CPU_Message_Base::setCpuTime(simtime_t cpuTime)
{
    this->cpuTime_var = cpuTime;
}

unsigned long SIMCAN_App_CPU_Message_Base::getTotalMIs() const
{
    return totalMIs_var;
}

void SIMCAN_App_CPU_Message_Base::setTotalMIs(unsigned long totalMIs)
{
    this->totalMIs_var = totalMIs;
}

unsigned long SIMCAN_App_CPU_Message_Base::getRemainingMIs() const
{
    return remainingMIs_var;
}

void SIMCAN_App_CPU_Message_Base::setRemainingMIs(unsigned long remainingMIs)
{
    this->remainingMIs_var = remainingMIs;
}

int SIMCAN_App_CPU_Message_Base::getQuantum() const
{
    return quantum_var;
}

void SIMCAN_App_CPU_Message_Base::setQuantum(int quantum)
{
    this->quantum_var = quantum;
}

int SIMCAN_App_CPU_Message_Base::getCpuPriority() const
{
    return cpuPriority_var;
}

void SIMCAN_App_CPU_Message_Base::setCpuPriority(int cpuPriority)
{
    this->cpuPriority_var = cpuPriority;
}

class SIMCAN_App_CPU_MessageDescriptor : public cClassDescriptor
{
  public:
    SIMCAN_App_CPU_MessageDescriptor();
    virtual ~SIMCAN_App_CPU_MessageDescriptor();

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

Register_ClassDescriptor(SIMCAN_App_CPU_MessageDescriptor);

SIMCAN_App_CPU_MessageDescriptor::SIMCAN_App_CPU_MessageDescriptor() : cClassDescriptor("SIMCAN_App_CPU_Message", "SIMCAN_Message")
{
}

SIMCAN_App_CPU_MessageDescriptor::~SIMCAN_App_CPU_MessageDescriptor()
{
}

bool SIMCAN_App_CPU_MessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SIMCAN_App_CPU_Message_Base *>(obj)!=NULL;
}

const char *SIMCAN_App_CPU_MessageDescriptor::getProperty(const char *propertyname) const
{
    if (!strcmp(propertyname,"customize")) return "true";
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SIMCAN_App_CPU_MessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int SIMCAN_App_CPU_MessageDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *SIMCAN_App_CPU_MessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "cpuTime",
        "totalMIs",
        "remainingMIs",
        "quantum",
        "cpuPriority",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int SIMCAN_App_CPU_MessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "cpuTime")==0) return base+0;
    if (fieldName[0]=='t' && strcmp(fieldName, "totalMIs")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "remainingMIs")==0) return base+2;
    if (fieldName[0]=='q' && strcmp(fieldName, "quantum")==0) return base+3;
    if (fieldName[0]=='c' && strcmp(fieldName, "cpuPriority")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SIMCAN_App_CPU_MessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "simtime_t",
        "unsigned long",
        "unsigned long",
        "int",
        "int",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *SIMCAN_App_CPU_MessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int SIMCAN_App_CPU_MessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_App_CPU_Message_Base *pp = (SIMCAN_App_CPU_Message_Base *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SIMCAN_App_CPU_MessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_App_CPU_Message_Base *pp = (SIMCAN_App_CPU_Message_Base *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getCpuTime());
        case 1: return ulong2string(pp->getTotalMIs());
        case 2: return ulong2string(pp->getRemainingMIs());
        case 3: return long2string(pp->getQuantum());
        case 4: return long2string(pp->getCpuPriority());
        default: return "";
    }
}

bool SIMCAN_App_CPU_MessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_App_CPU_Message_Base *pp = (SIMCAN_App_CPU_Message_Base *)object; (void)pp;
    switch (field) {
        case 0: pp->setCpuTime(string2double(value)); return true;
        case 1: pp->setTotalMIs(string2ulong(value)); return true;
        case 2: pp->setRemainingMIs(string2ulong(value)); return true;
        case 3: pp->setQuantum(string2long(value)); return true;
        case 4: pp->setCpuPriority(string2long(value)); return true;
        default: return false;
    }
}

const char *SIMCAN_App_CPU_MessageDescriptor::getFieldStructName(void *object, int field) const
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

void *SIMCAN_App_CPU_MessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_App_CPU_Message_Base *pp = (SIMCAN_App_CPU_Message_Base *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



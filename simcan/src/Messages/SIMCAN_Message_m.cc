//
// Generated file, do not edit! Created by nedtool 4.6 from Messages/SIMCAN_Message.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "SIMCAN_Message_m.h"

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

SIMCAN_Message_Base::SIMCAN_Message_Base(const char *name, int kind) : ::cPacket(name,kind)
{
    this->operation_var = -1;
    this->isResponse_var = false;
    this->remoteOperation_var = false;
    this->connectionId_var = -1;
    this->commId_var = -1;
    this->sourceId_var = NULL_PROCESS_ID;
    this->nextModuleIndex_var = -1;
    this->result_var = 0;
}

SIMCAN_Message_Base::SIMCAN_Message_Base(const SIMCAN_Message_Base& other) : ::cPacket(other)
{
    copy(other);
}

SIMCAN_Message_Base::~SIMCAN_Message_Base()
{
}

SIMCAN_Message_Base& SIMCAN_Message_Base::operator=(const SIMCAN_Message_Base& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void SIMCAN_Message_Base::copy(const SIMCAN_Message_Base& other)
{
    this->operation_var = other.operation_var;
    this->isResponse_var = other.isResponse_var;
    this->remoteOperation_var = other.remoteOperation_var;
    this->connectionId_var = other.connectionId_var;
    this->commId_var = other.commId_var;
    this->sourceId_var = other.sourceId_var;
    this->nextModuleIndex_var = other.nextModuleIndex_var;
    this->result_var = other.result_var;
}

void SIMCAN_Message_Base::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->operation_var);
    doPacking(b,this->isResponse_var);
    doPacking(b,this->remoteOperation_var);
    doPacking(b,this->connectionId_var);
    doPacking(b,this->commId_var);
    doPacking(b,this->sourceId_var);
    doPacking(b,this->nextModuleIndex_var);
    doPacking(b,this->result_var);
    // field trace is abstract -- please do packing in customized class
}

void SIMCAN_Message_Base::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->operation_var);
    doUnpacking(b,this->isResponse_var);
    doUnpacking(b,this->remoteOperation_var);
    doUnpacking(b,this->connectionId_var);
    doUnpacking(b,this->commId_var);
    doUnpacking(b,this->sourceId_var);
    doUnpacking(b,this->nextModuleIndex_var);
    doUnpacking(b,this->result_var);
    // field trace is abstract -- please do unpacking in customized class
}

unsigned int SIMCAN_Message_Base::getOperation() const
{
    return operation_var;
}

void SIMCAN_Message_Base::setOperation(unsigned int operation)
{
    this->operation_var = operation;
}

bool SIMCAN_Message_Base::getIsResponse() const
{
    return isResponse_var;
}

void SIMCAN_Message_Base::setIsResponse(bool isResponse)
{
    this->isResponse_var = isResponse;
}

bool SIMCAN_Message_Base::getRemoteOperation() const
{
    return remoteOperation_var;
}

void SIMCAN_Message_Base::setRemoteOperation(bool remoteOperation)
{
    this->remoteOperation_var = remoteOperation;
}

int SIMCAN_Message_Base::getConnectionId() const
{
    return connectionId_var;
}

void SIMCAN_Message_Base::setConnectionId(int connectionId)
{
    this->connectionId_var = connectionId;
}

int SIMCAN_Message_Base::getCommId() const
{
    return commId_var;
}

void SIMCAN_Message_Base::setCommId(int commId)
{
    this->commId_var = commId;
}

unsigned int SIMCAN_Message_Base::getSourceId() const
{
    return sourceId_var;
}

void SIMCAN_Message_Base::setSourceId(unsigned int sourceId)
{
    this->sourceId_var = sourceId;
}

unsigned int SIMCAN_Message_Base::getNextModuleIndex() const
{
    return nextModuleIndex_var;
}

void SIMCAN_Message_Base::setNextModuleIndex(unsigned int nextModuleIndex)
{
    this->nextModuleIndex_var = nextModuleIndex;
}

int SIMCAN_Message_Base::getResult() const
{
    return result_var;
}

void SIMCAN_Message_Base::setResult(int result)
{
    this->result_var = result;
}

class SIMCAN_MessageDescriptor : public cClassDescriptor
{
  public:
    SIMCAN_MessageDescriptor();
    virtual ~SIMCAN_MessageDescriptor();

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

Register_ClassDescriptor(SIMCAN_MessageDescriptor);

SIMCAN_MessageDescriptor::SIMCAN_MessageDescriptor() : cClassDescriptor("SIMCAN_Message", "cPacket")
{
}

SIMCAN_MessageDescriptor::~SIMCAN_MessageDescriptor()
{
}

bool SIMCAN_MessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SIMCAN_Message_Base *>(obj)!=NULL;
}

const char *SIMCAN_MessageDescriptor::getProperty(const char *propertyname) const
{
    if (!strcmp(propertyname,"customize")) return "true";
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SIMCAN_MessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 9+basedesc->getFieldCount(object) : 9;
}

unsigned int SIMCAN_MessageDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISCOMPOUND,
    };
    return (field>=0 && field<9) ? fieldTypeFlags[field] : 0;
}

const char *SIMCAN_MessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "operation",
        "isResponse",
        "remoteOperation",
        "connectionId",
        "commId",
        "sourceId",
        "nextModuleIndex",
        "result",
        "trace",
    };
    return (field>=0 && field<9) ? fieldNames[field] : NULL;
}

int SIMCAN_MessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='o' && strcmp(fieldName, "operation")==0) return base+0;
    if (fieldName[0]=='i' && strcmp(fieldName, "isResponse")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "remoteOperation")==0) return base+2;
    if (fieldName[0]=='c' && strcmp(fieldName, "connectionId")==0) return base+3;
    if (fieldName[0]=='c' && strcmp(fieldName, "commId")==0) return base+4;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceId")==0) return base+5;
    if (fieldName[0]=='n' && strcmp(fieldName, "nextModuleIndex")==0) return base+6;
    if (fieldName[0]=='r' && strcmp(fieldName, "result")==0) return base+7;
    if (fieldName[0]=='t' && strcmp(fieldName, "trace")==0) return base+8;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SIMCAN_MessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",
        "bool",
        "bool",
        "int",
        "int",
        "unsigned int",
        "unsigned int",
        "int",
        "TraceComponent",
    };
    return (field>=0 && field<9) ? fieldTypeStrings[field] : NULL;
}

const char *SIMCAN_MessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int SIMCAN_MessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_Message_Base *pp = (SIMCAN_Message_Base *)object; (void)pp;
    switch (field) {
        case 8: return pp->getTraceArraySize();
        default: return 0;
    }
}

std::string SIMCAN_MessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_Message_Base *pp = (SIMCAN_Message_Base *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getOperation());
        case 1: return bool2string(pp->getIsResponse());
        case 2: return bool2string(pp->getRemoteOperation());
        case 3: return long2string(pp->getConnectionId());
        case 4: return long2string(pp->getCommId());
        case 5: return ulong2string(pp->getSourceId());
        case 6: return ulong2string(pp->getNextModuleIndex());
        case 7: return long2string(pp->getResult());
        case 8: {std::stringstream out; out << pp->getTrace(i); return out.str();}
        default: return "";
    }
}

bool SIMCAN_MessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_Message_Base *pp = (SIMCAN_Message_Base *)object; (void)pp;
    switch (field) {
        case 0: pp->setOperation(string2ulong(value)); return true;
        case 1: pp->setIsResponse(string2bool(value)); return true;
        case 2: pp->setRemoteOperation(string2bool(value)); return true;
        case 3: pp->setConnectionId(string2long(value)); return true;
        case 4: pp->setCommId(string2long(value)); return true;
        case 5: pp->setSourceId(string2ulong(value)); return true;
        case 6: pp->setNextModuleIndex(string2ulong(value)); return true;
        case 7: pp->setResult(string2long(value)); return true;
        default: return false;
    }
}

const char *SIMCAN_MessageDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 8: return opp_typename(typeid(TraceComponent));
        default: return NULL;
    };
}

void *SIMCAN_MessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SIMCAN_Message_Base *pp = (SIMCAN_Message_Base *)object; (void)pp;
    switch (field) {
        case 8: return (void *)(&pp->getTrace(i)); break;
        default: return NULL;
    }
}



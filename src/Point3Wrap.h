#pragma once
#include "NodeV8.h"
#include "OCC.h"

// expose a point
class Point3Wrap : public node::ObjectWrap {
    virtual const gp_XYZ get() const = 0;

    double x() {
        return get().X();
    }
    double y() {
        return get().Y();
    }
    double z() {
        return get().Z();
    }
public:
    // Methods exposed to JavaScripts
    static v8::Handle<v8::Value> New(const Arguments& args);
    static void Init(v8::Handle<v8::Object> target);
    static Persistent<FunctionTemplate> constructor;

};



#define TEAROFF_INIT(ACCESSOR)   m_##ACCESSOR(*this)

template <class _ThisType, class Wrapper,class CLASS,const CLASS (_ThisType::*ACCESSOR)() const>
class Accessor : public Wrapper {
//    typedef Accessor<_ThisType, Wrapper,CLASS,ACCESSOR> THIS;
    _ThisType& m_parent;
public:
    Accessor(_ThisType& parent)
        :m_parent(parent) {
    }
    ~Accessor() {
    }
    virtual const CLASS get() const {
        return (m_parent.*ACCESSOR)();
    }
    static Handle<v8::Value> getter(Local<String> propName,const AccessorInfo &info) {
        HandleScope scope;
        if (info.This().IsEmpty()) {
            return scope.Close(Undefined());
        }
        if (info.This()->InternalFieldCount() == 0 ) {
            return scope.Close(Undefined());
        }
        _ThisType* pThis = node::ObjectWrap::Unwrap<_ThisType>(info.This());

        return scope.Close(NewInstance(*pThis));

    }
    static Handle<Value> NewInstance(_ThisType& parent) {
        HandleScope scope;
        Local<Object> instance = Wrapper::constructor->GetFunction()->NewInstance(0,0);
        Accessor* pThis = new Accessor(parent);
        pThis->Wrap(instance);
        return scope.Close(instance);
    }
};



#define TEAROFF_POINT(THISTYPE,ACCESSOR,WRAPPER,CLASS)                         \
    typedef Accessor<THISTYPE,WRAPPER,CLASS,&THISTYPE::ACCESSOR> t##ACCESSOR;  \
 



#define EXPOSE_TEAROFF(THISTYPE,ACCESSOR)                           \
    proto->SetAccessor(String::NewSymbol(#ACCESSOR), &t##ACCESSOR::getter,  0,Handle<v8::Value>(),DEFAULT,(PropertyAttribute)(ReadOnly|DontDelete))

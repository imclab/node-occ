#pragma once
#include "Shape.h"
#include "Mesh.h"

class Edge;
// a multi body shape
class Solid : public Shape 
{

protected:
    Solid() {};
    virtual ~Solid() { m_cacheMesh.Dispose(); };

 public:  
    virtual Local<Object>  Clone() ;
    virtual Base* Unwrap(v8::Local<v8::Object> obj) { return node::ObjectWrap::Unwrap<Solid>(obj); }

	Persistent<Object> m_cacheMesh;

	int numSolids();
	int numFaces();

	double volume();
	double area();
   
	Handle<Object>  mesh();

	Handle<Object> createMesh(double factor, double angle, bool qualityNormals = true);

	typedef enum BoolOpType {
		BOOL_FUSE,
		BOOL_CUT,
		BOOL_COMMON,
	} BoolOpType;

	int boolean(Solid *tool, BoolOpType op);
	int chamfer(const std::vector<Edge*>& edges, const std::vector<double>& distances);
	int fillet(const std::vector<Edge*>& edges, const std::vector<double>& distances);
	static Handle<v8::Value> fillet(const v8::Arguments& args);
	static Handle<v8::Value> chamfer(const v8::Arguments& args);

	// default mesh
	static Handle<v8::Value> _mesh(Local<String> property,const AccessorInfo &info);
	static Handle<v8::Value> createMesh(const v8::Arguments& args); // custom mesh
	
	static Handle<v8::Value> getEdges(const v8::Arguments& args);

	// Methods exposed to JavaScripts
	static void Init(Handle<Object> target);
	static Handle<v8::Value> NewInstance(const v8::Arguments& args);
	static Handle<Value>     NewInstance(TopoDS_Shape& shape);
	static Handle<v8::Value> New(const v8::Arguments& args);     

	static Persistent<FunctionTemplate> constructor;
};

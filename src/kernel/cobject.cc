/*
 * =====================================================================================
 *        Filename:  cobject.cc
 *     Description: CObject helper functions implementation.
 *         Created:  08/02/2006 02:08:14 PM CET
 *          Author:  jmisutka (), 
 *         Changes: Because anonymous namespaces in headers are in 99% not correct. 
 *         			(I knew that but I had mixed it with templates.)
 * =====================================================================================
 */

#include <ostream>
#include <iostream>
#include <sstream>
#include <string>

// debug
#include "debug.h"

//
#include "exceptions.h"
#include "cobject.h"
//#include "cobjectI.h"


// =====================================================================================

namespace pdfobjects
{

//
// General object functions
//
namespace utils {

using namespace std;
		
//
//
//
void
simpleObjToString (Object* obj,string& str)
{
 	assert (NULL != obj);
	printDbg (0,"simpleObjToString(" << (unsigned int)obj << ")");
	printDbg (0,"\tobjType = " << obj->getTypeName() );

	ostringstream oss;

	switch (obj->getType()) 
	{
	case objBool:
		oss << ((obj->getBool()) ? "true" : "false");
		break;

	case objInt:
		oss << obj->getInt();
		break;

	case objReal:
		oss << obj->getReal ();
		break;

	case objString:
		oss << "("  << obj->getString()->getCString() << ")";
		break;

	case objName:
		oss << "/" << obj->getName();
		break;

	case objNull:
		oss << "null";
		break;

	case objRef:
		oss << obj->getRefNum() << " " << obj->getRefGen() << " R";
		break;

	default:
		assert (!"Bad object passed to simpleObjToString.");
		throw ObjBadTypeE(); 
		break;
	}

	// convert oss to string
	str = oss.str ();
}


namespace {

	inline void objToString (Object* obj, string& str)
	{
		switch (obj->getType())
		{
			case objArray:
			case objDict:
			case objStream:
					complexObjToString (obj,str);
					break;
			default:
					simpleObjToString (obj,str);
					break;
		}
	}
}

//
//
//
void
complexObjToString (Object* /*obj*/, string& /*str*/)
{
/* 	assert (NULL != obj);
	printDbg (0,"objToString(" << (unsigned int)obj << ")");
	printDbg (0,"\tobjType = " << obj->getTypeName() );

	ostringstream oss;
	Object o;
	int i;

	switch (obj->getType()) 
	{
	
	case objArray:
		oss << "[";
		for (i = 0; i < obj->arrayGetLength(); ++i) 
		{
			if (i > 0)
				oss << " ";
			obj->arrayGetNF (i,&o);
			string tmp;
			objToString (&o,tmp);
			oss << tmp;
		o.free();
		}
		oss << "]";
		break;

	case objDict:
		oss << "<<";
		for (i = 0; i <obj-> dictGetLength(); ++i) 
		{
			oss << " /" << obj->dictGetKey(i) << " ";
			obj->dictGetValNF(i, &o);
			string tmp;
			objToString (&o,tmp);
			oss << tmp;
			o.free();
		}
		oss << " >>";
		break;

	case objStream:
		oss << "<stream>";
		break;

	default:
		assert (false);	
		break;
	}

	// convert oss to string
	str = oss.str ();
*/}


//
//
//
template <typename Storage>
void getAllXpdfObjects (Object& obj, Storage& /*store*/) 
{
	switch (obj.getType())
	{
		case objArray:
		{	//int size = obj.arrayGetLength ();
			//for (int i = 0; i < size; i++)
			{
				//store.push_back (obj.getArray()->elems[i]);
			}
		}	return;	
			break;
		
		case objDict:
		{	//int size = obj.dictGetLength ();
			//for (int i = 0; i < size; i++)
			{
				//store.push_back (obj.getDict()->entries[i].val);
			}
		}	return;	
			break;

		case objStream:
			assert (!"not implemented yet...");
			break;

		case objRef:
			assert (!"not implemented yet...");
			break;

		default:	// Null, Bool, Int, Real, String, Name
			assert (!"Bad object passed to simpleObjToString.");
			throw ObjBadTypeE ();
			break;
	}	
}
//
// We have to explicitely instantiate functions, because we have included them in header cobject.h
//
template void getAllXpdfObjects<std::vector<Object*> > (Object& obj, std::vector<Object*>& /*store*/ );


//
// Purposeful only for Dict and Stream
//
Object*
getObjectAtPos (Object& obj, const std::string& /*name*/)
{
	assert ((objDict == obj.getType()) || (objStream == obj.getType()) );

	switch (obj.getType ())
	{
		case objDict:
			//
			// have to dig into Dict structure, because
			// default functions just copy objects !!!
			//
			break;

		case objStream:
			//
			// have to dig into Dict structure, because
			// default functions just copy objects !!!
			//
			break;

		default:
			assert (!"bad type");
			break;
	}

	return NULL;
}

Object*
getObjectAtPos (Object& obj, const unsigned int /*pos*/)
{
	assert (objArray == obj.getType());
	//
	// have to dig into Dict structure, because
	// default functions just copy objects !!!
	//
	return NULL;
}


//
//
//
void
removeObjectAtPos (Object& obj, const std::string& /*name*/)
{
	assert ((objDict == obj.getType()) || (objStream == obj.getType()) );

	// Get the dictionary
	//Dict* o = (objDict == obj.getType()) ? obj.getDict () : obj.streamGetDict ();
	
	//
	// Dig into the structure
	//

}
void
removeObjectAtPos (Object& /*obj*/, const unsigned int /*pos*/)
{
}


		
//
//
//
void
objectFree (Object* obj)
{
	printDbg (1,"objectFree(" << (unsigned int)obj << ")");
	std::string str;
	objToString (obj,str); 
	printDbg (1,"\t..." << str);
	
	switch (obj->getType()) 
	{
		case objString:
			//delete obj->string;
			//obj->string = NULL;
			break;
		
		case objName:
			//gfree(name);
			break;

		case objArray:
			//
			// A bit of a hack. That means do not free any nested
			// objects, we have already done this.
			//
			//obj->getArray()->length = 0;
			//delete obj->getArray();
			//obj->array = NULL;
			break;
		
		case objDict:
			//
			// Another xpdf hack. That means do not free any nested
			// objects, we have already done this.
			//
			//for (i = 0; i < obj->getDict()->length; ++i) 
			{
			//	gfree(obj->getDict()->entries[i].key);
			//	obj->getDict()->entries[i].key = NULL;
			}	
			//obj->getDict()->length = 0;
			//delete obj->getDict();
			//obj->dict = NULL;
			break;
		
		case objStream:
			//
			// really Everything??
			//
			//for (i = 0; i < obj->getStream()->getDict()->length; ++i) 
			{
			//	gfree(obj->getStream()getDict()->entries[i].key);
			//	obj->getStream()->getDict()->entries[i].key = NULL;
			}
			//obj->getStream()->getDict()->length = 0;
			//delete obj->getStream()->getDict();
			break;
		
		default:
			break;
		}

	// delete main obj
	delete obj;
}



//
// Delete processor, it needs to be here because of the CPdf usage
//
template<typename Check>
void
ObjectDeleteProcessor<Check>::operator() (Object* obj)
{
		IProperty* ip = pdf->getExistingProperty (obj);
		
		//
		// There is no IProperty associated with this obj so free it
		// If complex, then it will be freed recursively
		// 
		if (NULL == ip)
		{
			// make a check if any
			check (obj);	
			// free it
			obj->free ();
		
		}//else
		{
			ip->release ();
		}
}
//
// Explicit instantiating
//
template class ObjectDeleteProcessor<utils::CheckDirectObject>;
template class ObjectDeleteProcessor<utils::NoCheck>;


//
//
//
inline void
getSimpleValueFromString (const std::string& str, bool& val)
{
  if (str == "true")
	val = true;
  else if (str == "false")
	val = false;
  else
	throw ObjBadValueE ();
}

void
getSimpleValueFromString (const std::string& str, int& val)
{
	std::stringstream ss (str);
	ss >> val;
}

inline void
getSimpleValueFromString (const std::string& str, double& val)
{
	val = atof (str.c_str());
}

inline void
getSimpleValueFromString (const std::string& str, std::string& val)
{
	val = str;
}

inline void
getSimpleValueFromString (const std::string& str, IndiRef& val)
{
	std::stringstream ss (str);
	ss >> val.num;
	ss >> val.gen;
}



} /* namespace utils */
} /* namespace pdfobjects */

/** @file
 QObject wrapper around CDict<br>
 Export some functions to scripting.<br>
 See doc/user/scripting.xml or kernel documentation for more informations about these functions
 @author Martin Petricek
*/

#include "qsdict.h"
#include "qsimporter.h"
#include <cobject.h>
#include "pdfutil.h"
#include <qstringlist.h>

namespace gui {

using namespace std;
using namespace pdfobjects;

/**
 Construct wrapper with given CDict
 @param _dict CDict
 @param _base scripting base
 */
QSDict::QSDict(boost::shared_ptr<CDict> _dict,Base *_base) : QSIProperty (_dict,"Dict",_base) {
}

/**
 Copy constructor
 @param source Source object to copy
*/
QSDict::QSDict(QSDict &source) : QSIProperty (source.obj,"Dict",source.base) {
}

/** destructor */
QSDict::~QSDict() {
}

/** get CDict held inside this class. Not exposed to scripting */
boost::shared_ptr<CDict> QSDict::get() {
 return boost::dynamic_pointer_cast<CDict>(obj);
}

/** call CDict::getPropertyCount() */
int QSDict::count() {
 CDict *dict=dynamic_cast<CDict*>(obj.get());
 return dict->getPropertyCount();
}

/**
 Get property by its name
 \see CDict::getProperty
 @param name Name of property
 @return the property, or NULL if not found
*/
QSCObject* QSDict::property(const QString &name) {
 try {
  CDict *dict=dynamic_cast<CDict*>(obj.get());
  boost::shared_ptr<IProperty> property=dict->getProperty(name);
  return QSImporter::createQSObject(property,base);
 } catch (...) { 
  //Some error, probably the property does not exist
  return NULL;
 }
}

/**
 Check if given property exists in dictionary
 @param name Name of property
 @return True, if property exists in dictionary, false if not
*/
bool QSDict::exist(const QString &name) {
 CDict *dict=dynamic_cast<CDict*>(obj.get());
 return dict->containsProperty(name);;
}

/**
 call CDict::getProperty(name),
 if property does not exist,
 property with defValue in it is added to dict and returned
 @param name Name of property
 @param defValue default value for property
 @return Properry from dictionary
*/
QSCObject* QSDict::propertyDef(const QString &name,int defValue) {
 QSCObject* ret=property(name);
 if (ret) return ret; //Property exists -> return it
 //Property does not exist -> add it
 add(name,defValue);
 //Now return it
 return property(name);
}

/**
 call CDict::getProperty(name),
 if property does not exist,
 property with defValue in it is added to dict and returned
 @param name Name of property
 @param defValue default value for property
 @return Properry from dictionary
*/
QSCObject* QSDict::propertyDef(const QString &name,QString defValue) {
 QSCObject* ret=property(name);
 if (ret) return ret; //Property exists -> return it
 //Property does not exist -> add it
 add(name,defValue);
 //Now return it
 return property(name);
}

/**
 Delete property from dictionary
 \see CDict::delProperty
 @param name Property name
*/
void QSDict::delProperty(const QString &name) {
 CDict *dict=dynamic_cast<CDict*>(obj.get());
 string pName=name;
 dict->delProperty(pName);
}

/**
 Add property to dictionary
 \see CDict::addProperty
 @param name Property name
 @param ip Property to add
*/
void QSDict::add(const QString &name,QSIProperty *ip) {
 CDict *dict=dynamic_cast<CDict*>(obj.get());
 string pName=name;
 dict->addProperty(pName,*(ip->get().get()));
}

/** \copydoc add(const QString&,QSIProperty*) */
void QSDict::add(const QString &name,QObject *ip) {
 //QSA-bugfix variant of this method
 QSIProperty *ipr=dynamic_cast<QSIProperty*>(ip);
 if (ipr) add(name,ipr);
}

/**
 Add string to dictionary as property
 \see CDict::addProperty
 @param name Property name
 @param ip string to add
*/
void QSDict::add(const QString &name,const QString &ip) {
 CDict *dict=dynamic_cast<CDict*>(obj.get());
 string pName=name;
 CString property(ip);
 dict->addProperty(pName,property);
}

/**
 Add integer to dictionary as property
 \see CDict::addProperty
 @param name Property name
 @param ip integer to add
*/
void QSDict::add(const QString &name,int ip) {
 CDict *dict=dynamic_cast<CDict*>(obj.get());
 string pName=name;
 CInt property(ip);
 dict->addProperty(pName,property);
}

/**
 Return text representation of dictionary
 \see CDict::getStringRepresentation
 @return string representation
*/
QString QSDict::getText() {
 CDict *dict=dynamic_cast<CDict*>(obj.get());
 string text;
 dict->getStringRepresentation(text);
 return text;
}

/**
 Return list of all property names in this dictionary
 \see CDict::getAllPropertyNames
 @return List of all property names
*/
QStringList QSDict::propertyNames() {
 CDict *dict=dynamic_cast<CDict*>(obj.get());
 QStringList names;
 vector<string> list;
 dict->getAllPropertyNames(list);
 vector<string>::iterator it;
 for( it=list.begin();it!=list.end();++it) { // for each property
  names+=*it;
 }
 return names;
}

/**
 recursive CDict/CArray getProperty(...)
 Will take the name as slash-separated list of childs to traverse to get to target property.
 References on the way are automatically dereferenced
 @param name Path to property
 @return specified property
*/
QSCObject* QSDict::child(const QString &name) {
 try {
  boost::shared_ptr<CDict> dict=boost::dynamic_pointer_cast<CDict>(obj);
  boost::shared_ptr<IProperty> property=util::recursiveProperty(dict,name);
  return QSImporter::createQSObject(property,base);
 } catch (...) { 
  //Some error, probably the property does not exist
  return NULL;
 }
}

} // namespace gui

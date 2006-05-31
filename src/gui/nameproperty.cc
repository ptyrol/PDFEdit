/** @file
 NameProperty - class for widget containing one editable property of type "Name"<br>
 Basically, CName is CString, but we might want differentiation in case we'll handle names specially in future<br>
 Used as one item in property editor
 @author Martin Petricek
*/

#include "nameproperty.h"

namespace gui {

using namespace std;

/** Default constructor of property item
 @param parent parent Property Editor containing this control
 @param _name name of this property
 @param _flags flags of this property items (default 0)
 */
NameProperty::NameProperty(const QString &_name, QWidget *parent/*=0*/, PropertyFlags _flags/*=defaultPropertyMode*/)
 : StringProperty(_name,parent,_flags) {
 //just use StringProperty
}

/** default destructor */
NameProperty::~NameProperty() {
}

} // namespace gui

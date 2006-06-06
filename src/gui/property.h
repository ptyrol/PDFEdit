#ifndef __PROPERTY_H__
#define __PROPERTY_H__

#include <qwidget.h>
#include <modecontroller.h>

class QString;
class QLabel;
namespace pdfobjects {
 class IProperty;
}

namespace gui {

using namespace pdfobjects;

/** Property flags (property mode - hidden, advanced ...) */
typedef PropertyMode PropertyFlags;

/** Default mode to be chosen if omitted in constructor */
const PropertyMode defaultPropertyMode=mdNormal;

/** Ancestor of all property items */
class Property : public QWidget {
Q_OBJECT
public:
 Property(const QString &_name=0,QWidget *parent=0, PropertyFlags _flags=defaultPropertyMode);
 void initLabel(QLabel *widget);
 virtual ~Property();
 QString getName();
 PropertyFlags getFlags();
 bool isHidden();
 static QString modeName(PropertyFlags flag);
 void setFlags(PropertyFlags flag);
 void override(bool showHidden,bool editReadOnly);
 void modifyColor(QWidget* widget);
 virtual void applyHidden(bool hideThis);
 //Abstract functions
 /**
  Disable this control
  @param disabled True to disable, false to enable
 */
 virtual void setDisabled(bool disabled) = 0;
 /** 
  Apply this value of "read only" to the property
  @param _readonly True for read-only, false for read-write
 */
 virtual void applyReadOnly(bool _readonly) = 0;
 /**
  write internal value to given PDF object
  @param pdfObject Objet to write to
 */
 virtual void setValue(IProperty *pdfObject) = 0;
 /**
  read internal value from given PDF object
  @param pdfObject Objet to read from
 */
 virtual void readValue(IProperty *pdfObject) = 0;
 /** 
  Check if edited property is currently valid.
  @return true if valid, false if not
 */
 virtual bool isValid()=0;
signals:
 /**
  Signal emitted when property is edited.
  Send property pointer with the signal
  @param prop Pointer to changed property item
 */
 void propertyChanged(Property *prop);
 /** 
  Send informational message when mouse cursor enters/leaves the property
  The message is sent on enter and QString::null on leave.
  @param message Message
 */
 void infoText(const QString &message);
 /** 
  Send warning message on some events (only once when the event occurs)
  @param message Message
 */
 void warnText(const QString &message);
protected:
 void emitChanged();
 virtual void enterEvent(QEvent *e);
 virtual void leaveEvent(QEvent *e);
protected:
 /** Name of this property */
 QString name;
 /** flags of the property (from PDF object) */
 PropertyFlags flags;
 /** was the property edited since last readValue or setValue? */
 bool changed;
 /** Label of property */
 QWidget* propertyLabel;
 /** is this property really readonly? result after applying overrides */
 bool effectiveReadonly;
private:
 /** is this property readonly? Set from flags */
 bool readonly;
 /** is this property hidden? Set from flags */
 bool hidden;
};

} // namespace gui

#endif

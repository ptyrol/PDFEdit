#ifndef __TREEITEMABSTRACT_H__
#define __TREEITEMABSTRACT_H__

#include <qlistview.h>
#include <qdict.h>
#include <qmap.h>

class QStringList;
class QString;

namespace gui {

class QSCObject;
class TreeData;
class TreeWindow;
class BaseCore;

/**
 Type of TreeItemAbstract's child.<br>
 The type is arbitrary integer and must be unique only across single type
 (different types must have different values).<br>
 Two distict TreeItemAbstract descendants can have collisions in types
 (same value meaning different type)<br>
 Types of childs from different items are never mixed or compared together
*/
typedef int ChildType;

/**
 Common ancestor of all tree Items,
 with functions to support automatic loading and reloading of tree,
 or its parts (subtrees)
 \brief Abstract tree item class
*/
class TreeItemAbstract : public QListViewItem {
public:
 void setSelect(bool selected);
 void unSelect(QListView *tree);
 QString path();
 TreeItemAbstract(const QString &itemName,TreeData *_data,QListView *parent,QListViewItem *after=NULL);
 TreeItemAbstract(const QString &itemName,TreeData *_data,QListViewItem *parent,QListViewItem *after=NULL);
 QString name();
 void setName(const QString &newNameId);
 virtual ~TreeItemAbstract();
 void reload(bool reloadThis=true,bool forceReload=false);
 virtual void deleteChild(const QString &name);
 virtual void deleteChild(QListViewItem *target);
 void eraseItems();
 void moveAllChildsFrom(TreeItemAbstract* src);
 QListViewItem* child(const QString &name);
 virtual QSCObject* getQSObject(BaseCore *_base);
 virtual void setOpen(bool open);
 virtual bool deepReload(const QString &childName,QListViewItem *oldItem);

 //Abstract functions
 /** 
  Validate child, given its name and reference to old child
  Usually it is valid, since item with same name refer to same subitem (key-based items),
  but for value-based items (array) same keys can correspond to different values after reloading.
  In such cases, false should be returned.
  Also, similar problems can happen when switching revisions (with almost all types)
  <br><br>

  Note: false negative it not a problem (only unnecessary reloading),
        while false positive mean the tree is invalid/outdated

  @param name name of (old and new) treeitem
  @param oldChild reference to old child tree item
  @return True, if the old child is pointing to same item as item "name", false, if the child item "name" no longer point to same item as oldChild.
 */
 virtual bool validChild(const QString &name,QListViewItem *oldChild)=0;
 /** 
  Create and return object for scripting representing this tree  item
  @return object for scripting
 */
 virtual QSCObject* getQSObject()=0;
 /**
  Create one subchild
  @param name Name of child (the same as in list returned from getChildNames)
  @param typ Type of TreeItemAbstract's child. (see ChildType)
  @param after QListViewItem after which will be this item added. If unspecified (or  NULL), it is added before any other items
  @return Pointer to subchild. If returned NULL, the subchild can not be created
  \see getChildNames
 */
 virtual TreeItemAbstract* createChild(const QString &name,ChildType typ,QListViewItem *after=NULL)=0;
 /**
  Get list of all item's childs names
  @return list of child names. Names in the list must be unique and the list may be empty.
 */
 virtual QStringList getChildNames()=0;
 /** Reload contents of itself and only itself (excluding any children) */
 virtual void reloadSelf()=0;
 /**
  Remove itself from document (including any children)
  Do not remove the treeitem itself in this method -> this may lead to crash!
  If desired, you may call reload() on parent to get rid of the item correctly
 */
 virtual void remove()=0;
 /**
  Get type of this items child, given its name.
  @param name Name of child item
  @return type of specified child
 */
 virtual ChildType getChildType(const QString &name)=0;
 /**
  Return true, if the item have at least one child
  @return Presence of at least one child
 */
 virtual bool haveChild()=0;
private:
 void initAbs();
protected:
 /** Name of this item */
 QString nameId;
 /** Mapping of all child names to child items for this tree item */
 QDict<QListViewItem> items;
 /** Mapping of all child names to types of child items for this tree item */
 QMap<QString,ChildType> types;
 /** Data from treewindow containing reference to it and other needed data */
 TreeData *data;
private:
 /** False, if childs of this items are yet unknown and to be parsed/added, true if already parsed */
 bool parsed;
 /** root window (for delete notification) */
 TreeWindow* rootWindow;
};

} // namespace gui

#endif

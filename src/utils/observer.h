// vim:tabstop=4:shiftwidth=4:noexpandtab:textwidth=80
/*
 * $RCSfile$
 *
 * $Log$
 * Revision 1.5  2006/04/12 17:58:28  hockm0bm
 * BasicChangeContext changed to BasicChangeContextType
 *
 * Revision 1.4  2006/04/09 21:32:03  misuj1am
 *
 *
 * -- changed a bit
 * 	-- virtual destructor
 * 	-- pointer to context changed to shared_ptr
 *
 * Revision 1.3  2006/04/02 08:09:59  hockm0bm
 * new observer design
 * * IObserver - interface for observer implementation (template class)
 * * IObserverHandler - interface for value keepers which wants to be
 *         registration target of observers (template class)
 * * IChangeContext - interface for change context for notifier (template class)
 *         - BasicCahngeContext - implementation of basic context with original
 *           value of changed one
 *
 *
 */

#ifndef _OBSERVER_H
#define _OBSERVER_H

#include <boost/shared_ptr.hpp>

//=============================================================================
namespace observer
{

/** Interface (pure abstract class) for change context.
 *
 * This is base class for all change contexts. It contains just information
 * about supported contexts (in enum) and type of context. 
 * <br>
 * User of this change context should check context type at first and than cast
 * to specific context subclass.
 */
template<typename T> class IChangeContext
{
public:
	/** Supported context types.
	 */
	enum ChangeContextType {BasicChangeContextType};
	
	/** Returns context type.
	 *
	 * @return context type.
	 */
	virtual ChangeContextType getType()const =0;
};

/** Basic change context template class.
 * 
 * Simplest subclass from IChangeContext. It holds previous value of changed
 * one. This value is wrapped by smart pointer to keep clear instancion policy.
 */
template<typename T> class BasicChangeContext:public IChangeContext<T>
{
	/** Original value. */
	boost::shared_ptr<T> originalValue;
public:
	/** Constructor with original value.
	 * @param origVal Original value used for originalValue initialization.
	 */
	BasicChangeContext(boost::shared_ptr<T> origVal):originalValue(origVal){};

	/** Destructor.
	 * Just to mark all destructors in subtypes as virtual.
	 */
	virtual ~BasicChangeContext(){};

	/** Returns type of context.
	 *
	 * @return BasicChangeContext value.
	 */
	typename IChangeContext<T>::ChangeContextType getType()const
	{
		return IChangeContext<T>::BasicChangeContextType;
	}

	/** Returns original value.
	 *
	 * @return Orignal value wrapped by smart pointer.
	 */
	virtual boost::shared_ptr<T> getOriginalValue()const
	{
		return originalValue;
	}
};


/** Observer interface (pure abstract class).
 *
 * Observer is mechanism how to be informed that somethig has changed. 
 * Implementator of this interface produces handler for this change event.
 * <br>
 * This should be used in following way:
 * <ul>
 * <li>value keeper which wants to enable observers has to implement
 * IObserverHandler interface which enables to register and unregister 
 * observers. It guaranties it calls notify on each registered observer.
 * <li>implementator of class is responsible for notify method implementation
 * which handles situation.
 * <li>observer must be registered on target value keeper.
 * </ul>
 *
 * Interface is template to enable different value types to be used (type 
 * parameter stands for value type to be managed - informed about change). 
 * This enables only one type to be handled by one implementator (with all 
 * subtypes).
 * <br>
 * Value change handling is done in notify method (see for more details). This
 * method is called after value has been changed. Only direct change of value is
 * announced.
 * <br>
 * Each observer implementation has its priority which is used be value keeper
 * to determine order in which to notify obsevers.
 */
template<typename T> class IObserver
{
public:
	/** Type for priority.
	 */
	typedef int priority_t;
	
	/** Notify method.
	 * @param newValue New value of changed value or its part.
	 * @param context Value change context.
	 * 
	 * Each time value keeper, which implements IObserverHandler, changes 
	 * value (or its part), all registered observers are announced about it
	 * by this method.
	 * <br>
	 * newValue stands for new value or its part (if value is complex). 
	 * <br>
	 * contex contains additional information about change. It depends on
	 * value keeper which information is provided (if any). It may be NULL
	 * which means that no information is provided. Method implementator
	 * should check context type (using getType() method TODO link).
	 * Accoring this type cast to correct IChangeContext subclass and use
	 * information.
	 * <p>
	 * <b>Example</b>:<br>
	 * Lets say that T is IProperty. If observer is registered on simple
	 * type, newValue contains new value of this simple one. If it is
	 * registered on complex type, it contains changed value (item) inside 
	 * this complex type (value keeper part is changed).
	 */
	virtual void notify (boost::shared_ptr<T> newValue, boost::shared_ptr<const IChangeContext<T> > context) const = 0;

	/** Returns priority of obsever.
	 *
	 * Lower number means higher priority.
	 * @return Observer priority value.
	 */
	virtual priority_t getPriority()const =0;

	/**
	 * Virtual destructor.
	 */
	virtual ~IObserver () {};
};

/** Interface for notifiers.
 *
 * Each value keeper which whants to manage observers should implement this
 * interface. 
 * 
 */
template<typename T> class IChangeContextHandler
{
public:
	/** Registers new observer.
	 * @param observer Observer to register (if NULL, nothing is registered).
	 *
	 * Adds given observer to the observer list which is (one by one) notified
	 * about change when it occures.
	 * <br>
	 * Implementator should guarant that observers with higher priority (lower
	 * priority numbers) should be noified sooner than lower priority ones.
	 * Observers with same priotities should be notified in same ordered
	 * accordind registration order.
	 * <br>
	 * Multiple calling with same observer should be ignored.
	 */
	virtual void registerObserver(const IObserver<T> * observer) =0;

	/** Unregisters given observer.
	 * @param observer Observer to unregister.
	 *
	 * Given one is not used in next change.
	 */
	virtual void unregisterObserver(const IObserver<T> * observer) =0;
};


} // namespace observer




#endif  // _OBSERVER_H

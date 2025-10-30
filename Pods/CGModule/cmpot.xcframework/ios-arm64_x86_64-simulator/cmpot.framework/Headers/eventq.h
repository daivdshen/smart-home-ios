/**
 *
 * @file eventq.h
 *
 * Include classes, structures and typedef for csp event queue.
 * An event queue is a sort of list, used in ViP Devices
 * to enqueue/dequeue csp messages, handle fsm machine and other stuff
 *
 * @author Donati Giacomo
 *
 ***************************************************************************/
#ifndef EVENTQ_H
#define EVENTQ_H

#include <list>
#include <stdint.h>
#include <cmpot/common.h>

/// Bitmask that contain family id
#define EV_ID_FAMILY_MASK    0xff00
/// Return the family of the passed event
#define EV_ID_FAMILY(evId)  ((evId) & EV_ID_FAMILY_MASK)

/// Event queues family events
#define EV_FAMILY_EVQ   0x0000
/// Empty queue event (cfr. evq_next())
/// - data: empty.
/// - distruttore: NULL.
#define EV_EVQ_EMPTY    ((EventqType) (EV_FAMILY_EVQ | 0x0000))

namespace Comelit {
namespace MPot    {
namespace Eventqueue {

using std::list;

typedef uint16_t EventqType;        ///< Event Type
typedef void*    EventqDataPtr;   ///< Data related to stored message

/// Define of the destructor that is called after remove an item from the list
typedef std::function<void (struct EventqNode *ev)> EventqDestructor;

/**
 * defines a node in an event queue list
 */
class EventqNode
{
public:
	EventqNode() : type(EV_EVQ_EMPTY), data(NULL), destr(NULL), nodeId(0) {}
	EventqNode(EventqType evType, EventqDataPtr evData, EventqDestructor evDestr, int nodeId);
	bool operator == (const EventqNode& n) const { return nodeId == n.nodeId; }
	bool operator != (const EventqNode& n) const { return !operator==(n); }

	EventqType       type;
	EventqDataPtr    data;
	EventqDestructor destr;
	unsigned int     nodeId;
};

/**
 * manage the event queue list
 */
class Eventq
{
public:
	Eventq();
	
	/**
	 * check if the event queue list is empty
	 * @return true - list is empty, false otherwise
	 */
	bool isEmpty() { return eventQueueList.empty(); }
	
	/**
	 * clear delete all elements in the list
	 */
	void clear();
	
	/**
	 * add an event queue item to the tail's list
	 * @param type  event type
	 * @param data  data provided by the message
	 * @param destr an eventually destructor that will be called when remove
	 *              the item from list
	 * @return      0 if the item has been added successfully
	 */
	int add(EventqType type, EventqDataPtr data, EventqDestructor destr);
	
	/**
	 * restart the iterator to the begin (head) of the list
	 * and fill the passed eventq node with the iterator's value
	 * @param node filled with the head eventq node
	 * @return false id the list is empty, true otherwise
	 */
	bool head(EventqNode& node);
	
	/**
	 * move to the iterator to the next element of the list.
	 * @param node pointer where to store the next node
	 * @return false if list is empty or  iterator reach the end of the list
	 *         true otherwise
	 */
	bool next(EventqNode& node);
	
	/**
	 * remove the passed node from the list
	 * @param node the node to be removed from the list
	 * @return false if list is empty, true otherwise
	 */
	bool remove(EventqNode& node);
	
	/**
	 * remove the head of the list
	 * @return false if the list is empty, true otherwise
	 */
	bool removeHead();

private:
	unsigned int idCounter;
	list<EventqNode>::iterator it;
	list<EventqNode> eventQueueList; ///< Event queue list
};

} // Eventq NS
} // MPot NS
} // Comelit NS

#endif // EVENTQ_H

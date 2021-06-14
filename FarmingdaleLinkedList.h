#include <deque>
#include "statusCode.h"
#include <ostream>
// This is the TEMPLATED_STACK code, for the templated version of Stack
namespace farmingdale {
	// Note: namespaces are additive, so you can add new elements to this namespace in other files or later
	template <class CL> struct llNode;
	template <class CL> class linkedList;
}


// a node of my ll
template <class CL> struct farmingdale::llNode {
	CL data;
	llNode<CL>* next;
};

// Each method has a notation next to it indicating the Module in which you will program it.
// M7 = we will implement in Module 7, M8 = implement in Module 8
template <class CL> class farmingdale::linkedList {
private:
	llNode<CL>* head; // first node
	llNode<CL>* tail; // last node
public:
	linkedList(); // M7 // default ctor

	// Rule of threes
	linkedList(const linkedList<CL>&); // M8 // copy ctor
	~linkedList(); // M7 // dtor
	// not a reference paramter because we use copy/swap
	const linkedList<CL>& operator=(linkedList<CL>); //M8 // assignment operator

	// accessors
	inline bool isEmpty() const {
		//can check if head or tail is 0
		return(0 == head);
	}; // M7 // write inline
	statusCode getHead(CL&) const; // M7
	statusCode getTail(CL&) const; // M7
	bool operator==(const linkedList<CL>&) const; // M7 (I write in the videos) // base comparison operator
	bool operator==(const std::deque<CL>&) const; //M7 (I write in the videos)
														   // print a linkedList to a stream (e.g., std::cout)
	void printToStream(std::ostream& outputStream) const; // M7 (I write in the videos)
	llNode<CL>* search(CL) const; // M8
	template <class CL> inline bool contains(CL findMe) const { // M8 // write inline using search
		//call search and see if it returns null
		return (search(findMe) != 0);
	}
	llNode<CL>* findAtPostion(int) const; // M8 // 0 = first item, 1 == second ....

	// mutators
	statusCode insertAtHead(CL); // M7
	statusCode removeAtHead(CL&); // M7
	statusCode insertAtTail(CL); // M7
	statusCode insertAfter(llNode<CL>*, CL); // M8
	statusCode removeAtTail(CL&); // M8
	statusCode remove(llNode<CL>*); // M8  
	template <class CL> inline farmingdale::statusCode remove(CL removeMe) {
		//first call search(string)
		llNode<CL>* removeNode = search(removeMe);
		//if search succeeds, call remove(node*)
		if (removeNode == 0) {
			return FAILURE;
		}
		return(remove(removeNode));
	}



};
// Write the following here as outside inline functions: (all M7)
// operator!= linkedList,linkedList
template <class CL> inline bool operator!=(const farmingdale::linkedList<CL>& lhs, const farmingdale::linkedList<CL>& rhs) {
	return (!(lhs == rhs));
}
// operator!= linkedList,std::deque
template <class CL> inline bool operator!=(const farmingdale::linkedList<CL>& lhs, const std::deque<CL>& rhs) {
	return (!(lhs == rhs));
}
// operator!= std::deque,linkedList
template <class CL> inline bool operator!=(const std::deque<CL>& lhs, const farmingdale::linkedList<CL>& rhs) {
	return (!(rhs == lhs));
}
// operator== std::deque,linkedList
template <class CL> inline bool operator==(const std::deque<CL>& lhs, const farmingdale::linkedList<CL>& rhs) {
	return (rhs == lhs);
}

template <class CL> inline std::ostream& operator<<(std::ostream& theStream, const farmingdale::linkedList<CL>& theLinkedList) {
	theLinkedList.printToStream(theStream);
	return theStream;
}

//default constructor

template <class CL> farmingdale::linkedList<CL>::linkedList()
	:
	head(0),
	tail(0)
{}

template <class CL> farmingdale::statusCode farmingdale::linkedList<CL>::getHead(CL& returnedValue) const {
	//if list is empty, return FAILURE
	if (isEmpty()) {
		return farmingdale::FAILURE;
	}

	//else put head's data value into returnedValue
	returnedValue = head->data;
	//return SUCCESS
	return farmingdale::SUCCESS;
}

template <class CL> farmingdale::statusCode farmingdale::linkedList<CL>::getTail(CL& returnedValue) const {
	//if list is empty, return FAILURE
	if (isEmpty()) {
		return farmingdale::FAILURE;
	}

	//else put tail's data value into returnedValue
	returnedValue = tail->data;
	//return SUCCESS
	return farmingdale::SUCCESS;
}

template <class CL> farmingdale::statusCode farmingdale::linkedList<CL>::insertAtHead(CL addMe) {
	//allocate memory for new node, return FAILURE if that fails
	llNode<CL>* temp;
	try {
		temp = new llNode<CL>;
	}
	catch (std::bad_alloc) {
		return farmingdale::FAILURE;
	}
	//assign addMe to the data field of the new node
	temp->data = addMe;
	//always set next field of new node to head
	temp->next = head;
	//make head point to new node
	head = temp;
	//if tail is null, set tail to point to new node
	if (0 == tail) {
		tail = temp;
	}
	//return SUCCESS
	return farmingdale::SUCCESS;
}

template <class CL> farmingdale::statusCode farmingdale::linkedList<CL>::insertAtTail(CL addMe) {
	//allocate new node and return FAILURE if it fails
	llNode<CL>* temp;
	try {
		temp = new llNode<CL>;
	}
	catch (std::bad_alloc) {
		return farmingdale::FAILURE;
	}
	//set data field of new node to addMe
	temp->data = addMe;
	//set next field of new node to 0
	temp->next = 0;
	//if tail is not NULL, set tail's "next" field to point to the new node
	if (0 != tail) {
		tail->next = temp;
	}

	//set tail to point to the new node
	tail = temp;

	if (0 == head) {
		//if head is null, head point to new node
		head = temp;
	}

	//return SUCCESS
	return farmingdale::SUCCESS;
}

template <class CL> farmingdale::statusCode farmingdale::linkedList<CL>::removeAtHead(CL& removedVal) {
	//return FAILURE if the list is empty
	if (isEmpty()) {
		return farmingdale::FAILURE;
	}
	//If it isn't, put the data from head into removedVal
	removedVal = head->data;
	//Make head into current head's next
	llNode<CL>* temp = head;
	head = head->next;
	//if head is null, tail should also be null
	if (0 == head) {
		tail = 0;
	}
	//delete old head
	delete temp;
	//return SUCCESS
	return farmingdale::SUCCESS;
}

template <class CL> farmingdale::linkedList<CL>::~linkedList() {
	// traverse the linked list
	farmingdale::llNode<CL>* current = head;
	while (current != 0) {
		farmingdale::llNode<CL>* next = current->next;

		//delete each node in turn
		delete current;
		current = next;
	}
}

template <class CL> bool farmingdale::linkedList<CL>::operator==(const linkedList<CL>& rhs) const {

	farmingdale::llNode<CL>* current = head;
	farmingdale::llNode<CL>* rhsCurrent = rhs.head;

	//do a paralell traversal
	while (current != 0 && rhsCurrent != 0) {
		//if, during the traversal, current's data and rhs's data isn't the same, return false
		if (current->data != rhsCurrent->data) {
			return false;
		}
		current = current->next;
		rhsCurrent = rhsCurrent->next;
	}
	//if both current and rhsCurrent are null, return true
	if (current == 0 && rhsCurrent == 0) {
		return true;
	}
	//return false
	return false;
}

template <class CL> bool farmingdale::linkedList<CL>::operator==(const std::deque<CL>& rhs) const {
	llNode<CL>* current = head;
	typename std::deque<CL>::const_iterator rhsIterator = rhs.begin();

	//do a paralell traversal
	while (current != 0 && rhsIterator != rhs.end()) {
		//if, during the traversal, current's data and rhsIterator's data isn't the same, return false
		if (current->data != *rhsIterator) {
			return false;
		}
		current = current->next;
		++rhsIterator;
	}
	//if both current and rhsIterator are null, return true
	if (current == 0 && rhsIterator == rhs.end()) {
		return true;
	}
	//return false
	return false;
}

template <class CL> farmingdale::linkedList<CL>::linkedList(const linkedList<CL>& copyMe)
	:
	head(0), tail(0)
{
	llNode<CL>* otherNode = copyMe.head;
	//Traverse copyMe, in each iteration
	while (otherNode != 0) {
		//create a new node
		llNode<CL>* temp = new llNode<CL>;
		//copy the content of the node from the traversal
		temp->data = otherNode->data;
		//add the new node to the end of us
		temp->next = 0;
		//if we exist, add to our tail
		if (tail != 0) {
			tail->next = temp;
		}
		else {
			head = temp;
		}
		tail = temp;
		otherNode = otherNode->next;
	}//end of traversal

}

template <class CL> farmingdale::statusCode farmingdale::linkedList<CL>::insertAfter(llNode<CL>* insertAfterMe, CL stringToInsert) {
	//create a new node, fail if we cannot create
	llNode<CL>* temp = new llNode<CL>;
	try {
		temp = new llNode<CL>;
		temp->data = stringToInsert;
	}
	catch (std::bad_alloc) {
		return farmingdale::FAILURE;
	}

	//stitch node in between inserAfterMe and its successor
	//take next field of new node and put successor there
	temp->next = insertAfterMe->next;

	//make new node successor of insert after me
	insertAfterMe->next = temp;

	//if insertAfter me was the tail, we are the new tail
	if (tail == insertAfterMe) {
		tail = temp;
	}

	//return successs
	return farmingdale::SUCCESS;
}

template <class CL> farmingdale::llNode<CL>* farmingdale::linkedList<CL>::search(CL findMe) const {
	//traverse the list
	llNode<CL>* temp = head;
	while (temp != 0) {
		if (temp->data == findMe) {
			return temp;
		}
		temp = temp->next;
	}
	//if we find the node, return, if we don't, return null
	return temp;
}

template <class CL> farmingdale::statusCode farmingdale::linkedList<CL>::removeAtTail(CL& removeMe) {
	//if empty, return failure
	if (isEmpty()) {
		return FAILURE;
	}
	//do traversal with trailCurrent until current == tail
	llNode<CL>* current = head;
	llNode<CL>* trailCurrent = 0;
	while (current != 0 && current != tail) {
		trailCurrent = current;
		current = current->next;
	}
	//if there was a single node then set head & tail to null
	//otherwise, stitch out the current from the ll and make trail Current's into 0
	if (trailCurrent == 0) {
		head = 0;
		tail = 0;
	}
	else {
		trailCurrent->next = 0;
	}
	tail = trailCurrent;
	//get string from current
	removeMe = current->data;
	//delete current
	delete current;
	//return success
	return SUCCESS;
}

template <class CL> farmingdale::statusCode farmingdale::linkedList<CL>::remove(llNode<CL>* removeMe) {
	//do a traversal with a trailCurrent
	llNode<CL>* current = head;
	llNode<CL>* trailCurrent = 0;
	while (current != 0 && current != removeMe) { //stop when current == removeMe
		trailCurrent = current;
		current = current->next;
	}
	//if current is null then removeMe isn't in this LL, return failure
	if (current == 0) {
		return FAILURE;
	}
	//if removeMe is head, remove the head
	if (removeMe == head) {
		CL tempString;
		return removeAtHead(tempString);
	}
	//if current is an interior node or tail, stitch out current (trailCurrent's new next will be current's current next)
	else {
		trailCurrent->next = current->next;
	}
	//if tail was current, tail will become trailCurrent
	if (current == tail) {
		tail == trailCurrent;
	}
	//delete current
	delete current;
	//return success
	return SUCCESS;
}

template <class CL> const farmingdale::linkedList<CL>& farmingdale::linkedList<CL>::operator=(linkedList<CL> linkle) {
	std::swap(head, linkle.head);
	std::swap(tail, linkle.tail);

	return *this;
}

template <class CL> farmingdale::llNode<CL>* farmingdale::linkedList<CL>::findAtPostion(int findMe) const {
	//traverse the list, stop when you've itereated n times also stop if current is null
	llNode<CL>* current = head;
	int count = 0;
	while (current != 0 && count != findMe) {
		current = current->next;
		count++;
	}
	//return current
	return current;
}

// print a linkedList to a stream (e.g., std::cout)
template <class CL> void farmingdale::linkedList<CL>::printToStream(std::ostream& outputStream) const {

	//traverse and send the data of each node to outputStream
	llNode<CL>* myIter = head;
	while (0 != myIter) {
		if (myIter != head) {
			outputStream << ";";
		}
		outputStream << myIter->data;
		myIter = myIter->next;
	}
}
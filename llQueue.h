#pragma once
#include "statusCode.h"
#include "ostream"
#include "FarmingdaleLinkedList.h"
#include <string>

namespace farmingdale {
	// Note: namespaces are additive, so you can add new elements to this namespace in other files or later
	template <class CL> class llQueue;
}
template <class CL> class farmingdale::llQueue {
private:
	farmingdale::linkedList<CL> theQueue;
public:
	llQueue();
	//rule of threes
	llQueue(const llQueue<CL>& copyMe);
	~llQueue();
	llQueue<CL>& operator=(llQueue<CL>); // using the copy/swap idiom

	inline bool isEmpty() const { return (theQueue.isEmpty()); }
	statusCode enqueue(CL addMe);
	statusCode dequeue(CL& returnedElement);
	statusCode peek(CL& returnedElement) const;
	// compare two stdQueues
	bool operator==(const llQueue<CL>&) const;
	// print a stack to a stream (e.g., std::cout)
	//void printToStream(std::ostream& outputStream);
};
template <class CL> inline std::ostream& operator<<(std::ostream& theStream, farmingdale::llQueue<CL> theQueue) {
	theQueue.printToStream(theStream);
	return theStream;
}
template <class CL> inline bool operator!=(const farmingdale::llQueue<CL>& lhs, const farmingdale::llQueue<CL>& rhs) {
	return (!(lhs == rhs));
}

//print a queue to a stream (e.g., std::cout)
/*template <class CL> void farmingdale::llQueue<CL>::printToStream(std::ostream& theStream) {
	std::cout << theQueue << std::endl;
}*/

//default constructor
template <class CL> farmingdale::llQueue<CL>::llQueue() {}

template <class CL> farmingdale::llQueue<CL>::llQueue(const llQueue<CL>& copyMe) {
	theQueue= copyMe.theQueue;
}

//destructor code
template <class CL> farmingdale::llQueue<CL>::~llQueue() {
	//theQueue.~linkedList();
}

template <class CL> farmingdale::statusCode farmingdale::llQueue<CL>::peek(CL& returnedElement) const {

	//put first item in theQueue into returnedElement & return success or failure
	return theQueue.getHead(returnedElement);
}

template <class CL> farmingdale::statusCode farmingdale::llQueue<CL>::dequeue(CL& returnedElement) {
	
	//put the first item in theQueue into returnedElement & return success or failure
	return theQueue.removeAtHead(returnedElement);
}

template <class CL> farmingdale::statusCode farmingdale::llQueue<CL>::enqueue(CL addMe) {

	
	//insert new item at tail & return success or failure
	return theQueue.insertAtTail(addMe);
}

template <class CL> bool farmingdale::llQueue<CL>::operator==(const llQueue<CL>& rhs) const {
	return (theQueue == rhs.theQueue);
}

template <class CL> farmingdale::llQueue<CL>& farmingdale::llQueue<CL>::operator=(llQueue<CL> copyMe) {
	std::swap(theQueue, copyMe.theQueue);
	return *this;

}
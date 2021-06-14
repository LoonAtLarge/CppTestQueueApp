#pragma once
#include "statusCode.h"
#include "ostream"
#include <queue>
#include <string>

namespace farmingdale {
	// Note: namespaces are additive, so you can add new elements to this namespace in other files or later
	template <class CL> class stdQueue;
}
template <class CL> class farmingdale::stdQueue {
private:
	std::queue<CL> theQueue;
public:
	stdQueue();
	//rule of threes
	stdQueue(const stdQueue& copyMe);
	~stdQueue();
	stdQueue<CL>& operator=(stdQueue); // using the copy/swap idiom

	inline bool isEmpty() const { return (theQueue.empty()); }
	inline bool isFull() { return !(theQueue.empty()); }
	statusCode enqueue(CL addMe);
	statusCode dequeue(CL& returnedElement);
	statusCode peek(CL& returnedElement) const;
	// compare two stdQueues
	bool operator==(const stdQueue<CL>&) const;
	// print a stack to a stream (e.g., std::cout)
	void printToStream(std::ostream& outputStream);
};
template <class CL> inline std::ostream& operator<<(std::ostream& theStream, farmingdale::stdQueue<CL> theQueue) {
	theQueue.printToStream(theStream);
	return theStream;
}
template <class CL> inline bool operator!=(const farmingdale::stdQueue<CL>& lhs, const farmingdale::stdQueue<CL>& rhs) {
	return (!(lhs == rhs));
}

//print a queue to a stream (e.g., std::cout)
template <class CL> void farmingdale::stdQueue<CL>::printToStream(std::ostream& theStream) {
	std::cout << theQueue << std::endl;
}

//default constructor
template <class CL> farmingdale::stdQueue<CL>::stdQueue() {}

template <class CL> farmingdale::stdQueue<CL>::stdQueue(const stdQueue<CL>& copyMe) {
	copyMe.theQueue.queue(theQueue);
}

//destructor code
template <class CL> farmingdale::stdQueue<CL>::~stdQueue() {
	// leaving this empty cuz it's never used and I dunno what to do with it
}

template <class CL> farmingdale::statusCode farmingdale::stdQueue<CL>::peek(CL& returnedElement) const {
	//check if empty and return failure if that's the case
	if (isEmpty()) {
		return farmingdale::FAILURE;
	}
	//put first item in theQueue into returnedElement
	returnedElement = theQueue.front();
	//return success
	return farmingdale::SUCCESS;
}

template <class CL> farmingdale::statusCode farmingdale::stdQueue<CL>::dequeue(CL& returnedElement) {
	//check if empty and return failure if that's the case
	if (isEmpty()) {
		return farmingdale::FAILURE;
	}
	//put the first item in theQueue into returnedElement
	returnedElement = theQueue.front();

	//
	theQueue.pop();

	//return success
	return farmingdale::SUCCESS;
}

template <class CL> farmingdale::statusCode farmingdale::stdQueue<CL>::enqueue(CL addMe) {
	
	//push item into theQueue
	theQueue.push(addMe);
	//return success
	return farmingdale::SUCCESS;
}

template <class CL> bool farmingdale::stdQueue<CL>::operator==(const stdQueue<CL>& rhs) const {
	return (theQueue == rhs.theQueue);
}

template <class CL> farmingdale::stdQueue<CL>& farmingdale::stdQueue<CL>::operator=(stdQueue<CL> copyMe) {
	 theQueue.swap(copyMe);
	 return *this;

}
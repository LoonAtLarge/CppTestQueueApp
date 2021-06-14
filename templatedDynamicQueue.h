#include "statusCode.h"
#include "ostream"

namespace farmingdale {
	// Note: namespaces are additive, so you can add new elements to this namespace in other files or later
	template <class CL> class queue;
}
template <class CL> class farmingdale::queue {
private:
	static const int INITIAL_QUEUE_SIZE = 16; // This should remain the initial size of the dynamic array
	CL* data; //  [INITIAL_QUEUE_SIZE] ;     // This should become a dynamic array in M5

	int oldestIndex;		// index of the oldest element in the queue
	int nextInsertIndex;	// index of the first empty spot
	int currentCapacity;

	// we use capacity. Note that since we want to distinguish between full and empty, we leave
	// a single empty space always in the circular array (see the slides and videos). Because of this, 
	// the array size used for the circular calculations is always one more than the available slots
	// given and index, returns the address of the next index in a circular array
	inline int nextIndexOf(int index) const { return((index + 1) % (currentQueueCapacity() + 1)); }
public:
	queue();
	// rule of threes
	queue(const queue& copyMe);
	~queue();
	queue<CL>& operator=(queue); // using the copy/swap idiom

	// returns the initial size without a reallocation of memory
	inline int initialQueueCapacity() const { return INITIAL_QUEUE_SIZE - 1; }
	// returns the current queue capacity: same as initial except will change in dynamic
	inline int currentQueueCapacity() const { return currentCapacity - 1; }

	inline bool isEmpty() const { return (oldestIndex == nextInsertIndex); }
	inline bool isFull() { return (nextIndexOf(nextInsertIndex) == oldestIndex); }
	statusCode enqueue(CL addMe);
	statusCode dequeue(CL& returnedElement);
	statusCode peek(CL& returnedElement) const;
	// compare two queues
	bool operator==(const queue<CL>&) const;
	// print a stack to a stream (e.g., std::cout)
	void printToStream(std::ostream& outputStream);
};
template <class CL> inline std::ostream& operator<<(std::ostream& theStream, farmingdale::queue<CL> theQueue) {
	theQueue.printToStream(theStream);
	return theStream;
}
template <class CL> inline bool operator!=(const farmingdale::queue<CL>& lhs, const farmingdale::queue<CL>& rhs) {
	return (!(lhs == rhs));
}

//print a queue to a stream (e.g., std::cout)
template <class CL> void farmingdale::queue<CL>::printToStream(std::ostream& theStream) {
	theStream << "(oldest)";
	for (int i = oldestIndex; i != nextInsertIndex; i = nextIndexOf(i)) {
		theStream << data[i];
		if (nextIndexOf(i) != nextInsertIndex) {
			theStream << " ; ";
		}
	}
	theStream << "(newest)";
}

//default constructor
template <class CL> farmingdale::queue<CL>::queue()
//initiallization list
	:
	//set oldest and nextInsert to 0
	oldestIndex(0),
	nextInsertIndex(0),
	currentCapacity(INITIAL_QUEUE_SIZE)
{
	data = new CL[currentCapacity]; //cannot be done in initialization list because new is being called
}

template <class CL> farmingdale::queue<CL>::queue(const queue<CL>& copyMe) {
	//copy oldestIndex from copyMe
	oldestIndex = copyMe.oldestIndex;
	//copy nextInsertIndex from copyMe
	nextInsertIndex = copyMe.nextInsertIndex;
	//copy the currentCapacity
	currentCapacity = copyMe.currentCapacity;
	//allocate the buffer using new
	data = new CL[currentCapacity];
	//copy every item in array from oldestIndex to nextInsertIndex into data
	for (int i = copyMe.oldestIndex; i != copyMe.nextInsertIndex; i = nextIndexOf(i)) {
		data[i] = copyMe.data[i];
	}
}

//destructor code
template <class CL> farmingdale::queue<CL>::~queue() {
	delete[] data;
}

template <class CL> farmingdale::statusCode farmingdale::queue<CL>::peek(CL& returnedElement) const {
	//check if empty and return failure if that's the case
	if (isEmpty()) {
		return farmingdale::FAILURE;
	}
	//put item [oldestIndex] into returnedElement
	returnedElement = data[oldestIndex];
	//return success
	return farmingdale::SUCCESS;
}

template <class CL> farmingdale::statusCode farmingdale::queue<CL>::dequeue(CL& returnedElement) {
	//check if empty and return failure if that's the case
	if (isEmpty()) {
		return farmingdale::FAILURE;
	}
	//put item [oldestIndex] into returnedElement
	returnedElement = data[oldestIndex];
	//increment oldestIndex
	oldestIndex = nextIndexOf(oldestIndex);
	//return success
	return farmingdale::SUCCESS;
}

template <class CL> farmingdale::statusCode farmingdale::queue<CL>::enqueue(CL addMe) {
	//check if queue is full, Grow the queue as follows:
	if (isFull()) {
		int doubleCapacity = currentCapacity * 2;
		//allocate new memory, 2x the currentCapacity
		CL* newDataMemory;
		try { //try-catch in case system cannot allocate memory due to lack of free memory space (use this whenever you call new outside of the constructor)
			newDataMemory = new CL[doubleCapacity];
		}
		catch (std::bad_alloc) { //bad_alloc error indicates that memory could not be allocated
			return farmingdale::FAILURE;
		}
		//copy the data
		int count = 0;
		for (int i = oldestIndex; i != nextInsertIndex; i = nextIndexOf(i)) {
			newDataMemory[count] = data[i];
			count++;
		}
		//fix the indexes (oldest will always end up as [0], nextInsert will always be the number of items copied)
		oldestIndex = 0;
		nextInsertIndex = count;
		//set data to the new memory & call delete[] on old memory
		CL* fodderMemory;
		fodderMemory = data;
		data = newDataMemory;
		delete[] fodderMemory;
		//set new capacity
		currentCapacity = doubleCapacity;
	}
	//if not, add item in slot
	data[nextInsertIndex] = addMe;
	//advance nextInsert to nextSlot
	nextInsertIndex = nextIndexOf(nextInsertIndex);
	//return success
	return farmingdale::SUCCESS;
}

template <class CL> bool farmingdale::queue<CL>::operator==(const queue<CL>& rhs) const {
	int myIterator = oldestIndex;
	int otherIterator = rhs.oldestIndex;

	while (myIterator != nextInsertIndex && otherIterator != rhs.nextInsertIndex) {
		//compare
		if (data[myIterator] != rhs.data[otherIterator]) {
			return false;
		}
		//advance
		myIterator = nextIndexOf(myIterator);
		otherIterator = rhs.nextIndexOf(otherIterator);
	}
	if (myIterator != nextInsertIndex || otherIterator != rhs.nextInsertIndex) {
		return false;
	}

	return true;
}

template <class CL> farmingdale::queue<CL>& farmingdale::queue<CL>::operator=(queue<CL> copyMe) {
	//swap the data
	std::swap(data, copyMe.data);

	//get copyMe's other resources (variables)
	currentCapacity = copyMe.currentCapacity;
	nextInsertIndex = copyMe.nextInsertIndex;
	oldestIndex = copyMe.oldestIndex;

	//return ourself so that we can do "a = b = c" (chaining)
	return *this;
	//scuffed explanation of chaining: the last variable, c, must have a value so that a value for b can be determined, 
	//so that the first = operator has a parameter to pass to a

}
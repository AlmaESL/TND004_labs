#include "set.h"
#include "node.h"

int Set::Node::count_nodes = 0;

/*****************************************************
 * Implementation of the member functions             *
 ******************************************************/

 /*
  *  Default constructor :create an empty Set
  */
int Set::get_count_nodes() {
	return Set::Node::count_nodes;
}

/*
 *  Default constructor :create an empty Set
 */
Set::Set() : counter{ 0 } {
	// IMPLEMENT before Lab2 HA

	//book p. 99
	head = new Node;
	tail = new Node;
	head->next = tail;
	tail->prev = head;
}

/*
 *  Conversion constructor: convert val into a singleton {val}
 */
Set::Set(int val) : Set{} {  // create an empty list
	// IMPLEMENT before Lab2 HA

	// create and insert nodes of val (from the back) using the insert node function 
	insert_node(tail->prev, val); 
}

/*
 * Constructor to create a Set from a sorted vector of unique ints
 * Create a Set with all ints in sorted vector list_of_values
 */
Set::Set(const std::vector<int>& list_of_values) : Set{} {  // create an empty list
	// IMPLEMENT before Lab2 HA

	Node* ptr = head; 

	// 1) iterate elements in vector
	for (int e : list_of_values) {

		// 2) insert each element e as a new node using the insert_node function
		insert_node(ptr, e);

		// 3) step forth in this set
		ptr = ptr->next; 
	}
}

/*
 * Copy constructor: create a new Set as a copy of Set S
 * \param S Set to copied
 * Function does not modify Set S in any way
 */
Set::Set(const Set& S) : Set{} {  // create an empty list
	// IMPLEMENT before Lab2 HA


	Node* s_ptr = S.head->next; 

	// 1) iterate S
	while (s_ptr != S.tail) {

		// 2) use the insert_node function to insert value os into this from the back 
		insert_node(tail->prev, s_ptr->value);

		// 3) step forth in S
		s_ptr = s_ptr->next; 
	}

}

/*
 * Transform the Set into an empty set
 * Remove all nodes from the list, except the dummy nodes
 */
void Set::make_empty() {
	// IMPLEMENT before Lab2 HA

	Node* ptr = head->next;

	// 1) Traverse set and use remove to delete each node encountered 
	while (ptr != tail) {

		// 2) create a temp node  
		Node* temp = ptr->next;

		// 3) delete ptr node
		remove_node(ptr);

		// 4) new current node is the next node in the set
		ptr = temp;
	}

}

/*
 * Destructor: deallocate all memory (Nodes) allocated for the list
 */
Set::~Set() {
	// IMPLEMENT before Lab2 HA

	// 1) use the make empty to delete all elements 
	make_empty();

	// 2) delete head and tail 
	delete head;
	delete tail;
}

/*
 * Assignment operator: assign new contents to the *this Set, replacing its current content
 * \param S Set to be copied into Set *this
 * Call by valued is used
 */
Set& Set::operator=(Set S) {
	// IMPLEMENT before Lab2 HA

	//copy swap idiom on heads, tails and counters -> constant swaps since swaps on single variables 
	std::swap(head, S.head);
	std::swap(tail, S.tail);
	std::swap(counter, S.counter);

	return *this;

}



/*
 * Test whether val belongs to the Set
 * Return true if val belongs to the set, otherwise false
 * This function does not modify the Set in any way
 */
bool Set::is_member(int val) const {
	// IMPLEMENT before Lab2 HA

	Node* ptr = head->next;

	// 1) traverse set and search for val
	while (ptr != tail && ptr->value != val) {
		ptr = ptr->next;
	}

	// 2) ptr != tail means val is found 
	return (ptr != tail);
}

/*
 * Test whether Set *this and S represent the same set
 * Return true, if *this has same elemnts as set S
 * Return false, otherwise
 */
bool Set::operator==(const Set& S) const {
	// IMPLEMENT before Lab2 HA

	// 1) sets must have same size for ==
	if (counter != S.counter) {
		return false;
	}


	Node* this_ptr = head->next;
	Node* s_ptr = S.head->next;

	// 2) iterate this
	while (this_ptr != tail) {

		// 3) compare and if certain values aren't identical, == is false
		if (this_ptr->value != s_ptr->value) {
			return false;
		}

		this_ptr = this_ptr->next;
		s_ptr = s_ptr->next;
	}

	// 4) if while-loop stops without return, all elements are identical and == is true
	return true;
}

/*
 * Three-way comparison operator: to test whether *this == S, *this < S, *this > S
 * Return std::partial_ordering::equivalent, if *this == S
 * Return std::partial_ordering::less, if *this < S
 * Return std::partial_ordering::greater, if *this > S
 * Return std::partial_ordering::unordered, otherwise
 */
std::partial_ordering Set::operator<=>(const Set& S) const {
	// IMPLEMENT before Lab2 HA

	// 1) test for equivalance, using operator ==
	if (*this == S) {
		return std::partial_ordering::equivalent; 
	}

	
	Node* this_ptr = head->next; 
	Node* s_ptr = S.head->next; 

	// 2) counter for identical values 
	std::size_t counter_identical{ 0 }; 

	// 3) loop sets until one is ended
	while (this_ptr != tail->next && s_ptr != S.tail->next) {

		// 4) step forth in set with smallest value
		if (this_ptr->value < s_ptr->value) {
			this_ptr = this_ptr->next; 
		}
		else if (this_ptr->value > s_ptr->value) {
			s_ptr = s_ptr->next; 
		}
		// 5) if values are same, step forth in both and increment identical counter
		else if (this_ptr->value == s_ptr->value) {
			this_ptr = this_ptr->next; 
			s_ptr = s_ptr->next; 
			++counter_identical; 
		}

	}

	// 6) compare identical counter to counters of sets, counter equaling size of either set gives which set is the larger/smaller
	if (counter_identical == S.counter) {
		return std::partial_ordering::greater; 
	}
	else if(counter_identical == counter) {
		return std::partial_ordering::less;
	}
	
	// 7) if all tests above fail, sets are not comparable 
	return std::partial_ordering::unordered;
}

/*
 * Modify Set *this such that it becomes the union of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator+=(const Set& S) {
	// IMPLEMENT

	Node* s_ptr = S.head->next; 
	Node* this_ptr = head->next; 

	// 1) iterate S
	while (s_ptr != S.tail) {
		
		// 2) if end of this is reached or s value smaller than this value, insert in this before this_ptr = this_ptr.prev since 
		// insert_node inserts after inout pointer 
		if (this_ptr == tail || s_ptr->value < this_ptr->value) {
			
			insert_node(this_ptr->prev, s_ptr->value);
			s_ptr = s_ptr->next; 
		}
		// 3) if this value smaller than s, wait for case s < this for insertion, by stepping forth in this
		else if (s_ptr->value > this_ptr->value) {
			
			this_ptr = this_ptr->next;
		}
		// 4) if this value and s value are same, step forth in both sets, doing nothing
		else { 
			this_ptr = this_ptr->next;
			s_ptr = s_ptr->next;
		}
	}

	return *this;
}

/*
 * Modify Set *this such that it becomes the intersection of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator*=(const Set& S) {
	// IMPLEMENT

	Node* this_ptr = head->next; 
	Node* s_ptr = S.head->next; 

	// 1) loop sets
	while (s_ptr != S.tail && this_ptr != tail) {

		// 2) if this < s, step forth in this and remove this since it is not found in s
		if (this_ptr->value < s_ptr->value) {
			this_ptr = this_ptr->next; 
			remove_node(this_ptr->prev); 
		}
		// 3) if s > this, s is not in this -> step forth in s
		else if (this_ptr->value > s_ptr->value) {
			s_ptr = s_ptr->next; 
		}
		// 4) if s==this, step forth in both sets 
		else {
			this_ptr = this_ptr->next; 
			s_ptr = s_ptr->next; 
		}
	}

	// 5) empty remaining elements in this if this > S
	while (this_ptr != tail) {
		this_ptr = this_ptr->next; 
		remove_node(this_ptr->prev); 
	}


	return *this;
}

/*
 * Modify Set *this such that it becomes the Set difference between Set *this and Set S
 * Set *this is modified and then returned
 */
Set& Set::operator-=(const Set& S) {
	// IMPLEMENT

	Node* this_ptr = head->next; 
	Node* s_ptr = S.head->next; 

	// 1) loop the sets 
	while (this_ptr != tail && s_ptr != S.tail) {

		// 2) if this < S or this > S, move forth on set with smaller element 
		if (this_ptr->value < s_ptr->value) {
			this_ptr = this_ptr->next; 
		}
		else if (this_ptr->value > s_ptr->value) {
			s_ptr = s_ptr->next; 
		}
		// 3) if values are the same, step forth and remove from this
		else {
			this_ptr = this_ptr->next; 
			s_ptr = s_ptr->next;

			remove_node(this_ptr->prev); 

		}
	}


	return *this;
}


/* ******************************************** *
 * Private Member Functions -- Implementation   *
 * ******************************************** */

 /*
  * Insert a new Node storing val after the Node pointed by p
  * \param p pointer to a Node
  * \param val value to be inserted  after position p
  */
void Set::insert_node(Node* p, int val) {
	// IMPLEMENT before Lab2 HA

	// 1) create a new node, insert val, and let it point to head and tail  
	Node* new_node = new Node(val);

	// 2) point new node to p
	new_node->prev = p;

	// 3) point new node to node after p
	new_node->next = p->next;

	// 4) point node after p tp new node
	p->next->prev = new_node;

	// 5) point p to new node
	p->next = new_node;

	// 6) increment the counter 
	++counter;
}

/*
 * Remove the Node pointed by p
 * \param p pointer to a Node
 *
 * used by the make_empty function
 */
void Set::remove_node(Node* p) {
	// IMPLEMENT before Lab2 HA

	// 1) point node before p to node after p
	p->prev->next = p->next;

	// 2) point node after p to node before p
	p->next->prev = p->prev;

	// 3) delete p 
	delete p;

	// 4) decrement the counter 
	--counter;
}

/*
 * Write Set *this to stream os
 */
void Set::write_to_stream(std::ostream& os) const {
	if (is_empty()) {
		os << "Set is empty!";
	}
	else {
		Set::Node* ptr{ head->next };

		os << "{ ";
		while (ptr != tail) {
			os << ptr->value << " ";
			ptr = ptr->next;
		}
		os << "}";
	}
}



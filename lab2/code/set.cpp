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

	//use the insert aux-function to insert val 
	insert_value(val);


}

/*
 * Constructor to create a Set from a sorted vector of unique ints
 * Create a Set with all ints in sorted vector list_of_values
 */
Set::Set(const std::vector<int>& list_of_values) : Set{} {  // create an empty list
	// IMPLEMENT before Lab2 HA

	//loop vector and insert each element using the insert_value aux-function 
	for (int e : list_of_values) {
		insert_value(e);
	}
}

/*
 * Copy constructor: create a new Set as a copy of Set S
 * \param S Set to copied
 * Function does not modify Set S in any way
 */
Set::Set(const Set& S) : Set{} {  // create an empty list
	// IMPLEMENT before Lab2 HA

	// 1) point at first node in S 
	Node* s_ptr = S.head->next;


	// 2) iterate S 
	while (s_ptr != S.tail) {

		// 3) insert current s value into this
		insert_value(s_ptr->value);

		// 4) step forth in S
		s_ptr = s_ptr->next;
	}

}

/*
 * Transform the Set into an empty set
 * Remove all nodes from the list, except the dummy nodes
 */
void Set::make_empty() {
	// IMPLEMENT before Lab2 HA

	// 1) point at first node 
	Node* ptr = head->next;

	// 2) Traverse set and use remove to delete each node encountered 
	while (ptr != tail) {

		// 3) create a temp node  
		Node* temp = ptr->next;

		// 4) delete ptr node
		remove_node(ptr);

		// 5) new current node is the next node in the set
		ptr = temp;
	}

}

/*
 * Destructor: deallocate all memory (Nodes) allocated for the list
 */
Set::~Set() {
	// IMPLEMENT before Lab2 HA

	// use the make empty to delete all elements 
	make_empty();

	//delete head and tail 
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

	//copy swap idiom on heads, tails and counters -> constant swaps on single vriables 
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
	
	// 1) point at first node 
	Node* ptr = head->next; 

	// 2) traverse set and search for val
	while (ptr != tail && ptr->value != val) {
		ptr = ptr->next; 
	}

	// 3) ptr != tail means val is found 
	return (ptr != tail); 
}

/*
 * Test whether Set *this and S represent the same set
 * Return true, if *this has same elemnts as set S
 * Return false, otherwise
 */
bool Set::operator==(const Set& S) const {
	// IMPLEMENT before Lab2 HA
	

	// returns true if S and this have the same size and <=> returns equivalent 
	if (counter != S.counter) {
		return false; 
	}

	return ((*this <=> S) == std::partial_ordering::equivalent); 

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
	
	// 1) pointers at first elements of S and this
	Node* s_ptr = S.head->next; 
	Node* this_ptr = head->next; 

	// 2) traverse both sets 
	while (s_ptr != S.tail && this_ptr != tail) {

		// 3) compare the current values -> if this < S -> return less
		if (this_ptr->value < s_ptr->value) {
			return std::partial_ordering::less; 
		}

		// 4) compare the current values -> if this > S -> return greater 
		if (this_ptr->value > s_ptr->value) {
			return std::partial_ordering::greater; 
		}

		// 5) step forth in both sets 
		s_ptr = s_ptr->next; 
		this_ptr = this_ptr->next; 
	}

	// 6) if s is smaller than this -> greater 
	if (s_ptr != S.tail) {
		return std::partial_ordering::greater; 
	}

	// 7) if this is smaller than S -> less
	if (this_ptr != tail) {
		return std::partial_ordering::less;
	}

	// 8) sets are qeuivalent if above tests fail 
	return std::partial_ordering::equivalent; 

}

/*
 * Modify Set *this such that it becomes the union of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator+=(const Set& S) {
	// IMPLEMENT
	return *this;
}

/*
 * Modify Set *this such that it becomes the intersection of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator*=(const Set& S) {
	// IMPLEMENT
	return *this;
}

/*
 * Modify Set *this such that it becomes the Set difference between Set *this and Set S
 * Set *this is modified and then returned
 */
Set& Set::operator-=(const Set& S) {
	// IMPLEMENT
	return *this;
}


/* ******************************************** *
 * Private Member Functions -- Implementation   *
 * ******************************************** */

 /*
  * Insert a new Node storing val after the Node pointed by p
  * \param p pointer to a Node
  * \param val value to be inserted  after position p
  *
  * used by insert_value
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

	//decrement the counter 
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


/*
* Aux-functions
*/

//to convert and insert a singleton 
void Set::insert_value(int val) {

	// Find the position to insert the value
	Node* ptr = head;
	while (ptr->next != tail && ptr->next->value < val) {
		ptr = ptr->next;
	}

	// Insert the value using insert_node function
	if (ptr->next == tail || ptr->next->value > val) {
		insert_node(ptr, val);
	}

}

//operator <=
bool operator<= (const Set& S1, const Set& S2) {

	//returns true if <=> returns less or equal for S1=this
	return(S1 <=> S2) != std::partial_ordering::greater;
}
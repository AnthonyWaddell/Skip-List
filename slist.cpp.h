//-----------------------------------------------------------------------------
// constructor
//-----------------------------------------------------------------------------
template<class Object>
SList<Object>::SList()
{
	init();
}

//-----------------------------------------------------------------------------
// List definition
//-----------------------------------------------------------------------------
template<class Object>
SList<Object>::SList(const SList &rhs)
{
	init();
	*this = rhs;                                   // then assign rhs to this.
}

template<class Object>
void SList<Object>::init()
{
	for (int i = 0; i < LEVEL; i++)
	{  // for each level
	  // create the left most dummy nodes;
		header[i] = new SListNode<Object>;
		header[i]->prev = NULL;
		header[i]->down = (i > 0) ? header[i - 1] : NULL;
		header[i]->up = NULL;
		if (i > 0) header[i - 1]->up = header[i];

		// create the right most dummy nodes;
		header[i]->next = new SListNode<Object>;
		header[i]->next->next = NULL;
		header[i]->next->prev = header[i];
		header[i]->next->down = (i > 0) ? header[i - 1]->next : NULL;
		header[i]->next->up = NULL;
		if (i > 0) header[i - 1]->next->up = header[i]->next;
	}

	// reset cost.
	cost = 0;
}

//-----------------------------------------------------------------------------
//destructor
//-----------------------------------------------------------------------------
template<class Object>
SList<Object>::~SList()
{
	clear();                                      // delete items starting 1st
	for (int i = 0; i < LEVEL; i++)
	{
		delete header[i]->next;                      // delete the right most dummy
		delete header[i];                            // delete the left most dummy
	}
}

//-----------------------------------------------------------------------------
// check to see if empty
//-----------------------------------------------------------------------------
template<class Object>
bool SList<Object>::isEmpty() const
{
	return (header[0]->next->next == NULL);
}

//-----------------------------------------------------------------------------
// getSize
//-----------------------------------------------------------------------------
template<class Object>
int SList<Object>::size() const
{
	SListNode<Object> *p = header[0]->next; // at least the right most dummy
	int size = 0;

	for (; p->next != NULL; p = p->next, ++size);
	return size;
}

//-----------------------------------------------------------------------------
// clear list
//-----------------------------------------------------------------------------
template<class Object>
void SList<Object>::clear()
{
	for (int i = 0; i < LEVEL; i++)
	{        // for each level
		SListNode<Object> *p = header[i]->next;  // get the 1st item 
		while (p->next != NULL)
		{              // if this is not the left most
			SListNode<Object> *del = p;
			p = p->next;                           // get the next item
			delete del;                            // delete the current item
		}

		header[i]->next = p;                     // p now points to the left most 
	}                                          // let the right most point to it
}

//-----------------------------------------------------------------------------
// Function:	template<class Object>
//				void SList<Object>::insert(const Object &obj)
// Title:		Insert
// Description: Inserts an item into the skip list and uses a simulated coin
//					flip to determine whether to propogate up or not
//
// Programmer:	Anthony Waddell
// Date:		11-16-17
// Version:		1.0
//  
// Environment: Hardware: PC, i7
// Software:	OS: Windows 10 
//				Compiles under Microsoft Visual C++ 2015
// 
// Input:		N/A
// Output:		N/A
// Calls:		searchPointer()
// Called By:	main()
// Parameters:	const Object &obj; the data to be inserted into the node
// Returns:		void
// History Log:	11-16-17 AW Began Function
//				11-17-17 AW Completed Function
//-----------------------------------------------------------------------------
template<class Object>
void SList<Object>::insert(const Object &obj)
{
	// right points to the level-0 item before which a new object is inserted.
	SListNode<Object> *right = searchPointer(obj);
	SListNode<Object> *up = NULL;

	// there is an identical object
	if (right->next != NULL && right->item == obj)
		return;

	//Construct node with the input data
	SListNode<Object> *newptr = new SListNode<Object>;
	newptr->item = obj;

	//Insert into list and fix pointers
	newptr->prev = right->prev;
	newptr->next = right;
	right->prev->next = newptr;
	right->prev = newptr;
	newptr->down = NULL;
	newptr->up = NULL;

	// Propogate up
	int flip = 0;
	int count = 0;
	do {
		// If 1 build up one level
		flip = (rand() % 2);
		if (flip && count < LEVEL - 1)
		{
			count++;
			// Move left toawrds -inf until finding link to next level up
			up = right->prev;
			while (up->up == NULL)
			{
				up = up->prev;
			}
			// Move up 1 level
			up = up->up;
			// Shift right one level
			up = up->next;

			// Insert new node and clean up pointers
			SListNode<Object> *m_node = new SListNode<Object>;
			m_node->prev = up->prev;
			m_node->next = up;
			m_node->item = obj;
			m_node->prev->next = m_node;
			m_node->next->prev = m_node;
			m_node->up = NULL;
			m_node->down = newptr;
			newptr->up = m_node;

			// Move up to newly created level
			newptr = m_node;
			right = up;
		}
	} while (flip);
}

//-----------------------------------------------------------------------------
// find item
//-----------------------------------------------------------------------------
template<class Object>
bool SList<Object>::find(const Object &obj)
{
	// p oints to the level-0 item close to a given object
	SListNode<Object> *p = searchPointer(obj);

	return (p->next != NULL && p->item == obj);     // true if obj was found
}

//-----------------------------------------------------------------------------
// searchpointer
//-----------------------------------------------------------------------------
template<class Object>
SListNode<Object> *SList<Object>::searchPointer(const Object &obj)
{
	SListNode<Object> *p = header[LEVEL - 1];     // start from the top left
	while (p->down != NULL)
	{                   // toward level 0
		p = p->down;                                // shift down once
		cost++;

		if (p->prev == NULL)
		{                    // at the left most item
			if (p->next->next == NULL)              // no intermediate items
				continue;
			else
			{                                    // some intermadiate items
				if (p->next->item <= obj)             // if 1st item <= obj
					p = p->next;                          // shift right to item 1
				cost++;
			}
		}

		while (p->next->next != NULL && p->next->item <= obj)
		{
			// shift right through intermediate items as far as the item value <= obj
			p = p->next;
			cost++;
		}
	}

	// now reached the bottom. shift right once if the current item < obj
	if (p->prev == NULL || p->item < obj)
	{
		p = p->next;
		cost++;
	}
	return p; // return the pointer to an item >= a given object.
}


//-----------------------------------------------------------------------------
// Function:	template<class Object>
//				void SList<Object>::remove(const Object &obj)
// Title:		Insert
// Description: Removes a node and its tower (if more than 1) from the skip 
//				list
//
// Programmer:	Anthony Waddell
// Date:		11-16-17
// Version:		1.0
//  
// Environment: Hardware: PC, i7
// Software:	OS: Windows 10 
//				Compiles under Microsoft Visual C++ 2015
// 
// Input:		N/A
// Output:		N/A
// Calls:		searchPointer()
// Called By:	main()
// Parameters:	const Object &obj; the data to be inserted into the node
// Returns:		void
// History Log:	11-16-17 AW Began Function
//				11-17-17 AW Completed Function
//-----------------------------------------------------------------------------
template<class Object>
void SList<Object>::remove(const Object &obj)
{
	// p points to the level-0 item to delete
	SListNode<Object> *p = searchPointer(obj);

	// validate if p is not the left most or right most and exactly contains the
	// item to delete
	if (p->prev == NULL || p->next == NULL || p->item != obj)
		return;

	// Node found, perform deletion
	do
	{
		// Link the left and the right sides of p
		p->prev->next = p->next;
		p->next->prev = p->prev;

		// Move p and delete from bottom of tower up
		p = p->up;
		delete p->down;
	} while (p->up != NULL);

	// Prevent memory leak
	p->prev->next = p->next;
	p->next->prev = p->prev;
	delete p;
}

//-----------------------------------------------------------------------------
// List definition
//-----------------------------------------------------------------------------
template<class Object>
const SList<Object> &SList<Object>::operator=(const SList &rhs)
{
	if (this != &rhs)// avoid self-assignment
	{
		clear();           // deallocate old items

		int index;
		SListNode<Object> *rnode;
		for (index = 0, rnode = rhs.header[0]->next; rnode->next != NULL;
			rnode = rnode->next, ++index)
			insert(rnode->item);

		cost = rhs.cost;
	}
	return *this;
}

//-----------------------------------------------------------------------------
// accessor
//-----------------------------------------------------------------------------
template<class Object>
int SList<Object>::getCost() const
{
	return cost;
}

//-----------------------------------------------------------------------------
// print list
//-----------------------------------------------------------------------------
template<class Object>
void SList<Object>::show() const
{
	cout << "contents:" << endl;
	for (SListNode<Object> *col = header[0]; col != NULL; col = col->next)
	{
		SListNode<Object> *row = col;
		for (int level = 0; row != NULL && level < LEVEL; level++)
		{
			if (row->prev == NULL)
				cout << "-inf\t";
			else if (row->next == NULL)
				cout << "+inf\t";
			else
				cout << row->item << "\t";
			row = row->up;
		}
		cout << endl;
	}
}
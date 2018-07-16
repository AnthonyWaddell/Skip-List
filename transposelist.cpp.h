template<class Object>
int TransposeList<Object>::find(const Object &obj) 
{
	DListNode<Object> *found = DList<Object>::header->next;

	int i = 0;
	for (; found != NULL && found->item != obj; found = found->next, ++i)
		++DList<Object>::cost;

	if (found == NULL)
		return -1; // not found 

	if (found == DList<Object>::header->next)
		return 0;  // no need to swap 

	DListNode<Object> *newPtr = found
	remove(found);			   // remove found from the current position 
	insert(newptr);			   // insert found before previous 

	return i;
}

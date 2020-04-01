#pragma once
#include "List.h"
#include "queue.h"
#include <iostream>
#include <stdexcept>
using namespace std;

// class RBTree implements the operations in Red Black Tree
class RBTree {
private:

	// data structure that represents a node in the tree
	struct Node {
		char symb; // holds the key
		int value; //holds the value
		Node* parent; // pointer to the parent
		Node* left; // pointer to left child
		Node* right; // pointer to right child
		bool isRed; // true -> Red, false -> Black
		queue code;
	};
	typedef Node* NodePtr;
	NodePtr root;
	NodePtr NIL;
	List sorted;

	// initializes the nodes with appropirate values
	// all the pointers are set to point to the null pointer
	void initializeNULLNode(NodePtr node, NodePtr parent) {
		node->symb = 0;
		node->value = 0;
		node->parent = parent;
		node->left = nullptr;
		node->right = nullptr;
		node->isRed = false;
	}

	void preOrderHelper(NodePtr node) {
		if (node != NIL) {
			cout << node->symb << " ";
			cout << node->value << " ";
			preOrderHelper(node->left);
			preOrderHelper(node->right);
		}
	}

	void inOrderHelper(NodePtr node, bool keys, bool values) {
		if (node != NIL) {
			inOrderHelper(node->left, keys, values);
			if (keys == true) {
				cout << "Symbol: ";
				if (node->symb == ' ')
					cout << "_" << " Its code: ";
				else 
					cout << node->symb << " Its code: ";
				node->code.show_queue();
				cout << endl;
			}
				
			if (values == true)
				cout << node->value << endl;
			inOrderHelper(node->right, keys, values);
		}
	}

	void postOrderHelper(NodePtr node) {
		if (node != NIL) {
			postOrderHelper(node->left);
			postOrderHelper(node->right);
			cout << node->symb << " ";
			cout << node->value << " ";
		}
	}

	NodePtr searchTreeHelper(NodePtr node, char key) {
		if (node == NIL || key == node->symb) {
			if (node == NIL) {
				//cout << "The tree does not contain key " << key << endl;
				return node;
			}
			else {
				//cout << "The tree does contain " << key << ". Its value is " << node->value << endl;
				return node;
			}
		}

		if (key < node->symb) {
			return this->searchTreeHelper(node->left, key);
		}
		return this->searchTreeHelper(node->right, key);
	}

	// fix the rb tree modified by the delete operation
	void fixDelete(NodePtr instead_scnd) {
		NodePtr sibling;
		while (instead_scnd != root && instead_scnd->isRed == false) {
			if (instead_scnd == instead_scnd->parent->left) {
				sibling = instead_scnd->parent->right;
				if (sibling->isRed == true) {
					// case 3.1
					sibling->isRed = false;
					instead_scnd->parent->isRed = true;
					leftRotate(instead_scnd->parent);
					sibling = instead_scnd->parent->right;
				}

				if (sibling->left->isRed == false && sibling->right->isRed == false) {
					// case 3.2
					sibling->isRed = true;
					instead_scnd = instead_scnd->parent;
				}
				else {
					if (sibling->right->isRed == false) {
						// case 3.3
						sibling->left->isRed = false;
						sibling->isRed = true;
						rightRotate(sibling);
						sibling = instead_scnd->parent->right;
					}

					// case 3.4
					sibling->isRed = instead_scnd->parent->isRed;
					instead_scnd->parent->isRed = false;
					sibling->right->isRed = false;
					leftRotate(instead_scnd->parent);
					instead_scnd = root;
				}
			}
			else {
				sibling = instead_scnd->parent->left;
				if (sibling->isRed == true) {
					// case 3.1
					sibling->isRed = false;
					instead_scnd->parent->isRed = true;
					rightRotate(instead_scnd->parent);
					sibling = instead_scnd->parent->left;
				}

				if (sibling->left->isRed == false && sibling->right->isRed == false) {
					// case 3.2
					sibling->isRed = true;
					instead_scnd = instead_scnd->parent;
				}
				else {
					if (sibling->left->isRed == false) {
						// case 3.3
						sibling->right->isRed = false;
						sibling->isRed = true;
						leftRotate(sibling);
						sibling = instead_scnd->parent->left;
					}

					// case 3.4
					sibling->isRed = instead_scnd->parent->isRed;
					instead_scnd->parent->isRed = false;
					sibling->left->isRed = false;
					rightRotate(instead_scnd->parent);
					instead_scnd = root;
				}
			}
		}
		instead_scnd->isRed = false;
	}

	//replacing first node with second node
	void rbTransplant(NodePtr first_node, NodePtr scnd_node) {
		if (first_node->parent == nullptr) {
			root = scnd_node;
		}
		else if (first_node == first_node->parent->left) {
			first_node->parent->left = scnd_node;
		}
		else {
			first_node->parent->right = scnd_node;
		}
		scnd_node->parent = first_node->parent;
	}

	void deleteNodeHelper(NodePtr node, char key) {
		// find the node containing key
		NodePtr cur = NIL;
		NodePtr instead_scnd, instead_first;
		bool found = false;
		while (node != NIL) {
			if (node->symb == key) {
				cur = node;
				found = true;
			}
			if (node->symb <= key) {
				node = node->right;
			}
			else {
				node = node->left;
			}
		}

		if (found == false && node == NIL) {
			throw "There's no key like that";
			return;
		}



		instead_first = cur;
		bool instead_original_color = instead_first->isRed;
		if (cur->left == NIL) {
			instead_scnd = cur->right;
			rbTransplant(cur, cur->right);
		}
		else if (cur->right == NIL) {
			instead_scnd = cur->left;
			rbTransplant(cur, cur->left);
		}
		else {
			instead_first = minimum(cur->right);
			instead_original_color = instead_first->isRed;
			instead_scnd = instead_first->right;
			if (instead_first->parent == cur) {
				instead_scnd->parent = instead_first;
			}
			else {
				rbTransplant(instead_first, instead_first->right);
				instead_first->right = cur->right;
				instead_first->right->parent = instead_first;
			}

			rbTransplant(cur, instead_first);
			instead_first->left = cur->left;
			instead_first->left->parent = instead_first;
			instead_first->isRed = cur->isRed;
		}
		delete cur;
		if (instead_original_color == false) {
			fixDelete(instead_scnd);
		}
	}

	// fix the red-black tree
	void fixInsert(NodePtr key) {
		NodePtr uncle;
		while (key->parent->isRed == true) {
			if (key->parent == key->parent->parent->right) {
				uncle = key->parent->parent->left; // uncle
				if (uncle->isRed == true) {
					// case 3.1 - 
					uncle->isRed = false;
					key->parent->isRed = false;
					key->parent->parent->isRed = true;
					key = key->parent->parent;
				}
				else {
					if (key == key->parent->left) {
						// case 3.2.2 - 
						key = key->parent;
						rightRotate(key);
					}
					// case 3.2.1 - 
					key->parent->isRed = false;
					key->parent->parent->isRed = true;
					leftRotate(key->parent->parent);
				}
			}
			else {
				uncle = key->parent->parent->right;

				if (uncle->isRed == true) {
					// mirror case 3.1 - 
					uncle->isRed = false;
					key->parent->isRed = false;
					key->parent->parent->isRed = true;
					key = key->parent->parent;
				}
				else {
					if (key == key->parent->right) {
						// mirror case 3.2.2 - 
						key = key->parent;
						leftRotate(key);
					}
					// mirror case 3.2.1
					key->parent->isRed = false;
					key->parent->parent->isRed = true;
					rightRotate(key->parent->parent);
				}
			}
			if (key == root) {
				break;
			}
		}
		root->isRed = false;
	}

	void printHelper(NodePtr root, string indent, bool last) {
		// print the tree structure on the screen
		if (root != NIL) {
			cout << indent;
			if (last) {
				cout << "R----";
				indent += "     ";
			}
			else {
				cout << "L----";
				indent += "|    ";
			}

			string sColor = root->isRed ? "RED" : "BLACK";
			if (root->symb == ' ')
				cout << "_";
			else
				cout << root->symb;
			cout << " " << root->value << "(" << sColor << ")" << endl;
			printHelper(root->left, indent, false);
			printHelper(root->right, indent, true);
		}
		// cout<<root->left->data<<endl;
	}

public:
	RBTree() {
		NIL = new Node;
		NIL->isRed = false;
		NIL->left = nullptr;
		NIL->right = nullptr;
		root = NIL;
	}

	// Pre-Order traversal
	// Node->Left Subtree->Right Subtree
	void preorder() {
		preOrderHelper(this->root);
	}

	// In-Order traversal
	// Left Subtree -> Node -> Right Subtree
	void inorder(bool keys, bool values) {
		inOrderHelper(this->root, keys, values);
	}

	// Post-Order traversal
	// Left Subtree -> Right Subtree -> Node
	void postorder() {
		postOrderHelper(this->root);
	}

	// search the tree for the key k
	// and return the corresponding node
	NodePtr find(char k) {
		return searchTreeHelper(this->root, k);
	}

	// find the node with the minimum key
	NodePtr minimum(NodePtr node) {
		while (node->left != NIL) {
			node = node->left;
		}
		return node;
	}

	// find the node with the maximum key
	NodePtr maximum(NodePtr node) {
		while (node->right != NIL) {
			node = node->right;
		}
		return node;
	}

	// find the successor of a given node
	NodePtr successor(NodePtr x) {
		// if the right subtree is not null,
		// the successor is the leftmost node in the
		// right subtree
		if (x->right != NIL) {
			return minimum(x->right);
		}

		// else it is the lowest ancestor of x whose
		// left child is also an ancestor of x.
		NodePtr y = x->parent;
		while (y != NIL && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
	}

	// find the predecessor of a given node
	NodePtr predecessor(NodePtr x) {
		// if the left subtree is not null,
		// the predecessor is the rightmost node in the 
		// left subtree
		if (x->left != NIL) {
			return maximum(x->left);
		}

		NodePtr y = x->parent;
		while (y != NIL && x == y->left) {
			x = y;
			y = y->parent;
		}

		return y;
	}

	// rotate left at node x
	void leftRotate(NodePtr x) {
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != NIL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	// rotate right at node x
	void rightRotate(NodePtr x) {
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != NIL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}
		else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	// insert the key to the tree in its appropriate position
	// and fix the tree
	void insert(char key) {
		// Ordinary Binary Search Insertion
		NodePtr node = new Node;
		node->parent = nullptr;
		node->symb = key;
		node->value = 1;
		node->left = NIL;
		node->right = NIL;
		node->isRed = true; // new node must be red

		NodePtr items_parent = nullptr;
		NodePtr item = this->root;

		while (item != NIL) {
			items_parent = item;

			if (node->symb < item->symb) {
				item = item->left;
			}
			else if (node->symb == item->symb) {
				item->value++;
				return;
			}
			else {
				item = item->right;
			}
		}

		node->parent = items_parent;
		if (items_parent == nullptr) {
			root = node;
		}
		else if (node->symb < items_parent->symb) {
			items_parent->left = node;
		}
		else {
			items_parent->right = node;
		}

		// if new node is a root node, simply return
		if (node->parent == nullptr) {
			node->isRed = false;
			return;
		}

		// if the grandparent is null, simply return
		if (node->parent->parent == nullptr) {
			return;
		}

		// Fix the tree
		fixInsert(node);
	}

	NodePtr getRoot() {
		return this->root;
	}

	// delete the node from the tree
	void remove(char data) {
		deleteNodeHelper(this->root, data);
	}

	// print the tree structure on the screen
	void Print() {
		if (root != NIL) {
			printHelper(this->root, "", true);
		}
		else {
			try {
				if (root == NIL)
					throw "The tree is empty";
			}
			catch (const char* msg)
			{
				cout << msg << endl;
			}
		}

	}

	void clear()
	{
		if (root != NIL) {
			clearHelper(this->root);
			this->root = NIL;
		}
		else
		{
			try {
				if (root == NIL)
					throw "The tree is empty";
			}
			catch (const char* msg)
			{
				cout << msg << endl;

			}
		}

	}

	void clearHelper(NodePtr root)
	{
		if (root != NIL)
		{
			clearHelper(root->left);
			clearHelper(root->right);
			delete root;
		}


	}

	void show_codes() {
		inorder(true, false);
	}

	void get_values() {
		inorder(false, true);
	}

	void makinglistHelper(NodePtr node)
	{
		if (node != NIL) {
			sorted.insert_sorted(node->value, node->symb);
			makinglistHelper(node->left);
			makinglistHelper(node->right);
		}
	}

	void show_as_list() {
		sorted.print_to_console();
	}

	void encoding_tree() {

		if (root != NIL) {

			makinglistHelper(this->root);
			sorted.encoding(0, sorted.get_size() - 1);
			//now we need that info in the map
			for (int i = 0; i < sorted.get_size(); i++)
			{
				NodePtr cur;
				cur = find(sorted.get_pointer(i)->symb);
				for (int k = 0; k < sorted.get_pointer(i)->code.length(); k++) {
					if (sorted.get_pointer(i)->code[k] == '0')
						cur->code.enqueue(false);
					else
						cur->code.enqueue(true);
				}

			}
		}

		else {
			try {
				if (root == NIL)
					throw "The tree is empty";
			}
			catch (const char* msg)
			{
				cout << msg << endl;

			}
		}
			




	} 

	void show_encoded(string quote) {
		NodePtr node;
		for (int i = 0; i < quote.length(); i++) {
			node = find(quote[i]);
			node->code.show_queue();
		}
	}

	void compare_codes(string part, NodePtr node, bool& found, string& decoded) {
		if (node != NIL) {
			if (found == true)
				return;
			compare_codes(part, node->left, found, decoded);
			compare_codes(part, node->right, found, decoded);
			if (node->code.compare(part, found) == true) {
				found = true;
				decoded += node->symb;
				return;
				}
							
		}
	}

	void decoding(string line, string& decoded) {
		int starting_position = 0;
		string part;

		for (int i = 0; i < line.length(); i++) {
			part += line[i];
			bool found = false;
			compare_codes(part, root, found, decoded);
			if (found == true) {
				part.clear();
			}
				
		}

	}
};

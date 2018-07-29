#include <string>
#include <iostream>
#include <stdexcept>


class NotFoundException : public std::runtime_error
{
public:
	NotFoundException(std::string msg) : std::runtime_error{ msg } {}
};


struct Node
{
	std::string word;
	unsigned count;
	Node* left;
	Node* right;
};


class Tree
{
public:
	Tree();
	~Tree();

	Node* Insert(std::string word);
	Node* Find(std::string word);

	std::string ToString();

private:
	Node* insertNode(Node** node, std::string word);
	Node* find(Node*, std::string word);
	void deleteNodes(Node* node);

	std::string toString(Node* node, std::string& tree);

	Node* mRoot;
};


Tree::Tree()
:	mRoot{ nullptr }
{}


Tree::~Tree()
{
	deleteNodes(mRoot);
}


void Tree::deleteNodes(Node* node)
{
	if (node == nullptr)
	{
		return;
	}

	if (node->left != nullptr)
	{
		deleteNodes(node->left);
	}

	if (node->right != nullptr)
	{
		deleteNodes(node->right);
	}

	delete node;
}


/// Inserts a new word into a Tree of Nodes
Node* Tree::Insert(std::string word)
{
	return insertNode(&mRoot, word);
}


Node* Tree::insertNode(Node** pNode, std::string word)
{
	Node* node{ *pNode };

	if (node == nullptr)
	{
		*pNode = new Node{ word, 1, nullptr, nullptr };
		return *pNode;
	}

	if (node->word == word)
	{
		++(node->count);
		return node;
	}

	if (word < node->word)
	{
		return insertNode(&node->left, word);
	}
	else
	{
		return insertNode(&node->right, word);
	}
}


Node* Tree::Find(std::string word)
{
	return find(mRoot, word);
}


Node* Tree::find(Node* node, std::string word)
{
	if (node == nullptr)
	{
		throw NotFoundException{ "Node not found" };
	}

	if (node->word == word)
	{
		return node;
	}
	else if (node->word > word)
	{
		return find(node->left, word);
	}
	else
	{
		return find(node->right, word);
	}
}


std::string Tree::ToString()
{
	std::string tree{ "" };
	toString(mRoot, tree);
	return tree;
}


std::string Tree::toString(Node* node, std::string& tree)
{
	if (node != nullptr)
	{
		toString(node->left, tree);
		tree += node->word;
		toString(node->right, tree);
	}

	return tree;
}


int main()
{
	Tree tree{};
	tree.Insert("c");
	tree.Insert("a");
	tree.Insert("b");
	tree.Insert("c");
	tree.Insert("b");
	tree.Insert("a");
	tree.Insert("t");
	tree.Insert("j");

	std::cout << tree.ToString() << std::endl;

	Node* found{ tree.Find("b") };
	std::cout << "Node found: " << found->word << std::endl;

	try
	{
		tree.Find("z");
	}
	catch (const NotFoundException& e)
	{
		std::cerr << "Exception captured: " << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}
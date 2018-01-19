// Define an apply(v,f) that applies a function f to each element of v assumed to be a vector<Shape∗>.
// Test apply() with a variety of functions, function objects, and lambdas.
// Note that by capturing variables from a lambda or storing values in a function object,
// you can call Shape functions that takes arguments without having to have f() take explicit arguments.
// Hint: there is a variety of ways of specifying apply()’s argument types; experiment

#include <vector>
#include <functional>
#include <iostream>
#include <cstdlib>


class Shape
{
public:
	Shape(std::string type) : mType{ type } {}

	std::string& GetType() { return mType; }
	void SetType(std::string type) { mType = type; }

private:
	std::string mType;
};


void DegenerateShape(Shape* shape)
{
	shape->SetType("point");
}


void apply(std::vector<Shape*> v, std::function<void(Shape*)> f)
{
	for (auto& shape : v)
	{
		f(shape);
	}
}


int main(int argc, char** argv)
{
	std::vector<Shape*> shapes{};

	// Create shapes
	for (size_t i{ 0 }; i < 8; ++i)
	{
		shapes.push_back(new Shape{"Circle"});
	}

	apply(shapes, DegenerateShape);

	// Delete shapes
	for (auto& shape : shapes)
	{
		std::cout << "Shape: " << shape->GetType().c_str() << std::endl;
		delete shape;
	}

	return EXIT_SUCCESS;
}

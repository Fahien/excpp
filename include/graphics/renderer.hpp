#include "graphics/graphics.hpp"

namespace graphics
{

class Renderer
{
  public:
	Renderer();

	void add( const Point& p );
	void add( const std::vector<Point*>& points );

	VertexBuffers vbuffers;
};

}

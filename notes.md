[x] Graphics::draw function should take as parameter a collection of vertices to render as points or lines primitives.

[x] Another Graphics::draw function will take as parameter a mesh, or a collection of meshes, or a scene, to render as triangles primitives.

[] In order to optimize performances, these functions should not create vulkan resources needed to draw. These resources should be created upfront, or asynchronously and used when ready.

[] The draw functions will find vulkan resources associated with each argument to be rendered in their specific Renderer objects.

[] The renderer objects can be released when they are not needed anymore, for example when the current scene changes.

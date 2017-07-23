ECS Interface
-------------

`entID createEntity()`

Creates a new Entity and returns its ID as a userdata.

`deleteEntity(entID)`

Deletes the entity entID

`setComponent(entID, compName, val)`

Sets the Component compName of Entity entID to the userdata val.

`setFlag(entID, compName, val)`

Sets the flag compName of Entity entID to the boolean val.

`setParent(entID, parID)`

Sets the parent of Entity entID to the Entity parID.

Types
-----

`val Vec2Double(x, y)`

Creates a userdata representing a Vector<2, double>. The two arguments x and y are numbers.

`spr Sprite(def)`

Creates a new Sprite. The parameter def is an array, each element of which represents a texture. Each texture is a table with the following possible values:
* `filename`: a string containing the filename of the image.
* `uniform`: a string containing the name of the sampler2D uniform to use for the texture.
* `UVAttrib`: a string containing the name of the attribute to use for the UV coordinates.
* `tileW`: an integer representing the tile width in pixels if this is to be treated as a texture atlas.
* `tileH`: an integer representing the tile height in pixels if this is to be treated as a texture atlas.
* `tileX`: an integer representing the x coordinate in units of tiles.
* `tileY`: an integer representing the y coordinate in units of tiles.
* `x`: an integer representing the x coordinate of the subimage to use in pixels.
* `y`: an integer representing the y coordinate of the subimage to use in pixels.
* `w`: an integer representing the width of the subimage to use in pixels.
* `h`: an integer representing the height of the subimage to use in pixels.

The only required field is filename. If def contains any one of tileW, tileH, tileX, tileY it must contain all of them. Similarly, if a def contains any one of x, y, w, h it must contain all of them. Furthermore, these variables are mutually exclusive with the tile variables..

`shd Shader(vertSrc, fragSrc)`

Creates a new Shader. The paramaters vertSrc and fragSrc are strings containing the source of the vertex and fragment shaders resepctively. Returns a userdata representing the shader.

`font FontRenderer(src, firstChar, lastChar)`

Returns a new FontRenderer as a userdata.

`spec SpriteSpec(spr, dx, dy, shd)`

Returns a userdata representing a VisualSpec for drawing a Sprite spr with Shader shd. The two numbers dx and dy represent offsets from the position. The last argument may be omited in which case the default Shader will be used.

`spec RectangleSpec(dx, dy, w, h, lineW, color, prim)`

Returns a userdata representing a VisualSpec for drawing a rectangle. The last argument may be omited in which case the default PrimitiveRenderer will be used.

`spec RoundedRectangleSpec(dx, dy, w, h, r, lineW, color, prim)`

Returns a userdata representing a VisualSpec for drawing a rounded rectangle. The last argument may be omited in which case the default PrimitiveRenderer will be used.

`spec ElipseArcSpec(dx, dy, rx, ry, theta1, theta2, lineW, color, prim)`

Returns a userdata representing a VisualSpec for drawing an eliptical arc. The last argument may be omited in which case the default PrimitiveRenderer will be used.

`spec TriangleSpec(x1, y1, x2, y2, x3, y3, lineW, color, prim)`

Returns a userdata representing a VisualSpec for drawing a triangle. The last argument may be omited in which case the default PrimitiveRenderer will be used.

`spec LineSpec(x1, y1, x2, y2, lineW, color, prim)`

Returns a userdata representing a VisualSpec for drawing a line. The last argument may be omited in which case the default PrimitiveRenderer will be used.

Auxillary
---------

`print(message)`

Prints a string message to the console

`exit()`

Exits the game. This is a "soft" exit achieved by setting shouldExit to true.

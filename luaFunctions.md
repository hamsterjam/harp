ECS Interface
-------------

`entID createEntity()`

Creates a new Entity and returns its ID as an integer.

`setComponent(entID, compName, val)`

Sets the Component compName of Entity entID to the usertype val.

`setFlag(entID, compName, val)`

Sets the flag compName of Entity entID to the boolean val.

`setParent(entID, parID)`

Sets the parent of Entity entID to the Entity parID.

Types
-----

`spr newSprite(def)`

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

The only required field is filename. If def contains any one of tileW, tileH, tileX, tileY it must contain all of them. Similarly, if a def contains any one of x, y, w, h it must contain all of them. Furthermore, these variables are mutually exclusive with the tile variables

`shd newShader(vertSrc, fragSrc)`

Creates a new Shader. The paramaters vertSrc and fragSrc are strings containing the source of the vertex and fragment shaders resepctively. Returns a usertype representing the shader.

`val getVec2Double(x, y)`

Creates a usertype representing a Vector<2, double>. The two arguments x and y are numbers.

`shd getDefaultShader()`

Returns a usertype representing the default shader defined in globals.h.

`spec getSpriteSpec(shd, spr)`

Returns a usertype representing a VisualSpec for drawing a Sprite spr with Shader shd.

Auxillary
---------

`print(message)`

Prints a string message to the console

`exit()`

Exits the game. This is a "soft" exit achieved by setting shouldExit to true.

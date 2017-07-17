screenWidth  = 640
screenHeight = 480

defaultShader = {}
io.input("shader/default.vert"); defaultShader.vertSrc = io.read("a")
io.input("shader/default.frag"); defaultShader.fragSrc = io.read("a")

primShader = {}
io.input("shader/defaultPrim.vert"); primShader.vertSrc = io.read("a")
io.input("shader/defaultPrim.frag"); primShader.fragSrc = io.read("a")

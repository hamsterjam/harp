readFile = function(src)
    io.input(src)
    return io.read("a")
end

loadfile("script/harp.lua")()

screenWidth  = 640
screenHeight = 480

defaultShader = {
    vertSrc = readFile("shader/default.vert"),
    fragSrc = readFile("shader/default.frag")
}

primShader = {
    vertSrc = readFile("shader/defaultPrim.vert"),
    fragSrc = readFile("shader/defaultPrim.frag")
}

-- This is the kind of thing that you should do in external files...
-- It's probably best to keep the config file as "value, pair" as possible
init = (function()
    local sprMiku = {
        {
            filename = "res/test.png"
        }
    }

    local function makeMiku(x, y)
        local e = createEntity()
        local pos = Vec2({x, y})
        local vis = SpriteSpec(sprMiku, 0, 0)

        e:set(comp.position, pos)
        e:set(comp.visual, vis)

        return e
    end

    return function()
        sprMiku = Sprite(sprMiku)

        miku = makeMiku(200, 200)
    end
end)()

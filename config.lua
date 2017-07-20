readFile = function(src)
    io.input(src)
    return io.read("a")
end

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

function init()
    sprMiku = Sprite({
        {
            filename = "res/test.png",
        }
    })

    local function makeMiku(x, y)
        local e = createEntity()
        local pos = Vec2Double(x, y)
        local vis = getSpriteSpecDef(sprMiku, 0, 0)

        setComponent(e, comp.position, pos)
        setComponent(e, comp.visual, vis)

        return e
    end

    -- miku = makeMiku(200, 200)
end

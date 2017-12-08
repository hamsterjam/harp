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

function moveMiku(e, input)
    local walkMag = 300
    local jumpMag = 10000
    local acc = {0, 0}

    if e:has(comp.onSurface) then
        -- Surface Input stuff
        if input.up then acc[2] = jumpMag end
    else
        -- Air input stuff
        if input.down  then acc[2] = -walkMag end
        if input.left  then acc[1] = -walkMag end
        if input.right then acc[1] =  walkMag end

    end

    local currAcc
    if e:has(comp.acceleration) then
        currAcc = asVec2(e:get(comp.acceleration))
    else
        currAcc = {0, 0}
    end

    acc[1] = acc[1] + currAcc[1]
    acc[2] = acc[2] + currAcc[2]

    e:set(comp.acceleration, Vec2(acc))
end

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
        e:set(comp.velocity, Vec2{0, 0})
        e:set(comp.collider, BoxCollider{256, 256});
        e:set(comp.inputFunction, moveMiku)
        e:set(comp.visual, vis)

        return e
    end

    return function()
        sprMiku = Sprite(sprMiku)

        miku = makeMiku(200, 200)

        border = createEntity()
        border:setParent(miku)
        border:set(comp.visual, RectFillSpec(0, 0, 256, 256, hsvaToColor(180, 1, 1, 0.2)))
        border:set(comp.layer, 1)

        floor = createEntity()
        floor:setFlag(flag.static, true)
        floor:set(comp.position, Vec2{0, 0})
        floor:set(comp.collider, LineCollider{screenWidth, 100})
        floor:set(comp.visual, LineSpec(0, 0, screenWidth, 100, 1, rgbToColor(0, 0, 0)))

        wall = createEntity()
        wall:setFlag(flag.static, true)
        wall:set(comp.position, Vec2{50, screenHeight})
        wall:set(comp.collider, LineCollider{50, -screenHeight})
        wall:set(comp.visual, LineSpec(0, 0, 50, -screenHeight, 1, rgbToColor(0, 0, 0)))
    end
end)()

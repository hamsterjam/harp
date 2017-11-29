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
moveMiku = (function()
    local maxSpeed     = 100
    local acceleration = 200
    local omega        = 0.01

    return function(e, input)
        local tarVel = {0, 0};

        if input.up    then tarVel[2] = tarVel[2] + maxSpeed end
        if input.down  then tarVel[2] = tarVel[2] - maxSpeed end
        if input.left  then tarVel[1] = tarVel[1] - maxSpeed end
        if input.right then tarVel[1] = tarVel[1] + maxSpeed end

        local curVel
        if e:has(comp.velocity) then
            curVel = asVec2(e:get(comp.velocity))
        else
            curVel = {0, 0}
        end

        local acc = {0, 0}
        for i = 1, 2 do
            if math.abs(curVel[i] - tarVel[i]) < omega then
                curVel[i] = tarVel[i]
            elseif tarVel[i] > curVel[i] then
                acc[i] =  acceleration
            elseif tarVel[i] < curVel[i] then
                acc[i] = -acceleration
            end
        end

        e:set(comp.velocity, Vec2(curVel))
        e:set(comp.acceleration, Vec2(acc))
    end
end)()

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
        e:set(comp.inputFunction, moveMiku)
        e:set(comp.visual, vis)

        return e
    end

    return function()
        sprMiku = Sprite(sprMiku)

        miku = makeMiku(200, 200)
    end
end)()

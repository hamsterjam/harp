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
    local tarVel = {0, 0};

    if input.up    then tarVel[2] = tarVel[2] + 100 end
    if input.down  then tarVel[2] = tarVel[2] - 100 end
    if input.left  then tarVel[1] = tarVel[1] - 100 end
    if input.right then tarVel[1] = tarVel[1] + 100 end

    local curVel = asVec2(e:get(comp.velocity))

    local acc = {0, 0}
    for i = 1, 2 do
        if math.abs(curVel[i] - tarVel[i]) < 0.0001 then
            curVel[i] = tarVel[i]
        elseif tarVel[i] > curVel[i] then
            acc[i] =  200
        elseif tarVel[i] < curVel[i] then
            acc[i] = -200
        end
    end

    e:set(comp.velocity, Vec2(curVel))
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
        e:set(comp.inputFunc, moveMiku)
        e:set(comp.visual, vis)

        return e
    end

    return function()
        sprMiku = Sprite(sprMiku)

        miku = makeMiku(200, 200)
    end
end)()

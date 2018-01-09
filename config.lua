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
    local function move(e, input);
        if e:has(comp.onSurface) then
            -- Surface Input stuff
            if input.up then return action.jump end
            if input.left  then return action.walkleft end
            if input.right then return action.walkright end
        else
            -- Air input stuff
            if input.down  then return action.fastdrop end
        end

        -- If we haveen't returned an action by now, return noaction
        return action.noaction
    end

    local function animUpdate(ent, deltaT)
        local vel = {0, 0}
        if ent:has(comp.velocity) then
            vel = asVec2(ent:get(comp.velocity))
        end

        local x = vel[1]
        local y = vel[2]

        if y > x then
            if y > -x then
                -- UP
                return 2
            else
                -- LEFT
                return 3
            end
        else
            if y > -x then
                -- RIGHT
                return 0
            else
                -- DOWN
                return 1
            end
        end
    end

    local sprites = {
        {{
            filename = "res/arrow.png",
            tileW = 32,
            tileH = 32,
            tileX = 0,
            tileY = 0
        }}, {{
            filename = "res/arrow.png",
            tileW = 32,
            tileH = 32,
            tileX = 1,
            tileY = 0
        }}, {{
            filename = "res/arrow.png",
            tileW = 32,
            tileH = 32,
            tileX = 0,
            tileY = 1
        }}, {{
            filename = "res/arrow.png",
            tileW = 32,
            tileH = 32,
            tileX = 1,
            tileY = 1
        }}
    }
    local anim

    return function()
        for i = 1,#sprites do
            sprites[i] = Sprite(sprites[i]);
        end
        anim = AnimationManager(sprites, animUpdate)
        local visSpec = AnimationSpec(anim, 0, 0)

        arrow = createEntity()
        arrow:set(comp.position, Vec2{200, 200})
        arrow:set(comp.velocity, Vec2{0, 0})
        arrow:set(comp.collider, BoxCollider{32, 32})
        arrow:set(comp.inputFunction, move)
        arrow:set(comp.visual, visSpec)

        floor = createEntity()
        floor:setFlag(flag.static, true)
        floor:set(comp.position, Vec2{0, 0})
        floor:set(comp.collider, LineCollider{screenWidth, 0})
    end
end)()

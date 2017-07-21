
-- More VisualSpec getter functions

function RectangleFillSpec(...)
    local numArgs = #{...}
    if numArgs == 5 then
        local x, y, w, h, color = ...
        return RectangleSpec(x, y, w, h, 0, color)
    elseif numArgs == 6 then
        local prim, x, y, w, h, color = ...
        return RectangleSpec(prim, x, y, w, h, 0, color)
    else
        error("RectangleFill takes 5 or 6 arguments, found " .. #i .. ".")
    end
end

function RoundedRectangleFillSpec(...)
    local numArgs = #{...}
    if numArgs == 6 then
        local x, y, w, h, r, color = ...
        return RoundedRectangleSpec(x, y, w, h, r, 0, color)
    elseif numArgs == 7 then
        local prim, x, y, w, h, r, color = ...
        return RoundedRectangleSpec(prim, x, y, w, h, r, 0, color)
    end
end

-- Color helper functions

function rgbaToColor(r, g, b, a)
    local color = {}
    color.r = r
    color.g = g
    color.b = b
    color.a = a

    return color
end

function rgbToColor(r, g, b)
    return rgbaToColor(r, g, b, 1)
end

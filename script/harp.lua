
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
        error("RectangleFillSpec takes 5 or 6 arguments, found " .. numArgs .. ".")
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
    else
        error("RoundedRectangleFillSpec takes 6 or 7 arguments, found " .. numArgs .. ".")
    end
end

function CircleFillSpec(...)
    local numArgs = #{...}
    if numArgs == 4 then
        local x, y, r, color = ...
        return ElipseArcSpec(x, y, r, r, 0, 360, 0, color)
    elseif numArgs == 5 then
        local prim, x, y, r, color = ...
        return ElipseArcSpec(prim, x, y, r, r, 0, 360, 0, color)
    else
        error("CircleFillSpec takes 4 or 5 arguments, found " .. numArgs .. ".")
    end
end

function CircleSpec(...)
    local numArgs = #{...}
    if numArgs == 5 then
        local x, y, r, lineW, color = ...
        return ElipseArcSpec(x, y, r, r, 0, 360, lineW, color)
    elseif numArgs == 6 then
        local prim, x, y, r, lineW, color = ...
        return ElipseArcSpec(prim, x, y, r, r, 0, 360, lineW, color)
    else
        error("CircleSpec takes 5 or 6 arguments, found " .. numArgs .. ".")
    end
end

function SegmentSpec(...)
    local numArgs = #{...}
    if numArgs == 6 then
        local x, y, r, theta1, theta2, color = ...
        return ElipseArcSpec(x, y, r, r, theta1, theta2, 0, color)
    elseif numArgs == 7 then
        local prim, x, y, r, theta1, theta2, color = ...
        return ElipseArcSpec(prim, x, y, r, r, theta1, theta2, 0, color)
    else
        error("SegmentSpec takes 6 or 7 arguments, found " .. numArgs .. ".")
    end
end

function ArcSpec(...)
    local numArgs = #{...}
    if numArgs == 7 then
        local x, y, r, theta1, theta2, lineW, color = ...
        return ElipseArcSpec(x, y, r, r, theta1, theta2, lineW, color)
    elseif numArgs == 8 then
        local prim, x, y, r, theta1, theta2, lineW, color = ...
        return ElipseArcSpec(prim, x, y, r, r, theta1, theta2, lineW, color)
    else
        error("ArcSpec takes 7 or 8 arguments, found " .. numArgs .. ".")
    end
end

function ElipseFillSpec(...)
    local numArgs = #{...}
    if  numArgs == 5 then
        local x, y, rx, ry, color = ...
        return ElipseArcSpec(x, y, rx, ry, 0, 360, 0, color);
    elseif numArgs == 6 then
        local prim,x, y, rx, ry, color = ...
        return ElipseArcSpec(prim, x, y, rx, ry, 0, 360, 0, color);
    else
        error("ElipseFillSpec takes 5 or 6 arguments, found " .. numArgs .. ".")
    end
end

function ElipseSpec(...)
    local numArgs = #{...}
    if  numArgs == 6 then
        local x, y, rx, ry, lineW, color = ...
        return ElipseArcSpec(x, y, rx, ry, 0, 360, lineW, color);
    elseif numArgs == 7 then
        local prim,x, y, rx, ry, lineW, color = ...
        return ElipseArcSpec(prim, x, y, rx, ry, 0, 360, lineW, color);
    else
        error("ElipseSpec takes 6 or 7 arguments, found " .. numArgs .. ".")
    end
end

function ElipseSegmentSpec(...)
    local numArgs = #{...}
    if  numArgs == 7 then
        local x, y, rx, ry, theta1, theta2, color = ...
        return ElipseArcSpec(x, y, rx, ry, theta1, theta2, 0, color);
    elseif numArgs == 8 then
        local prim,x, y, rx, ry, theta1, theta2, color = ...
        return ElipseArcSpec(prim, x, y, rx, ry, theta1, theta2, 0, color);
    else
        error("ElipseFillSpec takes 5 or 6 arguments, found " .. numArgs .. ".")
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

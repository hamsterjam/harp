
-- More VisualSpec getter functions

function RectFillSpec(x, y, w, h, color, ...)
    return RectSpec(x, y, w, h, 0, color, ...)
end

function RoundedRectFillSpec(x, y, w, h, r, color, ...)
    return RoundedRectSpec(x, y, w, h, r, 0, color, ...)
end

function CircleFillSpec(x, y, r, color, ...)
    return ElipseArcSpec(x, y, r, r, 0, 360, 0, color)
end

function CircleSpec(x, y, r, lineW, color, ...)
    return ElipseArcSpec(x, y, r, r, 0, 360, lineW, color, ...)
end

function SegmentSpec(x, y, r, theta1, theta2, color, ...)
    return ElipseArcSpec(x, y, r, r, theta1, theta2, 0, color, ...)
end

function ArcSpec(x, y, r, theta1, theta2, lineW, color, ...)
    return ElipseArcSpec(x, y, r, r, theta1, theta2, lineW, color, ...)
end

function ElipseFillSpec(x, y, rx, ry, color, ...)
    return ElipseArcSpec(x, y, rx, ry, 0, 360, 0, color, ...)
end

function ElipseSpec(x, y, rx, ry, lineW, color, ...)
    return ElipseArcSpec(x, y, rx, ry, 0, 360, lineW, color, ...)
end

function ElipseSegmentSpec(x, y, rx, ry, theta1, theta2, color, ...)
    return ElipseSegmentSpec(x, y, rx, ry, theta1, theta1, 0, color, ...)
end

function TriangleFillSpec(x1, y1, x2, y2, x3, y3, color, ...)
    return TriangleSpec(x1, y1, x2, y2, x3, y3, 0, color, ...)
end

-- Color helper functions

(function()
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

    local function chromaToColor(hh, c, x, m, a)
        local ret = {
            r = 0,
            g = 0,
            b = 0,
            a = a
        }

        if hh < 1 then
            ret.r = c
            ret.g = x
            ret.b = 0
        elseif hh < 2 then
            ret.r = x
            ret.g = c
            ret.b = 0
        elseif hh < 3 then
            ret.r = 0
            ret.g = c
            ret.b = x
        elseif hh < 4 then
            ret.r = 0
            ret.g = x
            ret.b = c
        elseif hh < 5 then
            ret.r = x
            ret.g = 0
            ret.b = c
        elseif hh < 6 then
            ret.r = c
            ret.g = 0
            ret.b = x
        else
            ret.r = 0
            ret.g = 0
            ret.b = 0
        end

        ret.r = ret.r + m
        ret.g = ret.g + m
        ret.b = ret.b + m

        return ret
    end

    function hsvaToColor(h, s, v, a)
        h = h % 360
        local hh = h / 60
        local c  = v * s
        local x  = c * (1 - math.abs(hh % 2 - 1))
        local m  = v - c

        return chromaToColor(hh, c, x, m, a)
    end

    function hsvToColor(h, s, v)
        return hsvaToColor(h, s, v, 1.0)
    end

    function hslaToColor(h, s, l, a)
        h = h % 360
        local hh = h / 60
        local c  = (1 - math.abs(2*l - 1)) * s
        local x  = c * (1 - math.abs(hh % 2 - 1))
        local m  = l - c / 2

        return chromaToColor(hh, c, x, m, a)
    end

    function hslToColor(h, s, l)
        return hslaToColor(h, s, l, 1.0)
    end

end)()

-- Print
function print(msg)
    if type(msg) == "table" and #msg ~= 0 then
        local out = "["
        for i = 1, #msg do
            out = out .. msg[i] .. ", "
        end
        out = string.sub(out, 1, -3) -- Remove last 2 characters ", "
        out = out .. "]"
        printRaw(out)
    else
        printRaw(msg)
    end
end

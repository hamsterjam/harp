
-- More VisualSpec getter functions

function RectangleFillSpec(x, y, w, h, color, ...)
    return RectangleSpec(x, y, w, h, 0, color, ...)
end

function RoundedRectangleFillSpec(x, y, w, h, r, color, ...)
    return RoundedRectangleSpec(x, y, w, h, r, 0, color, ...)
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

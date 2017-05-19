

-- ±È½ÏÏñËØÖµ
function compareposvalue(ta1, ta2, ta3)
    local i = 1;
    while
        (i <= 5)
    do
        if (ta1[i] ~= nil) and(ta2[i] ~= nil) then
            if abs(ta1[i][1] - ta2[i][1]) > 8 or abs(ta1[i][2] - ta2[i][2]) > 8 or abs(ta1[i][3] - ta2[i][3]) > 8 then
                if (ta3 ~= nil) and(ta3[i] ~= nil) then
                    if abs(ta3[i][1] - ta2[i][1]) > 8 or abs(ta3[i][2] - ta2[i][2]) > 8 or abs(ta3[i][3] - ta2[i][3]) > 8 then
                        return false
                    end
                else
                    return false
                end
            end
        end
        i = i + 1
    end
    return true
end


function compareWindowPixel(windowid, value)
    return INF_getWindowPixelRGB(windowid, value)
end

-- function comparePixel(windowTitle, pos, value)
    -- local tb = { }
    -- Lua_ComparePixel(pos,tb, windowTitle)
    -- return compareposvalue(value, tb)
-- end


function comparePixel(windowTitle, pixel)
    Lua_ComparePixel(windowTitle,pixel)
    return compareposvalue(value, tb)
end
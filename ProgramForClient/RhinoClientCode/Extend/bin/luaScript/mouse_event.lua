
-- 鼠标点击
function mouseClick(hWnd, x, y, isAbs)
    Inf_MouseClickLButton(hWnd,x, y, isAbs)
    Sleep(100)
end

-- 鼠标双击
function mouseDoubleClick(hWnd, x, y, isAbs)
    Inf_MouseClickLButton(hWnd,x, y, isAbs)
    Inf_MouseClickLButton(hWnd,x, y, isAbs)
    Sleep(100)
end


        
-- 鼠标三击
function mouseThreeClick(hWnd, x, y, isAbs)
    Inf_MouseClickLButton(hWnd, x, y, isAbs)
    Inf_MouseClickLButton(hWnd, x, y, isAbs)
    Inf_MouseClickLButton(hWnd, x, y, isAbs)
    Sleep(100)
end


--function startup_hs()
--    print("启动炉石传说");
--end

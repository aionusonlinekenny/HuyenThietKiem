function IntDiv(total, req)
    total = tonumber(total) or 0
    req   = tonumber(req) or 1
    if req <= 0 then return 0 end
    local q = 0
    while total >= req do
        total = total - req
        q = q + 1
    end
    return q
end
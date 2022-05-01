local lni = require "lni"
local ffi = require "ffi"
local fs = require "bee.filesystem"
-- for key, value in pairs(fs) do
--     print(key, value)
-- end
local foreseeLoad = {}
-- 转换256进制整数
foreseeLoad.ids1 = {}
foreseeLoad.ids2 = {}
function foreseeLoad._id(a)
    local r = (">I4"):pack(a)
    foreseeLoad.ids1[a] = r
    foreseeLoad.ids2[r] = a
    return r
end

function foreseeLoad.id2string(a)
    return foreseeLoad.ids1[a] or foreseeLoad._id(a)
end

function foreseeLoad.__id2(a)
    local r = (">I4"):unpack(a)
    foreseeLoad.ids2[a] = r
    foreseeLoad.ids1[r] = a
    return r
end

function foreseeLoad.string2id(a)
    return foreseeLoad.ids2[a] or foreseeLoad.__id2(a)
end

local function file_save(file_path, content)
    local f, e = io.open(file_path, "wb")
    if f then
        f:write(content)
        f:close()
        return true
    else
        return false, e
    end
end

---切割空格
function table.splitSpace(str, reps)
    local resultStrList = {}
    string.gsub(
        str,
        "[^" .. reps .. "]+",
        function(w)
            table.insert(resultStrList, w)
        end
    )
    return resultStrList
end

local function seach_file(path, callback)
    --遍历文件目录
    for child in path:list_directory() do
        --如果是文件夹 则再进入一层
        if fs.is_directory(child) then
            seach_file(child, callback)
        else
            --否则 直接处理文件
            callback(child)
        end
    end
end
local path = arg[1]
local cut_num = arg[2]
if path == nil or cut_num == nil then
    return
end
local unit_obj = [[
[%s]
_parent = "opeo"
-- 名字
Name = "%s"
-- 模型文件
file = "%s"
]]
local txt = {}
-- for i = 1, 10, 1 do
--     txt = txt .. (unit_obj):format("n00" .. i, "test_" .. i)
-- end
local res = path .. "\\map\\resource"
local i = 0
seach_file(
    fs.path(res),
    function(chil)
        if chil:string():sub(-3) == "mdx" then
            local name=chil:filename():string()
            local ar = table.splitSpace(name, "_")
            local s = (unit_obj):format(ar[3]:gsub("%.mdx", ""), name:gsub("%.", "_"), name)
            table.insert(txt, s)
            i = i + 1
        end
    end
)
file_save(path .. "\\map\\table\\unit.ini", table.concat(txt))
for ii = 1, cut_num do
    fs.remove(fs.path(path .. "\\map\\map\\war3mapUnits.doo"))
    fs.rename(fs.path((path .. "\\map\\map\\war3mapUnits.doo%d"):format(ii)),fs.path(path .. "\\map\\map\\war3mapUnits.doo"))
    os.execute((fs.path(path) / string.format([[w3x2lni\w2l.exe obj %s %s]], path .. "/map", path .. ("/map%d.w3x"):format(ii))):string())
end

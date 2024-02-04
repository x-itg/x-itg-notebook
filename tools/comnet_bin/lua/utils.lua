--- 模块功能：常用工具类接口
-- @module utils
-- @author openLuat
-- @license MIT
-- @copyright openLuat
-- @release 2019.01.05


--- 将Lua字符串转成HEX字符串，如"123abc"转为"313233616263"
-- @string str 输入字符串
-- @string[opt=""] separator 输出的16进制字符串分隔符
-- @return hexstring 16进制组成的串
-- @return len 输入的字符串长度
-- @usage
-- string.toHex("\1\2\3") -> "010203" 3
-- string.toHex("123abc") -> "313233616263" 6
-- string.toHex("123abc"," ") -> "31 32 33 61 62 63 " 6
function string.toHex(str, separator)
    return str:gsub('.', function(c)
        return string.format("%02X" .. (separator or ""), string.byte(c))
    end)
end
--- 将HEX字符串转成Lua字符串，如"313233616263"转为"123abc", 函数里加入了过滤分隔符，可以过滤掉大部分分隔符（可参见正则表达式中\s和\p的范围）。
-- @string hex,16进制组成的串
-- @return charstring,字符组成的串
-- @return len,输出字符串的长度
-- @usage
-- string.fromHex("010203")       ->  "\1\2\3"
-- string.fromHex("313233616263") ->  "123abc"
function string.fromHex(hex)
    return hex:gsub("[%s%p]", ""):gsub("%x%x", function(c)
        return string.char(tonumber(c, 16))
    end)
end
--- 返回字符串tonumber的转义字符串(用来支持超过31位整数的转换)
-- @string str 输入字符串
-- @return str 转换后的lua 二进制字符串
-- @return len 转换了多少个字符
-- @usage
-- string.toValue("123456") -> "\1\2\3\4\5\6"  6
-- string.toValue("123abc") -> "\1\2\3\a\b\c"  6
function string.toValue(str)
    return string.fromHex(str:gsub("%x", "0%1"))
end
--- 返回utf8编码字符串的长度
-- @string str,utf8编码的字符串,支持中文
-- @return number,返回字符串长度
-- @usage local cnt = string.utf8Len("中国a"),cnt == 3
function string.utf8Len(str)
    local _, count = string.gsub(str, "[^\128-\193]", "")
    return count
end
--- 返回utf8编码字符串的单个utf8字符的table
-- @string str，utf8编码的字符串,支持中文
-- @return table,utf8字符串的table
-- @usage local t = string.utf8ToTable("中国2018")
function string.utf8ToTable(str)
    local tab = {}
    for uchar in string.gfind(str, "[%z\1-\127\194-\244][\128-\191]*") do
        tab[#tab + 1] = uchar
    end
    return tab
end
--- 返回字符串的 RFC3986 编码
-- @string str，要转换编码的字符串,支持UTF8编码中文
-- @return str, RFC3986 编码的字符串
-- @usage local str = string.rawurlEncode("####133") ,str == "%23%23%23%23133"
-- @usage local str = string.rawurlEncode("中国2018") , str == "%e4%b8%ad%e5%9b%bd2018"
function string.rawurlEncode(str)
    local t = str:utf8ToTable()
    for i = 1, #t do
        if #t[i] == 1 then
            t[i] = string.gsub(string.gsub(t[i], "([^%w_%~%.%- ])", function(c) return string.format("%%%02X", string.byte(c)) end), " ", "%%20")
        else
            t[i] = string.gsub(t[i], ".", function(c) return string.format("%%%02X", string.byte(c)) end)
        end
    end
    return table.concat(t)
end
--- 返回字符串的urlEncode编码
-- @string str，要转换编码的字符串,支持UTF8编码中文
-- @return str,urlEncode编码的字符串
-- @usage local str = string.urlEncode("####133") ,str == "%23%23%23%23133"
-- @usage local str = string.urlEncode("中国2018") , str == "%e4%b8%ad%e5%9b%bd2018"
function string.urlEncode(str)
    local t = str:utf8ToTable()
    for i = 1, #t do
        if #t[i] == 1 then
            t[i] = string.gsub(string.gsub(t[i], "([^%w_%*%.%- ])", function(c) return string.format("%%%02X", string.byte(c)) end), " ", "+")
        else
            t[i] = string.gsub(t[i], ".", function(c) return string.format("%%%02X", string.byte(c)) end)
        end
    end
    return table.concat(t)
end
--- 返回数字的千位符号格式
-- @number num,数字
-- @return string，千位符号的数字字符串
-- @usage loca s = string.formatNumberThousands(1000) ,s = "1,000"
function string.formatNumberThousands(num)
    local k, formatted
    formatted = tostring(tonumber(num))
    while true do
        formatted, k = string.gsub(formatted, "^(-?%d+)(%d%d%d)", '%1,%2')
        if k == 0 then break end
    end
    return formatted
end
--- 按照指定分隔符分割字符串
-- @string str 输入字符串
-- @param[opt=string,number,nil] delimiter 分隔符：
--      string: 字符串类型分隔符
--      number: 按长度分割
--      nil   ：相当于"", 将字符串转为字符数组
-- @return 分割后的字符串列表
-- @usage "123,456,789":split(',') -> {'123','456','789'}
function string.split(str, delimiter)
    local strlist, tmp = {}, string.byte(delimiter)
    if delimiter and delimiter == "" then
        for i = 1, #str do strlist[i] = str:sub(i, i) end
    elseif type(delimiter) == "number" then
        for i = 1, #str, delimiter do table.insert(strlist, str:sub(i, i + delimiter - 1)) end
    else
        for substr in string.gmatch(str .. delimiter, "(.-)" .. (((tmp > 96 and tmp < 123) or (tmp > 64 and tmp < 91) or (tmp > 47 and tmp < 58)) and delimiter or "%" .. delimiter)) do
            table.insert(strlist, substr)
        end
    end
    return strlist
end
-- 和校验
-- @string str 需要校验的字符串
-- @string number 1为返回1个字节，2为返回2个字节
-- @retrun 返回和校验结果
-- @usage string.checkSum("1234",1)
function string.checkSum(str, num)
    assert(type(str) == "string", "The first argument is not a string!")
    local sum = 0
    for i = 1, #str do
        sum = sum + str:sub(i, i):byte()
    end
    if num == 2 then
        return sum % 0x10000
    else
        return sum % 0x100
    end
end
--- 将lua 列化lua的基本类型为序列化,方便存储和传输
-- @param o: lua基本类型支持number，boolea，table,string。
-- @return string: 序列化后的字符串
-- @usage ：string.serialize("\\a")
-- @usage ：string .serialize(true)
-- @usage ：string .serialize(12345)
-- @usage ：string .serialize({"\a\b\\""})
function string.serialize(o)
    if type(o) == "string" then
        return string.format("%q", o)
    elseif type(o) == "table" then
        local str = "{"
        for k, v in pairs(o) do
            str = str .. " [" .. string.serialize(k) .. "] = " .. string.serialize(v) .. ","
        end
        return str .. "}"
    else
        return tostring(o)
    end
end
--- 将序列化的lua字符串反序列化为lua基本类型
-- @string str: 序列化后的基本类型字符串
-- @return param: 反序列化为 number or table or string or boolean
-- @usage local v = string.unSerialize("true") --> v为布尔值True
function string.unSerialize(str)
    return loadstring("return " .. str)()
end
--- 判断文件是否存在
-- @string path,文件全名例如："/ldata/call.mp3"
-- @return boole,存在为true,不存在为false
-- @usage local ex = io.exists("/ldata/call.mp3")
function io.exists(path)
    local file, err = io.open(path, "r")
    if file then
        file:close()
        return true
    else
        log.info("io.exists:", err)
        return false
    end
end
--- 读取文件并返回文件的内容
-- @string path,文件全名例如："/ldata/call.txt"
-- @return string,文件的内容,文件不存在返回nil
-- @usage local c = io.readFile("/ldata/call.txt")
function io.readFile(path)
    local file, str = io.open(path, "rb")
    if file then
        local content = file:read("*a")
        io.close(file)
        return content
    else
        log.error("io.readFile:", str)
    end
end
--- 写入文件指定的内容,默认为覆盖二进制模式
-- @string path,文件全名例如："/ldata/call.txt"
-- @string content,文件内容
-- @string mode,文件写入模式默认"w+b"
-- @return string,文件的内容
-- @retrun nil, 写入失败返回nil
-- @usage local c = io.writeFile("/ldata/call.txt","test")
function io.writeFile(path, content, mode)
    local file, str = io.open(path, mode or "w+b")
    if file then
        local res, err = file:write(content)
        file:close()
        if res == nil then
            log.error("io.writeFile:", err)
            return nil
        end
        return true
    else
        log.error("io.writeFile:", str)
    end
end
--- 将文件路径分解为table信息
-- @string path,文件路径全名例如:"/ldata/call.txt"
-- @return table,{dirname="/ldata/",filename="call.txt",basename="call",extname=".txt"}
-- @usage loca p = io.pathInfo("/ldata/call.txt")
function io.pathInfo(path)
    local pos = string.len(path)
    local extpos = pos + 1
    while pos > 0 do
        local b = string.byte(path, pos)
        if b == 46 then -- 46 = char "."
            extpos = pos
        elseif b == 47 then -- 47 = char "/"
            break
        end
        pos = pos - 1
    end
    local dirname = string.sub(path, 1, pos)
    local filename = string.sub(path, pos + 1)
    extpos = extpos - pos
    local basename = string.sub(filename, 1, extpos - 1)
    local extname = string.sub(filename, extpos)
    return {
        dirname = dirname,
        filename = filename,
        basename = basename,
        extname = extname
    }
end
--- 返回文件大小
-- @string path,文件路径全名例如:"/ldata/call.txt","test"
-- @return number ,文件大小
-- @return nil ,文件不存在
-- @usage locan cnt = io.fileSize("/ldata/call.txt")
function io.fileSize(path)
    local file, str = io.open(path, "r")
    if file then
        local current = file:seek()
        local size = file:seek("end")
        file:seek("set", current)
        file:close()
        return size
    else
        log.error("io.fileSize:", str)
    end
end
--- 返回指定位置读取的字符串
-- @string path,文件路径全名例如:"/ldata/call.txt"
-- @number offset,要读取的指定位置
-- @number len,要读取的字节数
-- @return string,返回要读取的数据,读取失败返回nil
function io.readStream(path, offset, len)
    local file, str = io.open(path, "r")
    if file then
        local current = file:seek()
        file:seek("set", offset)
        str = file:read(len)
        file:seek("set", current)
        file:close()
        return str
    else
        log.error("io.readStream:", str)
    end
end
--- 持久化序列化后的字符串
-- @param file: io.open 打开后的文件对象
-- @param o : 要求序列化的lua数据类型
-- @return nil
-- @usage io.serialize(io.open("/1.lua","w+b"),{1,2})
function io.serialize(file, o)
    if type(o) == "string" then
        file:write(string.format("%q", o))
    elseif type(o) == "table" then
        file:write("{\n")
        for k, v in pairs(o) do
            file:write(" ["); io.serialize(file, k); file:write("] = ")
            io.serialize(file, v); file:write(",\n")
        end
        file:write("}\n")
    else
        file:write(tostring(o))
    end
end
--- 返回一个迭代器函数,每次调用函数都会返回hash表的排序后的键值对
-- @table t, 要排序的hash表
-- @param f, 自定义排序函数
-- @return function.
-- @usage test = {a=1,f=9,d=2,c=8,b=5}
-- @usage for name,line in pairsByKeys(test) do print(name,line) end
function table.gsort(t, f)
    local a = {}
    for n in pairs(t) do a[#a + 1] = n end
    table.sort(a, f)
    local i = 0
    return function()
        i = i + 1
        return a[i], t[a[i]]
    end
end
--- table.concat的增强版，支持嵌套字符串数组
-- @table l,嵌套字符串数组
-- @return string
-- @usage  print(table.rconcat({"a",{" nice "}," and ", {{" long "},{" list "}}}))
function table.rconcat(l)
    if type(l) ~= "table" then return l end
    local res = {}
    for i = 1, #l do
        res[i] = rconcat(l[i])
    end
    return table.concat(res)
end
--- table.merge(...) 合并多个表格
-- @table[...],要合并的多个table
-- @return table,返回合并后的表格
-- @usage table.merge({1,2,3},{3, a = 4, b = 5, 6})
function table.merge(...)
    local tabs = {...}
    if #tabs == 0 then return {} end
    local origin = tabs[1]
    for i = 2, #tabs do
        if origin then
            if tabs[i] then
                for k, v in pairs(tabs[i]) do
                    if type(k) == "number" then
                        table.insert(origin, v)
                    else
                        origin[k] = v
                    end
                end
            end
        else
            origin = tabs[i]
        end
    end
    return origin
end
--- 处理表的url编码
-- @table query: 需要转码的查询表
-- @return string: 经过urlEncode转换后的字符串
-- @usage local q = table.urlEncode({a="1",b="2"})
function table.urlEncode(query)
    local msg = {}
    for k, v in pairs(query) do
        if type(k) == "number" then
            table.insert(msg, tostring(v):urlEncode())
        else
            table.insert(msg, k:urlEncode() .. "=" .. tostring(v):urlEncode())
        end
    end
    return table.concat(msg, "&")
end

-- 打印表
function table.print( t )
    local print_r_cache={}
    local function sub_print_r(t,indent)
        if (print_r_cache[tostring(t)]) then
            myprint(indent.."*"..tostring(t))
        else
            print_r_cache[tostring(t)]=true
            if (type(t)=="table") then
                for pos,val in pairs(t) do
                    if (type(val)=="table") then
                        myprint(indent.."["..pos.."] => "..tostring(t).." {")
                        sub_print_r(val,indent..string.rep(" ",string.len(pos)+8))
                        myprint(indent..string.rep(" ",string.len(pos)+6).."}")
                    elseif (type(val)=="string") then
                        myprint(indent.."["..pos..'] => "'..val..'"')
                    else
                        myprint(indent.."["..pos.."] => "..tostring(val))
                    end
                end
            else
                myprint(indent..tostring(t))
            end
        end
    end
    if (type(t)=="table") then
        myprint(tostring(t).." {")
        sub_print_r(t,"  ")
        myprint("}")
    else
        sub_print_r(t,"  ")
    end
    myprint()
end



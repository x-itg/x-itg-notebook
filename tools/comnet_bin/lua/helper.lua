-- �ļ���Ϊ module.lua
-- ����һ����Ϊ module ��ģ��
helper = {}
 
-- ����һ������
helper.constant = "����һ������"
 
-- ����һ������
function helper.func1()
    io.write("����һ�����к�����\n")
end
 
local function func2()
    print("����һ��˽�к�����")
end
 
function helper.func3()
    func2()
end
 
return helper


SetTargetFromWindow("TargetProcessWindowTitle") 
addr = 0x048E786 -- Target Address
new_v = 100 -- Health?

c_health = Read(addr)

if c_ health < new_v then
Write(addr, new_v)
end


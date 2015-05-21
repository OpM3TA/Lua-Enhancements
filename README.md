# Lua-Enhancements
Modifications done within Lua to allow simple Window Manipulation and 
Memory Access (read/write) within the Windows OS.

Example:
=====
-- SetTargetFromWindow sets the TARGET pid !and! returns it.
pid = SetTargetFromWindow("TargetProcessWindowTitle") 
addr = 0x048E786 -- Target Address
new_v = 100 -- Health?

c_health = Read(addr)

if c_health < new_v then
Write(addr, new_v)
end
=====


mod_b: begin
section text
val: extern
l1: extern
public y
public mod_b
output y
output val
output y + 2
jmp l1
section data
y: space 3
end

mod_a: begin
section text
r: extern
mod_b: extern
public a
public b
public l1
input a
input b
jmp mod_b
l1: output r + 1
stop
section data
a: space 1
b: space 1
end

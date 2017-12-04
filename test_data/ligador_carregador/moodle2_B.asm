mod_b: begin
section text
a: extern
b: extern
l1: extern
public r
public mod_b
load a
mult b
store r
div dois
store r + 1
jmp l1
section data
r: space 2
dois: const 2
end

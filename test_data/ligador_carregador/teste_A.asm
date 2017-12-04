mod_a: begin
section text
extern y
extern mod_b
public mod_a
public x
input x
input y
jmp mod_b
section data
x: space
end

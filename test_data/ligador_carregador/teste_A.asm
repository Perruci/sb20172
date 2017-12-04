mod_a: begin
section text
y: extern
mod_b: extern
public mod_a
public x
input x
input y
jmp mod_b
section data
x: space
end

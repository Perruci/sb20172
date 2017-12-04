mod_b: begin
section text
public mod_b
public y
x: extern
fim: extern
result: extern
load x
add y
mult dois
store result
jmp fim
section data
dois: const 2
y: space
end

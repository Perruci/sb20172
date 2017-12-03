mod_b: begin
section text
	fat: extern
	n: extern
	public mod_b
	store aux
	mult n
	store n
	load aux
	jmp fat
section data
	aux: space
end

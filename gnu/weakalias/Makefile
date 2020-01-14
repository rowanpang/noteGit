.PHONY: all weakundef
targets= weakdef weakundef extsym extsymwithweakdef

all:$(targets)

weakundef:main.c
	gcc main.c -o weakundef

weakdef:main.c
	gcc main.c -DWEAK_SYM -o weakdef

extsym:main.c extern.c
	gcc main.c extern.c -o extsym

extsymwithweakdef:main.c extern.c
	gcc main.c extern.c -DWEAK_SYM -o extsymwithweakdef

clean:
	@rm -rf $(targets)

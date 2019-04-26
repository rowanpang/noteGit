.PHONY:all

OUT=main 2 3 4

all:$(OUT)

%:%.c
	@gcc $< -ggdb -o $@ -lrt

.PHONY:clean
clean:
	@-rm $(OUT)

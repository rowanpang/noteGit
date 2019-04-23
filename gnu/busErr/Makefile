.PHONY:all

OUT=main 2

all:$(OUT)

%:%.c
	@gcc $< -ggdb -o $@ -lrt

.PHONY:clean
clean:
	@-rm $(OUT)

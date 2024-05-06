.PHONY:all
all: value

%:%.c
	@gcc -g $< -o $@
	@readelf --all --wide $@ > log.readelf
	@objdump --source --disassemble $@ > log.objdump

.PHONY:clean
clean:
	@rm log.* value

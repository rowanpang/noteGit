
.PHONY:all,run,clean
all:
	@gcc main.c -o cap_a
	@gcc main.c -o cap_cap_a
	@gcc main.c -o suid_a
	@gcc main.c -o a
	
	@sudo setcap "cap_dac_override+ep" cap_a
	@sudo setcap "cap_dac_override+ep cap_setuid+ep" cap_cap_a
	@sudo chown root:root suid_a
	@sudo chmod u+s suid_a

clean:
	@sudo rm cap_cap_a cap_a suid_a a

run:
	@echo "------------------"
	./cap_cap_a
	@echo "------------------"
	./cap_a
	@echo "------------------"
	./suid_a
	@echo
	@echo
	

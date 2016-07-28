.PHONY:sender
all:sender receiver

sender:sender.c
	@gcc $< -I/usr/include/dbus-1.0/ -I/usr/lib64/dbus-1.0/include/ -ldbus-1 -o $@

.PHONY:receiver
receiver:receiver.c
	@gcc $< -I/usr/include/dbus-1.0/ -I/usr/lib64/dbus-1.0/include/ -ldbus-1 -o $@

.PHONY:clean
clean:
	-@rm sender
	-@rm receiver


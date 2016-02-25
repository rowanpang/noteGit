#comment
#
#shell special args use '' or ""
#
#need root
#


.PHONY:all

ifeq ($(MAKECMDGOALS),)
    $(error need specify a make goal)
endif

.PHONY:pwdchk
pwdchk:
	gcc -o pwdchk pwdchk.c -lcrypt

.PHONY:gotDigest
gotDigest:
	gcc -o gotDigest gotDigest.c

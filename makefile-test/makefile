CC := gcc
CFLAGS := -lpthread


a:test.o
	$(CC) -o a test.o $(CFLAGS)
test.o:test.c
	@$(CC) -c test.c  ##  @符号加在前面make的时候就不会显示这行命令
	
	
###ifeq ("zeke","zeke2")
###	$(warning  dddddddddddd)
###else
###	$(info  qqqqqqqqqqqqq)
###endif
###
###$(shell echo "dsdsd">1)
###
###$(info $(shell cat 1))

text := $(subst o,k,goodmano)
$(info $(text))

text := $(findstring a,sdsasddsdsa)
$(info $(text))

	
clear:
	rm test.o a

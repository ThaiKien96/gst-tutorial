export BUILD_DIR = $(shell realpath build)

basic:
	@make -f basic/Makefile.basic

basic_clean:
	@make -f basic/Makefile.basic clean

.PHONY: basic basic_clean
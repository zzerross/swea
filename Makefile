CFLAGS := -Werror
CFLAGS += -std=c++14
CFLAGS += -fconcepts

P ?= 0
CFLAGS += -DPRINT=$(P)

I ?= 0
CFLAGS += -DINPUT=$(I)

# CFLAGS += -include debug.h

.PHONY: FORCE

define build
$(eval 
$(1): format FORCE
	g++ $(CFLAGS) -o $$@ $$@.cpp main.cpp
	time ./$$@ 2>&1 | tee $$@.txt
	@echo

TARGETS += $(1)
LOGS += $(1).txt
)
endef

$(call build,user)

format:
	find -name *.cpp -or -name *.h | xargs clang-format --style=google -i

clean:
	rm -rf $(TARGETS)

reset:
	find ! -name Makefile | xargs rm -rf

all: clean $(TARGETS)

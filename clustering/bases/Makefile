CFLAGS := -Werror
CFLAGS += -std=c++14

P ?= 0
CFLAGS += -DPRINT=$(P)

I ?= 0
CFLAGS += -DINPUT=$(I)

CFLAGS += -include debug.h

.PHONY: FORCE

define build
$(eval 
$(1): format FORCE
	g++ $(CFLAGS) -o $$@ $$@.cc main.cc
	time ./$$@ 2>&1 | tee $$@.txt
	@echo

TARGETS += $(1)
LOGS += $(1).txt
)
endef

$(call build,sort_stl)
$(call build,kmeans_sum)
$(call build,kmeans)
$(call build,assign)

format:
	clang-format --style=google -i *.cc *.h

clean:
	rm -rf $(TARGETS)

reset:
	find ! -name Makefile | xargs rm -rf

all: clean $(TARGETS)

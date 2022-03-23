CFLAGS := -Werror -Wall

D ?= 0
CFLAGS := -DkDEBUG=$(D)
CFLAGS += -include debug.h

I ?= in0

define build
$(eval 
$(2): FORCE $(1)
	clang-format --style=google -i $(3)
	g++ $(CFLAGS) -o $$@ $(3)
	time ./$(2) $(4) 2>&1 | tee $(2).txt
	@echo

OUTS += $(2) $(2).txt
)
endef

$(call build,,main,user.cc main.cc,)

clean:
	rm -rf $(OUTS)

.PHONY: FORCE

reset:
	find ! -name Makefile | xargs rm -rf

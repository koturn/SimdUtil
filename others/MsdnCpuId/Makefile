### This Makefile was written for GNU Make. ###
ifeq ($(DEBUG),true)
    OPT_CFLAGS := -O0 -g3 -ftrapv -fstack-protector-all -D_FORTIFY_SOURCE=2
    OPT_LDLIBS := -lssp
    ifneq ($(shell echo $$OSTYPE),cygwin)
        OPT_CFLAGS += -fsanitize=address -fno-omit-frame-pointer
        OPT_LDLIBS += -fsanitize=address
    endif
    OPT_CXXFLAGS := $(OPT_CFLAGS) -D_GLIBCXX_DEBUG
else
    ifeq ($(OPT),true)
        OPT_CFLAGS := -flto -Ofast -march=native -DNDEBUG
        OPT_CXXFLAGS := $(OPT_CFLAGS)
        OPT_LDFLAGS := -flto -s
    else
        ifeq ($(LTO),true)
            OPT_CFLAGS := -flto -DNDEBUG
            OPT_CXXFLAGS := $(OPT_CFLAGS)
            OPT_LDFLAGS := -flto
        else
            OPT_CFLAGS := -O3 -DNDEBUG
            OPT_CXXFLAGS := $(OPT_CFLAGS)
            OPT_LDFLAGS := -s
        endif
    endif
endif

ifeq ($(OMP),true)
    OPT_CFLAGS += -fopenmp
    OPT_CXXFLAGS += -fopenmp
    OPT_LDFLAGS += -fopenmp
else
    OPT_CFLAGS += -Wno-unknown-pragmas
    OPT_CXXFLAGS += -Wno-unknown-pragmas
endif

WARNING_COMMON_FLAGS := \
    -Wall \
    -Wextra \
    -Wabi \
    -Wcast-align \
    -Wcast-qual \
    -Wconversion \
    -Wdisabled-optimization \
    -Wdouble-promotion \
    -Wfloat-equal \
    -Wformat=2 \
    -Winit-self \
    -Winline \
    -Wlogical-op \
    -Wmissing-declarations \
    -Wno-return-local-addr \
    -Wpointer-arith \
    -Wredundant-decls \
    -Wstrict-aliasing=2 \
    -Wsuggest-attribute=const \
    -Wsuggest-attribute=format \
    -Wsuggest-attribute=noreturn \
    -Wsuggest-attribute=pure \
    -Wsuggest-final-methods \
    -Wsuggest-final-types \
    -Wswitch-enum \
    -Wundef \
    -Wunsafe-loop-optimizations \
    -Wunreachable-code \
    -Wvector-operation-performance \
    -Wwrite-strings \
    -pedantic

WARNING_CFLAGS := \
    $(WARNING_COMMON_FLAGS) \
    -Wc++-compat \
    -Wbad-function-cast \
    -Wjump-misses-init \
    -Wmissing-prototypes \
    -Wunsuffixed-float-constants

WARNING_CXXFLAGS := \
    $(WARNING_COMMON_FLAGS) \
    -Wc++11-compat \
    -Wc++14-compat \
    -Weffc++ \
    -Woverloaded-virtual \
    -Wsign-promo \
    -Wstrict-null-sentinel \
    -Wsuggest-override \
    -Wuseless-cast \
    -Wzero-as-null-pointer-constant

CC           := gcc $(if $(STDC),$(addprefix -std=,$(STDC)),-std=gnu11)
CXX          := g++ $(if $(STDCXX),$(addprefix -std=,$(STDCXX)),-std=gnu++14)
MKDIR        := mkdir -p
CP           := cp
RM           := rm -f
CTAGS        := ctags
DOXYGEN      := doxygen
DOXYFILE     := Doxyfile
DOXYGENDISTS := doxygen_sqlite3.db html/ latex/
# MACROS     := MACRO
# INCDIRS    := ./include
CPPFLAGS     := $(addprefix -D,$(MACROS)) $(addprefix -I,$(INCDIRS))
CFLAGS       := -pipe $(WARNING_CFLAGS) $(OPT_CFLAGS)
CXXFLAGS     := -pipe $(WARNING_CXXFLAGS) $(OPT_CXXFLAGS)
LDFLAGS      := -pipe $(OPT_LDFLAGS)
LDLIBS       := $(OPT_LDLIBS)
CTAGSFLAGS   := -R --languages=c,c++
TARGET       := main
SRCS         := $(addsuffix .cpp,$(basename $(TARGET)))
OBJS         := $(foreach PAT,%.cpp %.cxx %.cc,$(patsubst $(PAT),%.o,$(filter $(PAT),$(SRCS))))
DEPENDS      := depends.mk

ifeq ($(OS),Windows_NT)
    TARGET := $(addsuffix .exe,$(TARGET))
else
    TARGET := $(addsuffix .out,$(TARGET))
endif
INSTALLED_TARGET := $(if $(PREFIX),$(PREFIX),/usr/local)/bin/$(TARGET)

%.exe:
	$(CXX) $(LDFLAGS) $(filter %.c %.cpp %.cxx %.cc %.o,$^) $(LDLIBS) -o $@
%.out:
	$(CXX) $(LDFLAGS) $(filter %.c %.cpp %.cxx %.cc %.o,$^) $(LDLIBS) -o $@


.PHONY: all test depends asm syntax ctags doxygen install uninstall clean distclean
all: $(TARGET)
$(TARGET): $(OBJS)

# $(OBJS): $(SRCS)
# -include $(DEPENDS)
$(foreach SRC,$(SRCS),$(eval $(filter-out \,$(shell $(CXX) -MM $(SRC)))))

test: $(TARGET)
	@./$<

depends:
	$(CXX) -MM $(SRCS) > $(DEPENDS)

asm:
	$(CXX) $(SRCS) -S --verbose-asm $(CPPFLAGS) $(CXXFLAGS)

syntax:
	$(CXX) $(SRCS) -fsyntax-only $(CPPFLAGS) $(WARNING_CXXFLAGS)

ctags:
	$(CTAGS) $(CTAGSFLAGS)

doxygen: $(DOXYFILE)
	$(DOXYGEN) $<

$(DOXYFILE):
	$(DOXYGEN) -g $@

install: $(INSTALLED_TARGET)
$(INSTALLED_TARGET): $(TARGET)
	@[ ! -d $(@D) ] && $(MKDIR) $(@D) || :
	$(CP) $< $@

uninstall:
	$(RM) $(INSTALLED_TARGET)

clean:
	$(RM) $(OBJS)

distclean:
	$(RM) $(TARGET) $(OBJS) $(DOXYGENDISTS)

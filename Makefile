CFLAGS   += -g

# Be super strict about everything
CFLAGS   += -std=c99 -Werror -Wall -Wextra -pedantic -O2
CPPFLAGS += -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2

# Automatically sort out header dependencies
CPPFLAGS += -MD -MF $(patsubst %.o,.%.mk,$@) -MP
-include $(patsubst %.o,.%.mk,$(obj))

objs = \
  constants.o \
  base_util.o \
  core_functions.o \
  build_array.o \
  populate_each_function.o \
  tokenizer.o \
  process_defs.o \
  store_defs.o \
  parser.o \
  execute.o \
  huo.o

all: huo

huo: $(objs)

clean: ; rm -f -- .*.mk *.o

.PHONY: all clean
.DELETE_ON_ERROR:

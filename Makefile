CFLAGS   += -g
LIBS = -lpthread

# Be super strict about everything
CFLAGS   += -std=c99 -Werror -Wall -Wextra -pedantic -O2
CPPFLAGS += -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2

# Automatically sort out header dependencies
CPPFLAGS += -MD -MF $(patsubst %.o,.%.mk,$@) -MP
-include $(patsubst %.o,.%.mk,$(obj))

CPPFLAGS += -MD -MF $(patsubst execution_functions/%.o,.%.mk,$@) -MP
-include $(patsubst %.o,.%.mk,$(obj))


objs = \
  constants.o \
  base_util.o \
  core_functions.o \
  apply_core_function.o \
  execution_functions/populate_each_function.o \
  execution_functions/populate_reduce_function.o \
  execution_functions/parallel_execution.o\
  execution_functions/for_each.o\
  execution_functions/for_loop.o\
  execution_functions/map_array.o\
  execution_functions/reduce.o\
  execution_functions/read_file.o\
  execution_functions/let_binding.o\
  execution_functions/reduce_ast.o\
  execution_functions/if_block.o\
  build_array.o \
  tokenizer.o \
  process_defs.o \
  store_defs.o \
  parser.o \
  execute.o \
  huo.o

all: huo

huo: $(objs)
	cc -o huo $(objs) $(LIBS)

clean: ; rm -f -- .*.mk *.o & rm -f & rm -f ./execution_functions/*.o

.PHONY: all clean
.DELETE_ON_ERROR:

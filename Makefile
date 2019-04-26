compiler = g++
flags = -Wall -Wextra -Wpedantic -Werror -O2
libs = sqlite3

build_dir = build
debug_dir = debug
source_dir = src
object_dir = obj
include_dir = include

_objects = main.o politician.o data_base.o exceptions.o system.o
objects = $(patsubst %, $(object_dir)/%, $(_objects))

_dependencies = data_base.h exceptions.h politician.h system.h
dependencies = $(patsubst %, $(include_dir)/%, $(_dependencies))

executable = politician

.SECONDARY: $(objects)

.PHONY: all
all: $(build_dir)/$(executable) | $(build_dir)

.PHONY: run
run: all
	./$(build_dir)/$(executable)

.PHONY: debug
debug: flags += -g
debug: $(debug_dir)/$(executable) | $(debug_dir)/

%/$(executable): $(objects)
	$(compiler) $(flags) -l $(libs) $^ -o $@

$(object_dir)/%.o: $(source_dir)/%.cpp $(dependencies) | $(object_dir)/
	$(compiler) $(flags) -I $(include_dir) -c $< -o $@

%/: 
	mkdir -p $@

.PHONY: clean
clean:
	rm $(object_dir)/*.o

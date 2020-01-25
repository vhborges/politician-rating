compiler := g++
flags := -Wall -Wextra -Wpedantic -Wconversion -std=c++17
libs := -lsqlite3 -lboost_locale

include_dir := include
build_dir := build
debug_dir := debug
source_dir := src
object_dir := obj
build_obj_dir := $(build_dir)/$(object_dir)
debug_obj_dir := $(debug_dir)/$(object_dir)

objects := main.o politician.o database.o exceptions.o input.o filesystem.o
build_objects := $(patsubst %, $(build_obj_dir)/%, $(objects))
debug_objects := $(patsubst %, $(debug_obj_dir)/%, $(objects))

dependencies := database.hpp exceptions.hpp politician.hpp input.hpp CLI11.hpp filesystem.hpp
dependencies := $(patsubst %, $(include_dir)/%, $(dependencies))

executable := politician

####### BUILD rules #######
.PHONY: all
all: flags += -O2 -march=native
all: $(build_dir)/$(executable) | $(build_dir)/

$(build_dir)/$(executable): $(build_objects)
	$(compiler) $(flags) $(libs) $^ -o $@

$(build_obj_dir)/%.o: $(source_dir)/%.cpp $(dependencies) | $(build_obj_dir)/
	$(compiler) $(flags) -I $(include_dir) -c $< -o $@
###########################

####### DEBUG rules #######
.PHONY: debug
debug: flags += -Og -g
debug: $(debug_dir)/$(executable) | $(debug_dir)/

$(debug_dir)/$(executable): $(debug_objects)
	$(compiler) $(flags) $(libs) $^ -o $@

$(debug_obj_dir)/%.o: $(source_dir)/%.cpp $(dependencies) | $(debug_obj_dir)/
	$(compiler) $(flags) -I $(include_dir) -c $< -o $@
###########################

%/:
	mkdir -p $@

.PHONY: clean
clean:
	rm -f $(build_objects) $(debug_objects)

.PHONY: clean-all
clean-all:
	rm -f $(build_objects) $(debug_objects) $(build_dir)/$(executable) $(debug_dir)/$(executable)

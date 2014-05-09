# Makefile for MD5 Loader Demo, by A.J. Tavakoli
src_files := $(wildcard ./*.cpp)
inc_dir:=./
out := ./build
lib := -lGL -lGLU -lGLEW -lglut

$(shell   mkdir -p $(out))

MD5Demo: 
	g++ -std=c++0x $(src_files) -I$(inc_dir) $(lib) -o $(out)/MD5Demo

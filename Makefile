# Builds the regular expression filter highlighter 'hi'
# Author: Ross Rogers, 2009.08.20


all:
	g++ \
        -Wall \
        hi.cpp \
        -lboost_regex \
        -lboost_program_options \
        -o hi
        

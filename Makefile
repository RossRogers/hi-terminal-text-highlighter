# Builds the regular expression filter highlighter 'hi'
# Author: Ross Rogers, 2009.08.20


all:
	g++ -lboost_regex-mt \
        -lboost_program_options-mt \
        -Wall \
        hi.cpp \
        -o hi
        

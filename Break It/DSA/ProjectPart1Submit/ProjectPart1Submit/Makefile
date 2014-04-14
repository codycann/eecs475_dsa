#
# For CAEN
#

PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64

#
# These are the variables you need to edit when files are added / removed
#
# PROGRAM_SOURCE is the name of the .cpp file holding the main method (including the .cpp suffix)
# INTERMEDIATE_FOLDER is the name of the folder to contain the build files (do not include the ending /).
#

PROGRAM_SOURCE := main.cpp 
LIBRARY_SOURCE := sha256.cpp dsa.cpp 
TEST_PROGRAM_FILES :=
# test2.txt, etc...
INTERMEDIATE_FOLDER := build

#
# OS specific suffixes, prefixes, and switches used by make
#
# FOR OS/X: CPP=.cpp, OBJ=.o, EXE=<blank>, DEP=.d,
#

CPP := .cpp
OBJ := .o
EXE :=
DEP := .d
G++ := g++ -lgmpxx -lgmp -Wall -Werror --std=c++11

#
# Variables used internally by make
#

INTERMEDIATE_FOLDER := $(addsuffix /,$(INTERMEDIATE_FOLDER))

PROGRAM_OBJECT := $(addprefix $(INTERMEDIATE_FOLDER),$(PROGRAM_SOURCE:$(CPP)=$(OBJ)))
LIBRARY_OBJECTS := $(addprefix $(INTERMEDIATE_FOLDER),$(LIBRARY_SOURCE:$(CPP)=$(OBJ)))
PROGRAM_BINARY := $(PROGRAM_OBJECT:$(OBJ)=$(EXE))

PROGRAM_DEPEND := $(PROGRAM_OBJECT:$(OBJ)=$(DEP))
LIBRARY_DEPEND := $(LIBRARY_OBJECTS:$(OBJ)=$(DEP))

# end of variables - start of targets

.PHONY: all clean test submit
.PRECIOUS: $(LIBRARY_OBJECTS)

all: $(PROGRAM_BINARY)

clean: 
	rm -f $(PROGRAM_OBJECT) $(PROGRAM_BINARY) $(LIBRARY_OBJECTS)
	rm -f $(INTERMEDIATE_FOLDER)*.d

test: all
	@for progs in $(PROGRAM_BINARY); do \
        for script in $(TEST_PROGRAM_FILES); do \
            $$progs < $$script > $$script.result ; \
            diff $$script.result $$script.answer ; \
            echo "-----" ; \
        done \
	done

submit: clean
	#dos2unix -U *
	tar czf ./submit.tar.gz *.cpp *.h Makefile test-*.txt

$(INTERMEDIATE_FOLDER)%$(OBJ): %$(CPP)
	$(G++) -Wall $(COVERAGE) -c $< $(CPP_INCDIR) -o $@
	@$(G++) -MM $< $(CPP_INCDIR) > $(@:$(OBJ)=$(DEP))
	@mv -f $(@:$(OBJ)=$(DEP)) $(@:$(OBJ)=$(DEP).tmp)
	@sed -e 's|.*:|$(@):|' < $(@:$(OBJ)=$(DEP).tmp) > $(@:$(OBJ)=$(DEP))
	@sed -e 's/.*://' -e 's/\\$$//' < $(@:$(OBJ)=$(DEP).tmp) | fmt -1 | \
  		sed -e 's/^ *//' -e 's/$$/:/' >> $(@:$(OBJ)=$(DEP))
	@rm -f $(@:$(OBJ)=$(DEP).tmp)

$(PROGRAM_BINARY): % : %$(OBJ)
	$(G++) -Wall $(<) $(CPP_INCDIR) $(LIBRARY_OBJECTS) -o $(@) $(CPP_LIBDIR) -L$(INTERMEDIATE_FOLDER)
	@$(G++) -MM $(CPP_INCDIR) $(notdir $(<:$(OBJ)=$(CPP))) > $(<:$(OBJ)=$(DEP))
	@mv $(<:$(OBJ)=$(DEP)) $(<:$(OBJ)=.tmp)
	@sed "s|^.*:|$(@):|" $(<:$(OBJ)=.tmp) > $(<:$(OBJ)=$(DEP))
	@rm $(<:$(OBJ)=.tmp)
    
$(PROGRAM_BINARY): $(LIBRARY_OBJECTS)

-include $(PROGRAM_DEPEND) $(LIBRARY_DEPEND)

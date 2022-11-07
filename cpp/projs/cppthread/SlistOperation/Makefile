# project name (generate executable with this name)
TARGET   = Obj

# change these to set the proper directories where each files should be
SRCDIR   = src
HDRDIR   = hdr
OBJDIR   = obj
BINDIR   = bin

CC       = g++
# compiling flags here
CFLAGS   = -g -Wall -I. -I$(HDRDIR)

LINKER   = g++ -g -o
# linking flags here
LFLAGS   = -Wall -I. -lm

LDFLAGS  = -pthread

SOURCES  := $(wildcard $(SRCDIR)/*.cc)
INCLUDES := $(wildcard $(HDRDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cc=$(OBJDIR)/%.o)
rm        = rm -f


$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(LFLAGS) $(LDFLAGS) $(OBJECTS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONEY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONEY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"

.PHONEY: count
count:
	find . -type f -name "*.h"|xargs wc -l
	find . -type f -name "*.cpp"|xargs wc -l
	find . -type f -name "*.cc"|xargs wc -l
	@echo "Lines of code counted!"
 

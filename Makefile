CC := g++
SRCDIR := src
BUILDDIR := build
TARGET1 := bin/generate_random
TARGET2 := bin/viewer

# Custom library locations
LIBHAND_SRCDIR := /home/alex/dev/projects/libhand-public/hand_cpp/source/
LIBHAND_LIBDIR := /home/alex/dev/projects/libhand-public/hand_cpp/build/source/

SRCEXT := cpp
# SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
TARGET1_SOURCES := $(SRCDIR)/PoseGenerator.cpp $(SRCDIR)/generate_random.$(SRCEXT)
TARGET2_SOURCES := $(SRCDIR)/PoseGenerator.cpp $(SRCDIR)/viewer.$(SRCEXT)
TARGET1_OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(TARGET1_SOURCES:.$(SRCEXT)=.o))
TARGET2_OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(TARGET2_SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -std=c++11
LIBHAND_INCLUDE := -I$(LIBHAND_SRCDIR)
LIBHAND_LIBRARIES := -L$(LIBHAND_LIBDIR) -Wl,-R$(LIBHAND_LIBDIR) -lhand_renderer -lhand_utils
# LIBHAND_LIBRARIES := -L$(LIBHAND_LIBDIR) -l:libhand_renderer.a -l:libhand_utils.a
# LIBHAND_LIBRARIES := $(LIBHAND_LIBDIR)/libhand_renderer.a $(LIBHAND_LIBDIR)/libhand_utils.a
OGRE_INCLUDES := -I/usr/include/OGRE/ -I/usr/include/OGRE/Plugins/OctreeSceneManager/ -I/usr/include/OGRE/RenderSystems/GL/ -I/usr/include/OGRE/Plugins/OctreeZone/
# OGRE_LIBRARIES := /usr/lib/x86_64-linux-gnu/OGRE-1.9.0/Plugin_OctreeSceneManager.so /usr/lib/x86_64-linux-gnu/OGRE-1.9.0/Plugin_OctreeZone.so
OGRE_LIBRARIES := -L/usr/lib/x86_64-linux-gnu/OGRE-1.9.0 -Wl,-R/usr/lib/x86_64-linux-gnu/OGRE-1.9.0 /usr/lib/x86_64-linux-gnu/OGRE-1.9.0/Plugin_OctreeSceneManager.so /usr/lib/x86_64-linux-gnu/OGRE-1.9.0/Plugin_OctreeZone.so
LIB := -pthread -Llib -ltiff -lboost_filesystem -lboost_system -lOgreMain $(OGRE_LIBRARIES) $(LIBHAND_LIBRARIES) -lGL -lGLU -lglut `pkg-config opencv --cflags --libs`
INC := -Iinclude $(OGRE_INCLUDES) $(LIBHAND_INCLUDE)

$(TARGET1): $(TARGET1_OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET1) $(LIB)"; $(CC) $^ -o $(TARGET1) $(LIB)

$(TARGET2): $(TARGET2_OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET2) $(LIB)"; $(CC) $^ -o $(TARGET2) $(LIB)

all: $(TARGET1) $(TARGET2)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET1) $(TARGET2)"; $(RM) -r $(BUILDDIR) $(TARGET1) $(TARGET2)

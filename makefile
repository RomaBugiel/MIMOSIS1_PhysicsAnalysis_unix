
CXX=`root-config --cxx`
CXXFLAGS=`root-config --cflags`
#
LDFLAGS=`root-config --ldflags`

LDLIBS = `root-config --glibs` 

SOURCES=  MIMOSIS1_TreeReader.cpp RunControl.cpp HistoPlotter.cpp Fitter.cpp Matrix.cpp Pixel.cpp BadPixelFinder.cpp

HEADERS=/globals.h /cout_msg.h
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=mimosis1analyser

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) 
	$(CXX) $(CXXFLAGS) -O2 -W $(INCLUDES) -o $@ $^ $(LDFLAGS) $(LDLIBS) 

#.cc.o:
	# $(CXX) $(CXXFLAGS) -W -Wall -c $<

clean:
	rm ./*~ ./*.o ./*.so ./mimosis1analyser ./AutoDict* ./*.pcm
	
docs:
	doxygen Doxyfile
	

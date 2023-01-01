BUILDDIR = build
all: 
	rm -rf $(BUILDDIR)
	mkdir $(BUILDDIR)
	g++ $(PWD)/*.cpp -o $(BUILDDIR)/Project3
clean:
	rm -rf $(BUILDDIR)
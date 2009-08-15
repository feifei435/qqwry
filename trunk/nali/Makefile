include config.mak
all:nali.c libqqwry/qqwry.c
	gcc -o bin/qqwrynali nali.c libqqwry/qqwry.c
install:
	install bin/qqwrynali $(DESTDIR)$(bindir) 
	install bin/nali $(DESTDIR)$(bindir) 
	install bin/nali-traceroute $(DESTDIR)$(bindir) 
	install bin/nali-dig $(DESTDIR)$(bindir) 
	install bin/nali-ping $(DESTDIR)$(bindir) 
	install share/QQWry.Dat $(DESTDIR)$(datadir) 
	install share/nali.pl $(DESTDIR)$(datadir) 
distclean: clean
	rm -f config.mak
	rm -f config.h
clean:
	rm -f bin/qqwrynali
	rm -f bin/nali

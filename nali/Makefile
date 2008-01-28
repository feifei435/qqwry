include config.mak
all:nali.c libs/qqwry.c
	gcc -o nali nali.c libs/qqwry.c
install:nali QQWry.Dat
	install nali $(DESTDIR)$(bindir) 
	install QQWry.Dat $(DESTDIR)$(bindir) 
clean:
	rm -f nali


CFLAGS = -I.. -I../FxGenLib -w -m32
LDFLAGS = -Wb,--as-cmd='as --32',--ld-cmd='ld -melf_i386'

.cpp.o:
	wineg++ $(CFLAGS) -c "$?"

OBJS = Dialog.o EventsMgr.o GUI.o pch.o SplitWnd.o Types.o

all: libgui.a

libgui.a: $(OBJS)
	ar cru libgui.a $(OBJS)

clean: 
	rm -f *.so *.o




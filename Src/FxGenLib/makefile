
CFLAGS = -I.. -w -m32
LDFLAGS = -Wb,--as-cmd='as --32',--ld-cmd='ld -melf_i386'

.cpp.o:
	wineg++ $(CFLAGS) -c "$?"

OBJS = Archive.o Bitmap.o CombineOps.o Controllers.o Core.o DistordOps.o \
    EngineOp.o FilterOps.o MainOps.o Maths.o pch.o RenderOps.o Stream.o

all: libfxgen.a

libfxgen.a: $(OBJS)
	ar cru libfxgen.a $(OBJS)

clean: 
	rm -f *.so *.o

CFLAGS = -I.. -I../FxGenLib -I../GUILib -w -m32
LDFLAGS = -Wb,--as-cmd='as --32',--ld-cmd='ld -melf_i386' ../FxGenLib/libfxgen.a ../GUILib/libgui.a
#LDFLAGS = -melf_i386 ../FxGenLib/libfxgen.a ../GUILib/libgui.a
#LDFLAGS = -melf_i386

.cpp.o:
	wineg++ $(CFLAGS) -c "$?"

OBJS = Editor.o EnterNameDlg.o MainFrm.o MenuCtrl.o OperatorsCtrl.o \
OperatorsWnd.o pch.o ProjectWnd.o PropertiesCtrl.o PropertiesWnd.o \
PropertyItems.o Renderer.o TGAWriter.o TreeNodeCtrl.o ViewportsCtrl.o \
ViewportsWnd.o


all: editor

editor: $(OBJS)
	g++ -o editor $(LDFLAGS) $(OBJS)
	chmod +x editor
	strip editor

clean: 
	rm -f *.so *.o

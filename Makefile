CC=gcc
CFLAGS=-std=c99
LIBFLAGS=`pkg-config --cflags --libs gtkmm-3.0`

SRCDIR=src
SOURCES:= $(wildcard $(SRCDIR)/*.c)


OBJDIR=objs
OBJS:=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

TARGET=example

$(TARGET): $(OBJS)
	@echo intentando crear ejecutable

	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBFLAGS)

	@echo exit succes

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c | $(OBJDIR)/
	@echo intentando crear objetos a partir de las fuentes
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBFLAGS)
	@echo exito creando los objetos

$(OBJDIR)/:
	@echo creando la carpeta de objetos
	mkdir -p $(OBJDIR)
	@echo exito creando la carpeta de objetos

clean:
	rm -f $(TARGET)
	rm -r $(OBJDIR)
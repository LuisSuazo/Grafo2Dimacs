#DIRECTORIOS
dir_guard=@mkdir -p $(@D)
ODIR = obj
SDIR = src
BDIR = bin
IH= -Iheaders
Valgrind= valgrind


#REGLAS EXPLICITAS
CC = g++-7
OBJ = $(ODIR)/main.o
CFLAGS = -O3 -std=c++17 -Wall -pedantic -fopenmp
#BINARIO
TARGET=main

$(BDIR)/$(TARGET):$(OBJ)
	$(dir_guard)
	$(CC) -o $@ $^ $(IH) $(CFLAGS) 
	chmod 700 $(BDIR)/$(TARGET)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(dir_guard)
	#clear
	$(CC) -c $(IH) -o $@ $< $(CFLAGS) 

clean:
	rm -rf $(ODIR) 
	rm -rf $(BDIR)
	rm -rf Output

#Input=input/1_20_8x8
Input=Input/grafo.csv


run:
	rm -rf Output
	mkdir -p Output/Aristas
	mkdir -p Output/Nodos
	mkdir -p Output/Dataset

	./$(BDIR)/$(TARGET) $(Input) Output/Nodos/ Output/Aristas/ Output/Dataset/ Output/



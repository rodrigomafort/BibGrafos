CPPFLAGS = -w -std=gnu++0x -O3 -mfpmath=sse -march=native -g -m64 -fexceptions -ftree-vectorize -ftree-vectorize
COMP = g++

TARGET = libBibGrafos.a
OBJ = BibGrafos.o Vertice.o ArvoreCliques.o ListaAdjacencia.o Grafo.o VetorRequisitos.o Intervalo.o GrafoIntervalo.o GrafoIntervaloProprio.o GrafoSplitIndiferenca.o Grafo2Cliques.o Grafo3Cliques.o GrafoCordal.o GrafoDominoIntervaloProprio.o  

all: $(TARGET)

$(TARGET): $(OBJ)
	ar rcs $@ $(OBJ)

Vertice.o: Vertice.cpp Vertice.h
	$(COMP) $(CPPFLAGS) -c $< 

ArvoreCliques.o: ArvoreCliques.cpp ArvoreCliques.h Vertice.o Grafo.o
	$(COMP) $(CPPFLAGS) -c $<	

ListaAdjacencia.o: ListaAdjacencia.cpp ListaAdjacencia.h Vertice.o
	$(COMP) $(CPPFLAGS) -c $<	

Grafo.o: Grafo.cpp Grafo.h ListaAdjacencia.o Vertice.o
	$(COMP) $(CPPFLAGS) -c $<	

VetorRequisitos.o: VetorRequisitos.cpp VetorRequisitos.h Vertice.o Grafo.o
	$(COMP) $(CPPFLAGS) -c $<	

Intervalo.o: Intervalo.cpp Intervalo.h
	$(COMP) $(CPPFLAGS) -c $<	

GrafoIntervalo.o: GrafoIntervalo.cpp GrafoIntervalo.h Grafo.o Vertice.o Intervalo.o 
	$(COMP) $(CPPFLAGS) -c $<	

GrafoIntervaloProprio.o: GrafoIntervaloProprio.cpp GrafoIntervaloProprio.h GrafoIntervalo.o Grafo.o Vertice.o Intervalo.o
	$(COMP) $(CPPFLAGS) -c $<	

GrafoSplitIndiferenca.o: GrafoSplitIndiferenca.cpp GrafoSplitIndiferenca.h Grafo.o Vertice.o 
	$(COMP) $(CPPFLAGS) -c $<	

Grafo2Cliques.o: Grafo2Cliques.cpp Grafo2Cliques.h Grafo.o Vertice.o GrafoCordal.o
	$(COMP) $(CPPFLAGS) -c $<	

Grafo3Cliques.o: Grafo3Cliques.cpp Grafo3Cliques.h Grafo.o Vertice.o GrafoCordal.o
	$(COMP) $(CPPFLAGS) -c $<	

GrafoCordal.o: GrafoCordal.cpp GrafoCordal.h Grafo.o Vertice.o
	$(COMP) $(CPPFLAGS) -c $<	

GrafoDominoIntervaloProprio.o: GrafoDominoIntervaloProprio.cpp GrafoDominoIntervaloProprio.h Grafo.o Vertice.o GrafoCordal.o
	$(COMP) $(CPPFLAGS) -c $<	

BibGrafos.o: BibGrafos.cpp BibGrafos.h ArvoreCliques.o ListaAdjacencia.o Grafo.o VetorRequisitos.o Intervalo.o GrafoIntervalo.o GrafoIntervaloProprio.o GrafoSplitIndiferenca.o Grafo2Cliques.o Grafo3Cliques.o GrafoCordal.o GrafoDominoIntervaloProprio.o
	$(COMP) $(CPPFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)

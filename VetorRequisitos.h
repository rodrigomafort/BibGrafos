#include "BibGrafos.h"
#pragma once

namespace BibGrafos
{
	class VetorRequisitos
	{
		private:
			map<Vertice, int> requisitos;
		public:
			VetorRequisitos();
			VetorRequisitos(map<Vertice, int> req);
			VetorRequisitos(int* vec, Grafo* G);
			VetorRequisitos(Grafo* G);
			VetorRequisitos(const VetorRequisitos &obj);
			int& operator[](Vertice index);
			int ObterRequisito(Vertice index);
			~VetorRequisitos();
			string ObterString();

			static VetorRequisitos GerarConstante(Grafo* G, int req);
			static VetorRequisitos GerarAleatorio(Grafo* G, int minReq = 0, int maxReq = INT_MAX);
			static VetorRequisitos GerarAleatorio(Grafo* G, default_random_engine* rnd, int minReq = 0, int maxReq = INT_MAX);

			vector<Vertice> VDObterNaoDominados(vector<Vertice> S, Grafo* G);
			set<Vertice> VDObterNaoDominados(set<Vertice> S, Grafo* G);

			bool VDAvaliarDominacao(vector<Vertice> S, Grafo* G);
			bool VDAvaliarDominacao(set<Vertice> S, Grafo* G);
			static bool VDAvaliarDominacao(Grafo* G, VetorRequisitos R, vector<Vertice> S);
			static bool VDAvaliarDominacao(Grafo* G, VetorRequisitos R, set<Vertice> S);

			vector<Vertice> TSSObterNaoDominados(vector<Vertice> S, Grafo* G);
			set<Vertice> TSSObterNaoDominados(set<Vertice> S, Grafo* G);

			bool TSSAvaliarDominacao(vector<Vertice> S, Grafo* G);
			bool TSSAvaliarDominacao(set<Vertice> S, Grafo* G);
			static bool TSSAvaliarDominacao(Grafo* G, VetorRequisitos R, vector<Vertice> S);
			static bool TSSAvaliarDominacao(Grafo* G, VetorRequisitos R, set<Vertice> S);

			void ObterVetor(Grafo* G, int* vet);

			std::stringstream ObterStream();
			static void ProcessarStream(int n, std::stringstream* s, int* reqs);
	};
}

#include "BibGrafos.h"

#include <cstring>
#include <sstream> 

namespace BibGrafos
{
	VetorRequisitos::VetorRequisitos()
	{

	}

	VetorRequisitos::VetorRequisitos(map<Vertice, int> req)
	{
		requisitos = req;
	}

	VetorRequisitos::VetorRequisitos(int* vector, Grafo* G)
	{
		for (int i = 0; i < G->N(); i++)
		{
			Vertice v = G->VerticePorId(i + 1);
			requisitos[v] = vector[i];
		}
	}

	VetorRequisitos::VetorRequisitos(Grafo* G)
	{
		for (int i = 0; i < G->N(); i++)
		{
			Vertice v = G->VerticePorId(i + 1);
			requisitos[v] = 0;
		}
	}
	
	int& VetorRequisitos::operator[](Vertice index)
	{
		return requisitos[index];
	}

	int VetorRequisitos::ObterRequisito(Vertice index)
	{
		return requisitos[index];
	}

	string VetorRequisitos::ObterString()
	{
		string str = "";
		int i = 0;
		for (pair<Vertice, int> e : requisitos)
		{
			str = str + to_string(e.second);
			if (i < requisitos.size() - 1) str = str + " ";
		}
		return str;
	}

	VetorRequisitos VetorRequisitos::GerarAleatorio(Grafo* G, int minReq, int maxReq)
	{
		VetorRequisitos F;

		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator(seed);

		for (Vertice v : G->Vertices)
		{
			uniform_int_distribution<int> rndSet(minReq, min(G->Grau(v), maxReq));
			int value = rndSet(generator);
			F.requisitos.insert(pair<Vertice, int>(v, value));
		}
		return F;
	}

	VetorRequisitos VetorRequisitos::GerarAleatorio(Grafo* G, default_random_engine* generator, int minReq, int maxReq)
	{
		VetorRequisitos F;
		for (Vertice v : G->Vertices)
		{
			uniform_int_distribution<int> rndSet(minReq, min(G->Grau(v), maxReq));
			int value = rndSet(*generator);
			F.requisitos.insert(pair<Vertice, int>(v, value));
		}
		return F;
	}

	VetorRequisitos VetorRequisitos::GerarConstante(Grafo* G, int req)
	{
		VetorRequisitos R;
		for (Vertice v: G->Vertices)
		{
			int grau = G->Grau(v);
			if (req <= grau)
				R.requisitos.insert(pair<Vertice,int>(v,req));
			else
				R.requisitos.insert(pair<Vertice,int>(v,grau));
		}
		return R;
	}


	bool VetorRequisitos::VDAvaliarDominacao(vector<Vertice> S, Grafo* G)
	{
		vector<Vertice> NC = VDObterNaoDominados(S, G);
		return NC.size() == 0;
	}

	bool VetorRequisitos::VDAvaliarDominacao(set<Vertice> S, Grafo* G)
	{
		set<Vertice> NC = VDObterNaoDominados(S, G);
		return NC.size() == 0;
	}

	vector<Vertice> VetorRequisitos::VDObterNaoDominados(vector<Vertice> S, Grafo* G)
	{
		vector<Vertice> convertidos;
		sort(S.begin(), S.end());
		for (Vertice v : G->Vertices)
		{
			if (find(S.begin(), S.end(), v) != S.end())
			{
				convertidos.push_back(v);
			}
			else
			{
				vector<Vertice> vizinhosNaSolucao;
				set<Vertice> adjV = G->Adjacentes(v);
				set_intersection(adjV.begin(), adjV.end(), S.begin(), S.end(), inserter(vizinhosNaSolucao, vizinhosNaSolucao.end()));
				if (vizinhosNaSolucao.size() >= requisitos[v])
				{
					convertidos.push_back(v);
				}
			}
		}

		vector<Vertice> naoConvertidos;
		set_difference(G->Vertices.begin(), G->Vertices.end(), convertidos.begin(), convertidos.end(), inserter(naoConvertidos, naoConvertidos.end()));
		sort(naoConvertidos.begin(), naoConvertidos.end());

		return naoConvertidos;
	}

	set<Vertice> VetorRequisitos::VDObterNaoDominados(set<Vertice> S, Grafo* G)
	{
		set<Vertice> convertidos;
		for (Vertice v : G->Vertices)
		{
			if (binary_search(S.begin(), S.end(), v))
			{
				convertidos.insert(v);
			}
			else
			{
				set<Vertice> vizinhosNaSolucao;
				set<Vertice> adjV = G->Adjacentes(v);
				set_intersection(adjV.begin(), adjV.end(), S.begin(), S.end(), inserter(vizinhosNaSolucao, vizinhosNaSolucao.end()));
				if (vizinhosNaSolucao.size() >= requisitos[v])
				{
					convertidos.insert(v);
				}
			}
		}

		set<Vertice> naoConvertidos;
		set_difference(G->Vertices.begin(), G->Vertices.end(), convertidos.begin(), convertidos.end(), inserter(naoConvertidos, naoConvertidos.end()));
		return naoConvertidos;
	}
	
	VetorRequisitos::VetorRequisitos(const VetorRequisitos &obj)
	{
		for (pair<Vertice, int> th : obj.requisitos)
		{
			requisitos.insert(pair<Vertice, int>(th.first, th.second));
		}
	}

	VetorRequisitos::~VetorRequisitos()
	{
		requisitos.clear();
	}

	set<Vertice> VetorRequisitos::TSSObterNaoDominados(set<Vertice> S, Grafo* G)
	{
		int n = G->N() * sizeof(bool);

		map<Vertice,pair<bool,int>> proc;

		for (Vertice v : G->Vertices)
		{
			proc.insert(make_pair(v,make_pair(false,0)));
		}

		queue<Vertice> fila;
		for (Vertice v : S)
		{
			fila.push(v);
			proc[v].first = true;
		}

		while (fila.size() > 0)
		{
			Vertice v = fila.front();
			fila.pop();

			for (Vertice u : G->Adjacentes(v))
			{
				proc[u].second = proc[u].second + 1;
				if (proc[u].second >= requisitos[u] && proc[u].first == false)
				{
					fila.push(u);
					proc[u].first = true;
				}
			}
		}

		set<Vertice> res;
		for (Vertice v: G->Vertices)
		{
			if (proc[v].first == false) res.insert(v);
		}
		return res;

		/*
		set<Vertice> S1;
		set<Vertice> S2(S.begin(), S.end());

		while (S1 != S2)
		{
			S1 = S2;
			
			set<Vertice> naoDom;
			set_difference(G->Vertices.begin(), G->Vertices.end(), S1.begin(), S1.end(), inserter(naoDom, naoDom.end()));

			for (Vertice v : naoDom)
			{
				if (G->AdjacentesEmS(v, S1).size() >= requisitos[v])
				{
					S2.insert(v);
				}
			}
		}

		set<Vertice> naoDom;
		set_difference(G->Vertices.begin(), G->Vertices.end(), S2.begin(), S2.end(), inserter(naoDom, naoDom.end()));

		//return naoDom;

		if (naoDom.size() != V.size()) cout << "ERRO NA AVALIACAO" << endl;

		return V;*/
	}

	vector<Vertice> VetorRequisitos::TSSObterNaoDominados(vector<Vertice> S, Grafo* G)
	{
		set<Vertice> SSet(S.begin(), S.end());
		set<Vertice> res = TSSObterNaoDominados(SSet, G);

		vector<Vertice> resVet(res.begin(), res.end());
		return resVet;
	}

	bool VetorRequisitos::TSSAvaliarDominacao(set<Vertice> S, Grafo* G)
	{
		set<Vertice> NDom = TSSObterNaoDominados(S, G);
		return NDom.size() == 0;
	}

	bool VetorRequisitos::TSSAvaliarDominacao(vector<Vertice> S, Grafo* G)
	{
		vector<Vertice> NDom = TSSObterNaoDominados(S, G);
		return NDom.size() == 0;
	}


	bool VetorRequisitos::VDAvaliarDominacao(Grafo* G, VetorRequisitos R, vector<Vertice> S)
	{
		return R.VDAvaliarDominacao(S, G);
	}

	bool VetorRequisitos::VDAvaliarDominacao(Grafo* G, VetorRequisitos R, set<Vertice> S)
	{
		return R.VDAvaliarDominacao(S, G);
	}

	bool VetorRequisitos::TSSAvaliarDominacao(Grafo* G, VetorRequisitos R, vector<Vertice> S)
	{
		return R.TSSAvaliarDominacao(S, G);
	}

	bool VetorRequisitos::TSSAvaliarDominacao(Grafo* G, VetorRequisitos R, set<Vertice> S)
		{
			return R.TSSAvaliarDominacao(S, G);
		}

	void VetorRequisitos::ObterVetor(Grafo* G, int* vet)
	{
		for (Vertice v : G->Vertices)
		{
			vet[v.Id() - 1] = requisitos[v];
		}
	}

	std::stringstream VetorRequisitos::ObterStream()
	{
		stringstream ss;
		set<Vertice> V;
		for (pair<Vertice, int> par : requisitos)
		{
			V.insert(par.first);
		}

		for (Vertice v : V)
		{
			ss << requisitos[v] << endl;
		}

		return ss;
	}
	
	void VetorRequisitos::ProcessarStream(int n, std::stringstream* ss, int* reqs)
	{
		ss->read((char*)reqs, n * sizeof(int));
	}

}

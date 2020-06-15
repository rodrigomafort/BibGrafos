#include "BibGrafos.h"
namespace BibGrafos
{
	Grafo3Cliques::Grafo3Cliques() : GrafoCordal()
	{
	}


	Grafo3Cliques::Grafo3Cliques(int n) : GrafoCordal(n)
	{
		random_device rd;
		mt19937_64 rnd(rd());
		std::uniform_int_distribution<int> casornd(1, 4);
		//int caso = casornd(rnd);
		int caso = 1;
		switch (caso)
		{
		case 1: ConstruirCaso1(n); break;
		case 2: ConstruirCaso2(n); break;
		case 3: ConstruirCaso3(n); break;
		default: ConstruirCaso4(n); break;
		}
	}

	Grafo3Cliques::Grafo3Cliques(int n, int caso) : GrafoCordal(n)
	{
		switch (caso)
		{
		case 1: ConstruirCaso1(n); break;
		case 2: ConstruirCaso2(n); break;
		case 3: ConstruirCaso3(n); break;
		default: ConstruirCaso4(n); break;
		}

	}

	Grafo * Grafo3Cliques::ConstruirAleatorio(int n)
	{
		Grafo3Cliques* G = new Grafo3Cliques(n);
		return G;		
	}

	Grafo3Cliques::Grafo3Cliques(int n, int** matAdj) : GrafoCordal(n, matAdj)
	{

	}

	int max(int v1, int v2)
	{
		if (v1 >= v2)
			return v1;
		else
			return v2;
	}

	void Grafo3Cliques::ConstruirGrafo(int n, bool H[7])
	{
		random_device rd;
		mt19937_64 rnd(rd());
		uniform_int_distribution<int> intrnd(1, 7);

		int R[7] = { 0,	0, 0, 0, 0, 0, 0 };
		int soma = 0;

		for (int i = 0; i < 7; i++)
		{
			if (H[i])
			{
				R[i] = 1;
				soma = soma + 1;
			}
		}

		while (soma < n)
		{
			int r = intrnd(rnd);
			if (H[r - 1])
			{
				R[r - 1] = R[r - 1] + 1;
				soma = soma + 1;
			}
		}
		ConstruirGrafo(n, R);
	}

	void Grafo3Cliques::ConstruirGrafo(int n, int R[7])
	{
		info = "";
		for (int i = 0; i < 7; i++)
		{
			info = info + to_string(R[i]);
			if (i < 6) info = info + "-";
		}

		vector<Vertice> vR[7];

		for (int i = 0; i < 7; i++)
		{
			for (int ct = 1; ct <= R[i]; ct++)
			{
				Vertice v = AdicionarVertice();
				vR[i].push_back(v);
			}
		}

		int rC[3][4] = { { 1, 2, 6, 7 },
						{ 3, 2, 4, 7 },
						{ 5, 4, 6, 7 } };
		vector<Vertice> C[3];

		for (int c = 0; c < 3; c++)
		{
			for (int iRC = 0; iRC < 4; iRC++)
			{
				int R = rC[c][iRC];
				C[c].insert(C[c].end(), vR[R - 1].begin(), vR[R - 1].end());
			}

			for (Vertice v : C[c])
			{
				for (Vertice u : C[c])
				{
					if (v.Id() > u.Id())
						AdicionarAdjacencia(v, u);
				}
			}
		}
	}

	void Grafo3Cliques::ConstruirCaso1(int n)
	{
		//if (n < 4) throw exception("O Grafo precisa ter no m�nino 4 v�rtices");
		bool H[7] = { true, true, false, true, true, false, false };
		ConstruirGrafo(n, H);
	}

	void Grafo3Cliques::ConstruirCaso2(int n)
	{
		//if (n < 5) throw exception("O Grafo precisa ter no m�nino 5 v�rtices");
		bool H[7] = { true, true, true, true, true, false, false };
		ConstruirGrafo(n, H);
	}

	void Grafo3Cliques::ConstruirCaso3(int n)
	{
		//if (n < 5) throw exception("O Grafo precisa ter no m�nino 5 v�rtices");
		bool H[7] = { true, true, false, true, true, false, true };
		ConstruirGrafo(n, H);
	}

	void Grafo3Cliques::ConstruirCaso4(int n)
	{
		//if (n < 6) throw exception("O Grafo precisa ter no m�nino 6 v�rtices");
		bool H[7] = { true, true, true, true, true, false, true };
		ConstruirGrafo(n, H);
	}

	//void Grafo3Cliques::ConstruirCaso5(int n)
	//{
	//	if (n < 6) throw exception("O Grafo precisa ter no m�nino 6 v�rtices");
	//	bool H[7] = { true, true, true, true, false, true, true };
	//	ConstruirGrafo(n, H);
	//}

	int Grafo3Cliques::identificarCaso()
	{
		ArvoreCliques ac = ObterArvoreCliques();

		set<Vertice> C1 = ac.cliques[0]->vertices;
		set<Vertice> C2 = ac.cliques[1]->vertices;
		set<Vertice> C3 = ac.cliques[2]->vertices;

		vector<Vertice> uC1C3;
		set_union(C1.begin(), C1.end(), C3.begin(), C3.end(), inserter(uC1C3, uC1C3.end()));

		vector<Vertice> uC1C2;
		set_union(C1.begin(), C1.end(), C2.begin(), C2.end(), inserter(uC1C2, uC1C2.end()));

		vector<Vertice> uC2C3;
		set_union(C2.begin(), C2.end(), C3.begin(), C3.end(), inserter(uC2C3, uC2C3.end()));

		vector<Vertice> iC1C2;
		set_intersection(C1.begin(), C1.end(), C2.begin(), C2.end(), inserter(iC1C2, iC1C2.end()));

		vector<Vertice> iC1C3;
		set_intersection(C1.begin(), C1.end(), C3.begin(), C3.end(), inserter(iC1C3, iC1C3.end()));

		vector<Vertice> iC2C3;
		set_intersection(C2.begin(), C2.end(), C3.begin(), C3.end(), inserter(iC2C3, iC2C3.end()));

		vector<Vertice> R1;
		set_difference(C1.begin(), C1.end(), uC2C3.begin(), uC2C3.end(), inserter(R1, R1.end()));

		vector<Vertice> R3;
		set_difference(C2.begin(), C2.end(), uC1C3.begin(), uC1C3.end(), inserter(R3, R3.end()));

		vector<Vertice> R5;
		set_difference(C3.begin(), C3.end(), uC1C2.begin(), uC1C2.end(), inserter(R5, R5.end()));

		vector<Vertice> R2;
		set_difference(iC1C2.begin(), iC1C2.end(), C3.begin(), C3.end(), inserter(R2, R2.end()));

		vector<Vertice> R4;
		set_difference(iC2C3.begin(), iC2C3.end(), C1.begin(), C1.end(), inserter(R4, R4.end()));

		vector<Vertice> R6;
		set_difference(iC1C3.begin(), iC1C3.end(), C2.begin(), C2.end(), inserter(R6, R6.end()));

		vector<Vertice> R7;
		set_intersection(iC1C2.begin(), iC1C2.end(), C3.begin(), C3.end(), inserter(R7, R7.end()));

		if ((R3.size() == 0 && R6.size() == 0 && R7.size() == 0) || (R1.size() == 0 && R4.size() == 0 && R7.size() == 0) || (R5.size() == 0 && R2.size() == 0 && R7.size() == 0))
		{
			return 1;
		}

		if ((R3.size() == 0 && R6.size() == 0) || (R1.size() == 0 && R4.size() == 0) || (R5.size() == 0 && R2.size() == 0))
		{
			return 3;
		}

		if ((R6.size() == 0 && R7.size() == 0) || (R4.size() == 0 && R7.size() == 0) || (R2.size() == 0 && R7.size() == 0))
		{
			return 2;
		}

		if ((R6.size() == 0) || (R4.size() == 0) || (R2.size() == 0))
		{
			return 4;
		}

		if ((R3.size() == 0) || (R1.size() == 0) || (R5.size() == 0))
		{
			return 5;
		}

		return -1;
	}
}

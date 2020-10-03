#include "BibGrafos.h"
namespace BibGrafos
{
	template<typename T>
	T removerAleatorio(vector<T> &vec)
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator(seed);
		uniform_int_distribution<int> rndSet(0, vec.size() - 1);
		int j = rndSet(generator);
		T value = vec[j];
		vec.erase(vec.begin() + j);
		return value;
	}

	static vector<Vertice> Intersecao(set<Vertice> s1, set<Vertice> s2)
	{
		vector<Vertice> result;
		set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), inserter(result, result.end()));
		return result;
	}

	static vector<Vertice> Uniao(set<Vertice> s1, set<Vertice> s2)
	{
		vector<Vertice> result;
		set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), inserter(result, result.end()));
		return result;
	}

	DetalhesSplitIndiferenca GrafoSplitIndiferenca::Detalhar()
	{
		set<set<Vertice>> cliques = ObterCliquesMaximais();
		switch (cliques.size())
		{
		case 1:
		{
			return DetalhesSplitIndiferenca(1, set<Vertice>(*cliques.begin()));
		}
		case 2:
		{
			set<Vertice> C1 = *cliques.begin();
			set<Vertice> C2 = *cliques.rbegin();

			set<Vertice> diferenca;
			set_difference(C1.begin(), C1.end(), C2.begin(), C2.end(), inserter(diferenca, diferenca.begin()));

			if (diferenca.size() != 1)
			{
				C1 = *cliques.rbegin();
				C2 = *cliques.begin();
			}

			diferenca.clear();
			set_difference(C1.begin(), C1.end(), C2.begin(), C2.end(), inserter(diferenca, diferenca.begin()));
			Vertice v = *diferenca.begin();

			return DetalhesSplitIndiferenca(2, set<Vertice>(C1), set<Vertice>(C2), v);
		}
		default:
		{
			set<set<Vertice>>::iterator it = cliques.begin();

			set<Vertice> A = *it; it++;
			set<Vertice> B = *it; it++;
			set<Vertice> C = *it; it++;

			set<Vertice> C1;
			set<Vertice> C2;
			set<Vertice> C3;

			//Descobrir C2
			set<Vertice> BEA;
			set_difference(B.begin(), B.end(), A.begin(), A.end(), inserter(BEA, BEA.end()));

			set<Vertice> CEA;
			set_difference(C.begin(), C.end(), A.begin(), A.end(), inserter(CEA, CEA.end()));

			set<Vertice> AEB;
			set_difference(A.begin(), A.end(), B.begin(), B.end(), inserter(AEB, AEB.end()));

			set<Vertice> CEB;
			set_difference(C.begin(), C.end(), B.begin(), B.end(), inserter(CEB, CEB.end()));


			if (BEA.size() == 1 && CEA.size() == 1)
			{
				C2 = A;
				C1 = B;
				C3 = C;
			}
			else
			{
				if (AEB.size() == 1 && CEB.size() == 1)
				{
					C2 = B;
					C1 = A;
					C3 = C;
				}
				else
				{
					C2 = C;
					C1 = A;
					C3 = B;
				}
			}

			set<Vertice> C1EC2;
			set_difference(C1.begin(), C1.end(), C2.begin(), C2.end(), inserter(C1EC2, C1EC2.end()));

			set<Vertice> C3EC2;
			set_difference(C3.begin(), C3.end(), C2.begin(), C2.end(), inserter(C3EC2, C3EC2.end()));

			Vertice v = *C1EC2.begin();
			Vertice w = *C3EC2.begin();

			set<Vertice> interC1C3;
			set_intersection(C1.begin(), C1.end(), C3.begin(), C3.end(), inserter(interC1C3, interC1C3.end()));

			if (interC1C3.size() == 0)
				return DetalhesSplitIndiferenca(3, C1,C2,C3, v, w);
			else
				return DetalhesSplitIndiferenca(4, C1,C2,C3, v, w);
		}
		}
	}


	int GrafoSplitIndiferenca::Caso()
	{
		ArvoreCliques AC = ObterArvoreCliques();
		switch (AC.cliques.size())
		{
		case 1: return 1;
		case 2: return 2;
		case 3:
		{
			set<Vertice> A = AC.cliques[0]->vertices;
			set<Vertice> B = AC.cliques[1]->vertices;
			set<Vertice> C = AC.cliques[2]->vertices;

			auto x = Intersecao(A, C);
			vector<Vertice> result;
			set_difference(A.begin(), A.end(), C.begin(), C.end(), inserter(result, result.end()));

			if ((Intersecao(A, B).size() == 0) || (Intersecao(A, C).size() == 0) || (Intersecao(B, C).size() == 0))
			{
				return 3;
			}
			else
			{
				if ((Uniao(A, B).size() == N()) || (Uniao(A, C).size() == N()) || (Uniao(B, C).size() == N()))
				{
					return 4;
				}
				else
				{
					return 0;
				}
			}
		}
		default: return 0;
		}
	}

	GrafoSplitIndiferenca::GrafoSplitIndiferenca(int n) : GrafoCordal(n)
	{
		//int classe = aleatorio(1, 5);	
		int classe = 4;
		switch (classe)
		{
		case (1): ConstruirCaso1(n); break;
		case (2): ConstruirCaso2(n); break;
		case (3): ConstruirCaso3(n); break;
		default: ConstruirCaso4(n); break;
		}
	}

	void GrafoSplitIndiferenca::ConstruirCaso1(int n)
	{
		for (int i = 1; i <= n; i++)
		{
			Vertice v = AdicionarVertice(i);
			for (Vertice u : Vertices)
			{
				if (v == u) continue;
				AdicionarAdjacencia(v, u);
			}
		}
	}

	void GrafoSplitIndiferenca::ConstruirCaso2(int n)
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator(seed);
		std::uniform_int_distribution<int> distribution(1, n - 2);

		//Construção Clique C2
		for (int i = 2; i <= n; i++)
		{
			Vertice v = AdicionarVertice(i);
			for (Vertice u : Vertices)
			{
				if (v == u) continue;
				AdicionarAdjacencia(v, u);
			}
		}
		//Construção de C1
		//C1 tem tamanho de no máximo n-2 (1.. n-2)
		vector<Vertice> CL(Vertices.begin(), Vertices.end());
		int tc1 = distribution(generator);

		Vertice v = AdicionarVertice(1);

		for (int ct = 1; ct <= tc1; ct++)
		{
			std::uniform_int_distribution<int> rndSet(0, CL.size() - 1);

			int j = rndSet(generator);
			Vertice u = CL[j];
			CL.erase(CL.begin() + j);
			AdicionarAdjacencia(v, u);
		}
	}

	void GrafoSplitIndiferenca::ConstruirCaso3(int n)
	{
		//Trẽs cliques C1, C2 e C3 tais que |C1\C2| = |C3\C2| = 1
		//E a interseção entre C1 e C3 é vazia
		int i = 0;

		Vertice v = AdicionarVertice(1);
		Vertice w = AdicionarVertice(n);

		vector<Vertice> C2;

		for (i = 2; i < n; i++)
		{
			Vertice u = AdicionarVertice(i);
			for (Vertice x : C2)
			{
				AdicionarAdjacencia(u, x);
			}
			C2.push_back(u);
		}

		vector<Vertice> cl(C2);
		uniform_real_distribution<double> distribution(0.0, 1.0);
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator(seed);


		while (cl.size() > 0)
		{
			Vertice u = removerAleatorio(cl);
			if (Grau(v) == 0)
			{
				AdicionarAdjacencia(v, u);
			}
			else
			{
				double d = distribution(generator);
				if (Grau(w) == 0)
				{
					AdicionarAdjacencia(w, u);
				}
				else
				{
					if (d <= 0.35)
					{
						//37% - u é vizinho de v
						AdicionarAdjacencia(v, u);
					}
					else
					{
						if (d <= 0.70)
						{
							//37% - u é vizinho de w
							AdicionarAdjacencia(w, u);
						}
						else
						{
							//~26% - u será simplicial em C_2
						}
					}
				}
			}
		}
	}

	void GrafoSplitIndiferenca::ConstruirCaso4(int n)
	{
		//Trê cliques C1, C2 e C3 tais que |C1\C2| = |C3\C2| = 1
		//E a união entre C1 e C3 equivale ao conjunto de vértices
		int i = 0;

		Vertice v = AdicionarVertice(1);
		Vertice w = AdicionarVertice(n);

		vector<Vertice> C2;

		for (i = 2; i < n; i++)
		{
			Vertice u = AdicionarVertice(i);
			for (Vertice x : C2)
			{
				AdicionarAdjacencia(u, x);
			}
			C2.push_back(u);
		}

		uniform_real_distribution<double> distribution(0.0, 1.0);
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator(seed);


		for (i = 0; i < C2.size(); i++)
		{
			//Para cada vértice vi pertencente a c2 ou ele pertence:
			//C1 e C3
			//Somente C1
			//Somente C3
			//Para garantir que nem v nem w serão isolados, o primeiro vértice será adjacente aos dois
			if (i == 0)
			{
				AdicionarAdjacencia(v, C2[0]);
				AdicionarAdjacencia(w, C2[0]);
			}
			else
			{
				double d = distribution(generator);
				if (d <= 0.35)
				{
					//35% -- adjacente somente a v
					AdicionarAdjacencia(v, C2[i]);
				}
				else
				{
					if (d <= 0.70)
					{
						//35% -- adjacente somente a w
						AdicionarAdjacencia(w, C2[i]);
					}
					else
					{
						//~30% -- adjacente aos dois (v e w)
						AdicionarAdjacencia(v, C2[i]);
						AdicionarAdjacencia(w, C2[i]);
					}
				}
			}
		}
	}

	GrafoSplitIndiferenca::GrafoSplitIndiferenca(int n, int** matAdj) : GrafoCordal(n, matAdj)
	{
	}

	Grafo * GrafoSplitIndiferenca::ConstruirAleatorio(int n)
	{
		GrafoSplitIndiferenca* G = new GrafoSplitIndiferenca(n);
		return G;
	}

	GrafoSplitIndiferenca::GrafoSplitIndiferenca()
	{

	}

	string GrafoSplitIndiferenca::ObterDetalhes()
	{
		stringstream sb;
		int caso = Caso();
		sb << "Split Indiferença - Caso: " << caso << "\n";
		ArvoreCliques AC = ObterArvoreCliques();

		if (caso == 1)
		{
			sb << "G é Completo" << "\n";
			sb << "C1: " + Grafo::ObterString(AC.cliques[0]->vertices) << "\n";
		}
		else
		{
			if (caso == 2)
			{
				sb << "G possui duas cliques C1 e C2 tais que | C1 \\ C2 | = 1" << "\n";

				set<Vertice> C1 = AC.cliques[0]->vertices;
				set<Vertice> C2 = AC.cliques[1]->vertices;


				vector<Vertice> diferenca;
				set_difference(C1.begin(), C1.end(), C2.begin(), C2.end(), inserter(diferenca, diferenca.begin()));

				if (diferenca.size() != 1)
				{
					C1 = AC.cliques[1]->vertices;
					C2 = AC.cliques[0]->vertices;
				}

				diferenca.clear();
				set_difference(C1.begin(), C1.end(), C2.begin(), C2.end(), inserter(diferenca, diferenca.begin()));
				Vertice v = diferenca[0];

				sb << "C1: " + Grafo::ObterString(C1) << "\n";
				sb << "C2: " + Grafo::ObterString(C2) << "\n";
				sb << "v = C1\\C2 = " << v.Id() << "\n";
			}
			else
			{
				//Classe 3 ou 4
				set<Vertice> A = AC.cliques[0]->vertices;
				set<Vertice> B = AC.cliques[1]->vertices;
				set<Vertice> C = AC.cliques[2]->vertices;

				set<Vertice> C1;
				set<Vertice> C2;
				set<Vertice> C3;

				//Descobrir C2
				vector<Vertice> BEA;
				set_difference(B.begin(), B.end(), A.begin(), A.end(), inserter(BEA, BEA.end()));

				vector<Vertice> CEA;
				set_difference(C.begin(), C.end(), A.begin(), A.end(), inserter(CEA, CEA.end()));

				vector<Vertice> AEB;
				set_difference(A.begin(), A.end(), B.begin(), B.end(), inserter(AEB, AEB.end()));

				vector<Vertice> CEB;
				set_difference(C.begin(), C.end(), B.begin(), B.end(), inserter(CEB, CEB.end()));


				if (BEA.size() == 1 && CEA.size() == 1)
				{
					C2 = A;
					C1 = B;
					C3 = C;
				}
				else
				{
					if (AEB.size() == 1 && CEB.size() == 1)
					{
						C2 = B;
						C1 = A;
						C3 = C;
					}
					else
					{
						C2 = C;
						C1 = A;
						C3 = B;
					}
				}

				vector<Vertice> C1EC2;
				set_difference(C1.begin(), C1.end(), C2.begin(), C2.end(), inserter(C1EC2, C1EC2.end()));

				vector<Vertice> C3EC2;
				set_difference(C3.begin(), C3.end(), C2.begin(), C2.end(), inserter(C3EC2, C3EC2.end()));

				Vertice v = C1EC2[0];
				Vertice w = C3EC2[0];

				if (caso == 3)
				{
					sb << "G possui trés cliques C1, C2 e C3, tais que | C1 \\ C2 | = | C3 \\ C2 | = 1 e C1 n C3 = { }" << "\n";
				}
				else
				{
					sb << "G possui três cliques C1, C2 e C3, tais que | C1 \\ C2 | = | C3 \\ C2 | = 1 e C1 U C3 = V(G)" << "\n";
				}

				sb << "C1: " << Grafo::ObterString(C1) << "\n";
				sb << "C2: " << Grafo::ObterString(C2) << "\n";
				sb << "C3: " << Grafo::ObterString(C3) << "\n";
				sb << "v = C1\\C2 = " << v.Id() << "\t" << "w = C3\\C2 = " << w.Id() << "\n";

			}
		}

		return sb.str();
	}

	GrafoSplitIndiferenca::GrafoSplitIndiferenca(int n, int classe)
	{
		switch (classe)
		{
			case (1): ConstruirCaso1(n); break;
			case (2): ConstruirCaso2(n); break;
			case (3): ConstruirCaso3(n); break;
			default: ConstruirCaso4(n); break;
		}
	}


}

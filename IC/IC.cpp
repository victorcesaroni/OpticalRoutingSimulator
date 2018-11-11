#include "stdafx.h"
#include "Network.h"
#include "NetworkConfigParser.h"
#include "NetworkInfo.h"
#include "StaticSimulation.h"

int main()
{
	Network network;
	//NetworkConfigParser::Parse("config.txt", &network);

	/*
	// 1 
	// | \ 
	// 2 - 3
	network.AddNode("n1", { Link("n2", 1), Link("n3", 1) });
	network.AddNode("n2", { Link("n1", 1), Link("n3", 1) });
	network.AddNode("n3", { Link("n2", 1), Link("n1", 1) });*/

	/*// 1 
	// | \ 
	// 2 - 3 - 4 - 5 - 6 - 7
	network.AddNode("n1", { Link("n2", 1), Link("n3", 1) });
	network.AddNode("n2", { Link("n1", 1), Link("n3", 1) });
	network.AddNode("n3", { Link("n2", 1), Link("n1", 1), Link("n4", 1) });
	network.AddNode("n4", { Link("n3", 1), Link("n5", 1) });
	network.AddNode("n5", { Link("n4", 1), Link("n6", 1) });
	network.AddNode("n6", { Link("n5", 1), Link("n7", 1) });
	network.AddNode("n7", { Link("n6", 1) });*/

	
	// 1 - 4
	// |   |
	// 2 - 3	
	network.AddNode("n1", { Link("n2", 1, false), Link("n4", 1, false) });
	network.AddNode("n2", { Link("n1", 1, false), Link("n3", 1, false) });
	network.AddNode("n3", { Link("n2", 1, false), Link("n4", 1, false) });
	network.AddNode("n4", { Link("n1", 1, false), Link("n3", 1, false) });

	/*
	//  1[c]-[c]4
	// [c]      |
	//  |       | 
	// [c]      |
	//  2-------3	
	network.AddNode("n1", { Link("n2", 1, true), Link("n4", 1, true) });
	network.AddNode("n2", { Link("n1", 1, true), Link("n3", 1, false) });
	network.AddNode("n3", { Link("n2", 1, false), Link("n4", 1, false) });
	network.AddNode("n4", { Link("n1", 1, true), Link("n3", 1, false) });*/

	/*
	// 1 - 2[conversor] - 3 - 4
	network.AddNode("n1", { Link("n2", 1, false) });
	network.AddNode("n2", { Link("n3", 1, true), Link("n1", 1, false) });
	network.AddNode("n3", { Link("n4", 1, false), Link("n2", 1, false) });
	network.AddNode("n4", { Link("n3", 1, false) });*/

	/*
	// 1[conversor] - 2[conversor] - 3[conversor] - 4
	network.AddNode("n1", { Link("n2", 1, true) });
	network.AddNode("n2", { Link("n3", 1, true), Link("n1", 1, false) });
	network.AddNode("n3", { Link("n4", 1, true), Link("n2", 1, false) });
	network.AddNode("n4", { Link("n3", 1, false) });*/

	/*
	// 1 - 2 - 3 - 4
	network.AddNode("n1", { Link("n2", 1, false) });
	network.AddNode("n2", { Link("n3", 1, false), Link("n1", 1, false) });
	network.AddNode("n3", { Link("n4", 1, false), Link("n2", 1, false) });
	network.AddNode("n4", { Link("n3", 1, false) });*/

	/*
	// 1 - 4 - 5 - 6
	// |   |       
	// 2 - 3 	
	network.AddNode("n1", { Link("n2", 1, false), Link("n4", 1, false) });
	network.AddNode("n2", { Link("n1", 1, false), Link("n3", 1, false) });
	network.AddNode("n3", { Link("n2", 1, false), Link("n4", 1, false) });
	network.AddNode("n4", { Link("n1", 1, false), Link("n3", 1, false) });
	network.AddNode("n5", { Link("n4", 1, false), Link("n6", 1, false) });
	network.AddNode("n6", { Link("n5", 1, false) });*/
	
	// RNP - http://www.av.it.pt/anp/on/figuras/rnp.jpg
	/*network.AddNode("Fortaleza", { Link("BeloHorizonte", 1) ,Link("Recife", 1) });
	network.AddNode("Recife", { Link("Fortaleza", 1),Link("Salvador", 1) });
	network.AddNode("Salvador", { Link("Recife", 1),Link("RioDeJaneiro", 1) });
	network.AddNode("Brasilia", { Link("BeloHorizonte", 1) ,Link("SaoPaulo", 1),Link("SemNome", 1) });
	network.AddNode("BeloHorizonte", { Link("Fortaleza", 1),Link("Brasilia", 1),Link("RioDeJaneiro", 1) });
	network.AddNode("RioDeJaneiro", { Link("Salvador", 1),Link("BeloHorizonte", 1),Link("SaoPaulo", 1) });
	network.AddNode("SaoPaulo", { Link("Brasilia", 1),Link("RioDeJaneiro", 1),Link("Curitiba", 1) });
	network.AddNode("Curitiba", { Link("SaoPaulo", 1),Link("Florianopolis", 1) });
	network.AddNode("Florianopolis", { Link("Curitiba", 1),Link("SemNome", 1) });
	network.AddNode("SemNome", { Link("Florianopolis", 1),Link("Brasilia", 1) });*/
	
	StaticSimulation staticSim = StaticSimulation(&network);
	staticSim.Run();

#if 0
	printf("************************\n");
	printf("* Matriz de conexoes:\n");
	network.PrintNodeMatrix(staticSim.connectionsMatrix);
	printf("\n");

	printf("************************\n");
	printf("* Matriz de distancia minima:\n");
	network.PrintNodeMatrixNotInf(staticSim.minimumDistanceMatrix);
	printf("\n");
#endif

	auto printRoute = [&](const std::vector<PathInfo>& paths)
	{
		int last = -1;
		for (const auto& p : paths)
		{
			auto fromName = network.FindNameById(p.from);
			auto toName = network.FindNameById(p.to);

			if (p.from != last)
			{
				printf("Partindo de %s\n", fromName.c_str());
			}
			if (p.hops.size() > 0)
			{
				printf(" ate %s: ", toName.c_str());

				printf("%s->", fromName.c_str());
				for (unsigned i = 0; i < p.hops.size(); i++)
				{
					auto& h = p.hops[i];
					printf("%s", network.FindNameById(h).c_str());
					if (i != p.hops.size() - 1)
						printf("->");
				}
			}
			else
			{
				printf(" nao pode chegar ate %s.", toName.c_str());
			}

			last = p.from;
			printf("\n");
		}
		printf("\n");
	};

#if 0
	printf("************************\n");
	printf("* Rotas minimas:\n");
	printRoute(staticSim.minimumCostPaths);

	printf("************************\n");
	printf("* Numero medio de hops: ");
	printf("%f\n", staticSim.avgHops);
	printf("\n");
#endif

	printf("************************\n");
	printf("* Matriz de utilizacao de enlace:\n");
	network.PrintNodeMatrix(staticSim.utilizationMatrix);
	printf("\n");
	network.PrintPairMatrix(staticSim.utilizationMatrix);
	printf("\n");

#if 0
	printf("************************\n");
	printf("* Matriz de alocacao de lambda por caminho minimo (caso nao use conversor):\n");
	network.PrintLambdaPairMatrix(staticSim.lambdaMatrix);

	{
		std::map<int, bool> usedLambdas;

		Matrix<int> tmp; // matriz que mostra o numero de lambda alocado de (de i, ate j)
		tmp.Create(staticSim.lambdaMatrix.N, staticSim.lambdaMatrix.M);
		tmp.Fill(0);

		for (int i = 0; i < staticSim.lambdaMatrix.N; i++)
		{
			for (int j = 0; j < staticSim.lambdaMatrix.M; j++)
			{
				if (i == j)
					continue;

				for (const auto& k : staticSim.lambdaMatrix[i][j]) // percorre a lista de lambdas que foram alocados de i ate j
				{
					usedLambdas[k.lambda] = true; // salva o lambda para exibir depois
					tmp[k.from][k.to] = k.lambda; // salva o numero de lambda que foi escolhido para o caminho do lambda atual
				}
			}
		}

		printf("Lambdas usados: ");
		for (const auto& k : usedLambdas)
		{
			printf("L%d ", k.first);
		}
		printf("\n");

		network.PrintNodeMatrix(tmp);
	}
#endif

	printf("************************\n");
	printf("* Alocacao de lambda por caminho minimo:\n");
	network.PrintLambdaPairMatrix(staticSim.lambdaMatrix);

	{
		std::map<int, bool> usedLambdas;

		for (int i = 0; i < staticSim.lambdaMatrix.N; i++)
		{
			for (int j = 0; j < staticSim.lambdaMatrix.M; j++)
			{
				if (i == j)
					continue;

				for (const auto& k : staticSim.lambdaMatrix[i][j]) // percorre a lista de lambdas que foram alocados de i ate j
				{
					usedLambdas[k.lambda] = true; // salva o lambda para exibir depois					
				}
			}
		}

		printf("Lambdas usados: ");
		for (const auto& k : usedLambdas)
		{
			printf("L%d ", k.first);
		}
		printf("\n");
	}

	printf("\n");


#if 0
	printf("************************\n");
	printf("* Todas as rotas:\n");

	{
		std::vector<PathInfo> tmp;
		int N = network.nodeCount;

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (i == j)
					continue;

				for (const auto& p : staticSim.allPaths[i][j])
					tmp.push_back(p);
			}
		}

		printRoute(tmp);
	}
#endif
	getchar();

	return 0;
}

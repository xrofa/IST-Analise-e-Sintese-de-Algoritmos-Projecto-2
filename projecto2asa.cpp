/*	Christian Sousa @ ist.utl.pt
	Projecto de Analise e Sintese de Algoritmos - Entrega 2
	Descricao:	O problema proposto envolve a verificação de existência de uma solução
	para o problema de construção de uma rede de transporte entre cidades 
	(em que o transporte pode ser feito por estrada ou por aeroportos) dado um input 
	correspondente às estradas e aeroportos que podem ser construídos bem como os seus custos.
	Para além disso, se for verificado que é possível solucionar o problema, deve ser devolvida
	a solução que implique o melhor preço (custo de construção de estradas e aeroportos menor).
	Para solucionar este problema foi usado o algoritmo Kruskal de modo a obter a árvore 
	abrangente de menor custo.
		
	Language: C++
	Compiler: g++ (GCC)
	Version: 4.7.2
	Command Line: g++ -O3 -ansi -Wall $file -lm
*/
#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

#define S 0 // Vertice ficticio que faz a ligacao de qualquer cidade caso se pretenda construir um aeroporto 
#define AEROPORTO 1 // Identificador se a ligacao em questao e com Aeroporto ou Estrada
using namespace std;
vector<list<int> > InputAeroportoCusto; // Vector com os Aeroportos e Respectivos Custos
vector<int> Insuficiente; // Vector com a Informacao para a decisao de ser possivel ou nao construir uma rede
vector<list <pair <int, int> > >InputEstradaCusto; // Vector com as Estradas a ser construidas e respectivo custo
int N_cidades, N_aeroportosAconstruir, N_estradasAconstruir;
int N_estradasAconstruirOUT=0; // Output do numero de estradas a construir
int N_aeroportosAconstruirOUT=0; // Output do numero de aeroportos a construir


typedef  pair<int, int> iPair;// Atalho para um par de inteiros
/*	Estrutura com:
 	u- numero da cidade A
 	v- numero da cidade B
 	w- custo de ser construida uma estrada
 	z- custo de ser construido um aeroporto
*/
struct Edge
{
    int u, v, w, z;
    
};

/*
	Funcao de comparacao de arestas: Compara a aresta A com a aresta B
	-Se o custo de construcao de uma estrada de A for menor que o de B, retorna true
	-Se os custos de construcao de estradas forem iguais e o custo de construcao de um aeroporto
	de A for menor que B retorna true
	-Caso contrario retorna false
*/
bool Ecompare(Edge a, Edge b){
    if(a.w < b.w){
        return true;
    }
    else if(a.w == b.w && a.z < b.z){
        return true;
    }
    else return false;
    //return a.w < b.w;
}

// Estrutura para representar um grafo
struct Graph
{
    int V, E;
    vector< Edge > edges;
    
    // Construtor
    Graph(int V, int E)
    {
        this->V = V;
        this->E = E;
    }
    
    // Funcao para adicionar uma aresta
    
    void addEdge(int u, int v, int w, int z)
    {
        Edge edge;
        edge.u=u;
        edge.v=v;
        edge.w=w;
        edge.z=z;
        edges.push_back(edge);
    }
    
    
    // Funcao para encontrar a MST utilizando o Algoritmo Kruskal
    int kruskalMST();
};

// Para representar sets disjuntos
struct DisjointSets
{
    int *parent, *rnk;
    int n;
    
    // Construtor
    DisjointSets(int n)
    {
        // Alocar memoria
        this->n = n;
        parent = new int[n+1];
        rnk = new int[n+1];
        /*
        	Inicialmente, todos os vertices estao em sets 
        	diferentes e tem rank = 0
        */
        for (int i = 0; i <= n; i++)
        {
            rnk[i] = 0;
            
            //cada elemento e pai de si proprio
            parent[i] = i;
        }
    }
    
    // Encontrar pai do no u
    int find(int u)
    {
        /* Fazer com que os pais no caminho de
        u--> parent[u] apontem para parent[u] */
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }
    
    // Uniao por rank
    void merge(int x, int y)
    {
        x = find(x), y = find(y);
        
         /* Fazer arvore com altura menor 
         	uma sub-arvore de outra arvore 
         */
        if (rnk[x] > rnk[y])
            parent[y] = x;
        else // Se rnk[x] <= rnk[y]
            parent[x] = y;
        
        if (rnk[x] == rnk[y])
            rnk[y]++;
    }
};

/* A funcao retorna o valor da MST */

int Graph::kruskalMST()
{
    int mst_wt = 0; // Inicializar o resultado
    
    // Ordenar os caminhos de forma crescente em termos dos custos
    sort(edges.begin(), edges.end(), Ecompare);
    vector<Edge>::iterator i;
    for (i=edges.begin(); i!=edges.end(); i++)
    {
        //cout<<"Edge u:"<<i->u<<", v:"<<i->v<<", w:"<<i->w<<", tipo:"<<i->z<<endl;
    }
    //cout<<"==============================================================="<<endl;
    // Criar sets disjuntos
    DisjointSets ds(V);
    
    // Iterar através de todos os caminhos ordenados
    vector< Edge >::iterator it;
    for (it=edges.begin(); it!=edges.end(); it++)
    {
        int u = it->u;
        int v = it->v;
        
        int set_u = ds.find(u);
        int set_v = ds.find(v);
        
        /*
        	Verificar se o caminho escolhido esta a criar
        	um ciclo ou nao ( O ciclo e criado se u e v
        	pertencerem ao mesmo set)
        */
        if (set_u != set_v)
        {
            // O caminho ficara na MST
            //cout << u << "-" << v << endl;
            
            Insuficiente[u]=1;
            Insuficiente[v]=1;
            
            
            if(v == 0 || u == 0){
                N_aeroportosAconstruirOUT+=1;
            }else{
                N_estradasAconstruirOUT+=1;
            }
            // Actualiza o peso da MST 
            mst_wt += it->w;
            
            // Merge dos dois sets
            ds.merge(set_u, set_v);
        }
    }
    return mst_wt;
}


int main()
{
    int cidadeA, custoaeroporto, cidadeEA, cidadeEB, custoestrada;
    /*
    	Variaveis:
    	cidadeA - cidade onde o aeroporto pode ser construido
    	custoaeroporto - custo do aeroporto a ser construido
    	cidadeEA - cidade A onde pode ser construida a estrada que liga a cidade B
    	cidadeEB - cidade B onde pode ser construida a estrada que liga a cidade A
    	custoestrada - custo da estrada a ser construida ligando a cidade A a cidade B
    */
    
    /*
    	Recebe os dois inputs:
    	N_cidades: com o numero de cidades a serem consideradas 
    	N_aeroportosAconstruir : numero de aeroportos que podem ser construidos
    */
    cin>>N_cidades;
    cin>>N_aeroportosAconstruir;
    
    // Limitacao dada pelo enunciado, o numero de cidades tem que ser superior ou igual a 2
    if(N_cidades < 2){
        return 0;
    }
    
    //Reserva de memória para os varios vectores
    InputAeroportoCusto.resize(N_cidades+1);
    InputAeroportoCusto.reserve(N_cidades+1);
    InputEstradaCusto.resize(N_cidades+1);
    InputEstradaCusto.reserve(N_cidades+1);
    Insuficiente.resize(N_cidades+1);
    Insuficiente.reserve(N_cidades+1);
    
    // Colocar todos os valores do vector Insuficiente a 0
    Insuficiente.assign(Insuficiente.size(), 0);
    
    
	/*
		Recebe os inputs que contem:
		Identificador da cidade e o custo de construcao de um aeroporto
		Sao colocados no vector InputAeroportoCusto
	*/
    for(int i=1; i<=N_aeroportosAconstruir; i++){
        cin>>cidadeA>>custoaeroporto;
        InputAeroportoCusto[cidadeA].push_back(custoaeroporto);
    }
    
    // Recebe o input do numero de estradas que podem ser construidas
    cin>>N_estradasAconstruir;
    
    /*
    	Cria dois grafos:
    	Grafo g onde sao considerados o conjunto de Aeroportos+Estradas
    	Grafo e onde apenas e considerado o conjunto de estradas a serem construidas
    */
    Graph g(N_cidades+1, N_estradasAconstruir+N_aeroportosAconstruir);
    Graph e(N_cidades+1, N_estradasAconstruir);
    
    /*
    	Como qualquer cidade que tenha um aeroporto pode ser ligada a outra cidade 
    	que tambem ja tenha um aeroporto, e criado um novo vertice ficticio S (com o valor
    	0) que representa as ligacoes entre os aeroportos.
    	Sao adicionados ao grafo g as ligacoes da cidade i, ao vertice S com o valor 
    	do custo de construcao do aeroporto(*itr), tendo tambem um valor z, neste caso 
    	aeroporto que indica que e uma ligacao de aeroporto(1) e nao de estrada(0)
    */
    for (size_t i = 1; i < InputAeroportoCusto.size(); ++i) {
        list <int> ::iterator itr = InputAeroportoCusto[i].begin();
        while (itr != InputAeroportoCusto[i].end()) {
            g.addEdge(i, S, (*itr), AEROPORTO);
            ++itr;
        }
    }
    
    /*
    	Sao adicionados aos grafos g e e as ligacoes entre as cidades que podem estar
    	ligadas atraves de uma estrada
    	Com cidadeEA como indicador da primeira cidade, cidadeEB indicador da segunda cidade
    	custoestrada o custo de construcao da estrada e S como indicador de que se trata
    	de uma ligacao de estradas(0) e nao de aeroportos(1)
    */
    for(int i=1; i<= N_estradasAconstruir; i++){
        cin>>cidadeEA>>cidadeEB>>custoestrada;
        g.addEdge(cidadeEA, cidadeEB, custoestrada, S);
        e.addEdge(cidadeEA, cidadeEB, custoestrada, S);
    }
    
    
    
    int kruskal_mst_estradas = e.kruskalMST(); // Corre o algoritmo Kruskal referente apenas ao conjunto de estradas e guarda o valor da MST na variavel kruskal_mst_estradas
    int InsuficienteEFlag=0; // Coloca a Flag de Insuficiencia a 0
    int OutAeroportosE = N_aeroportosAconstruirOUT; // Guarda o numero de aeroportos finais a serem construidos que e calculado durante o algoritmo Kruskal referente as estradas (neste caso o valor desta variavel sera sempre 0)
    int OutEstradasE = N_estradasAconstruirOUT; // Guarda o numero de Estradas finais a serem construidas que sao calculadas durante o algoritmo Kruskal referente as estradas
    
    N_estradasAconstruirOUT=0; // Coloca o numero de estradas a construir a 0 para serem usadas novamente no proximo algoritmo Kruskal referente ao conjunto aeroportos+estradas
    N_aeroportosAconstruirOUT=0;// Coloca o numero de aeroportos a construir a 0 para serem usadas novamente no proximo algoritmo Kruskal referente ao conjunto aeroportos+estradas
    
    /*
    	Caso nao seja possivel criar a rede pretendida, coloca a Flag de Insuficiente a 1
    */
    for (size_t i = 1; i < Insuficiente.size(); ++i) {
        if(Insuficiente[i] == 0){
            InsuficienteEFlag=1;
        }
    }
    
    Insuficiente.assign(Insuficiente.size(), 0); // Coloca novamente o vector de Insuficiente a 0 para ser usado novamente no proximo Kruskal de aeroportos+estradas
    
    int kruskal_mst_peso = g.kruskalMST(); // Corre o algoritmo Kruskal referente ao conjunto de aeroportos+estradas e guarda o valor da MST na variavel kruskal_mst_peso
    
    /*
    	Verifica a variavel Insuficiente, caso seja diferente de 0 quer dizer que nao e possivel criar a rede prentendida e por isso o programa termina com a mensagem Insuficiente
    */
    for (size_t i = 1; i < Insuficiente.size(); ++i) {
        if(Insuficiente[i] == 0){
            cout << "Insuficiente" <<endl;
            return 0;
        }
    }
    
    //cout << "Peso Estradas: " << kruskal_mst_estradas << endl;
    //cout << "Peso Aeroportos: " << kruskal_mst_peso << endl;
    //cout << "Insuficiente Flag:" << InsuficienteEFlag << endl;
    
    /*
    	E possivel a criacao da rede pretendida.
    	Se o valor da MST de estradas e inferior ou igual ao valor da MST de aeroportos+estradas
    	entao e dado o output do valor da MST assim como o numero de aeroportos e estradas
    	a serem construidas, deste modo, caso o valor das MST for igual, damos prioridade
    	a rede que constroi o menor numero de aeroportos
    	Caso o valor da MST de estradas for superior ao da MST de aeroportos+estradas
    	entao e dado como output o valor da MST assim como o numero de aeroportos e estradas
    	a serem construidas e o programa termina.
    */
    if( kruskal_mst_estradas <= kruskal_mst_peso && InsuficienteEFlag!=1){
        cout << kruskal_mst_estradas <<endl;
        cout << OutAeroportosE << " " << OutEstradasE << endl;
        return 0;
    }else{
        cout << kruskal_mst_peso <<endl;
        cout << N_aeroportosAconstruirOUT << " " << N_estradasAconstruirOUT << endl;
        return 0;
    }
    return 0;
}

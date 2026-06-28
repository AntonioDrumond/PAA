# Trabalho Prático de Projeto e Análise de Algoritmos
Trabalho para a disciplina de PAA, do curso de Ciência da Computação da Pontifícia Universidade Católica de Minas Gerais. 
Professor: Sílvio Jamil Ferzoli Guimarães
Tema: Redução de Transitividade em Grafos

# Execução

Primeiro, baixe as dependências do visualizador de grafos com:
```fish
pip install -r visualizer/requirements.txt 
```

Defina o grafo no arquivo `graph.txt`

Exemplo:
```
0 1
0 2
1 2
2 0
```

Após isso, execute o código com:
```fish
.\run.bat # Windows
./run.sh  # Linux
```

Ou alternativamente:
```fish
# Linux
g++ ./src/main.cc -o ./src/main
./src/main > out.txt
python visualizer/visualizer.py out.txt

# Windows
g++ .\src\main.cc -o .\src\main.exe
.\src\main.exe > out.txt
python visualizer\visualizer.py out.txt
```

Também é possível gerar um grafos aleatórios e não-direcionados ao adicionar as flags `random` e `undirected` respectivamente.

```fish
./run.x random
./run.x undirected
./run.x random undirected

# Ou
./src/main random > out.txt
python visualizer\visualizer.py --undirected out.txt
```

## Contribuições

Embora as contribuições estejam organizadas por área de responsabilidade principal, todos os membros participaram ativamente do desenvolvimento do projeto como um todo, incluindo discussões, revisões e decisões conjuntas ao longo de todas as etapas.

- **Antônio Drumond Cota de Sousa:** implementação da estrutura de dados de grafo e algoritmos.
- **Davi Ferreira Puddo:** desenvolvimento e execução dos testes.
- **Gabriel Valedo Batista Silva:** implementação do gerador de grafos aleatórios.
- **João Pedro Torres:** redação do relatório.
- **Laura Menezes Heráclito Alves:** revisão bibliográfica
- **Lucas Carneiro Nassau Malta:** revisão bibliográfica.
- **Mateus Henrique Medeiros Diniz:** desenvolvimento e execução dos testes.
- **Raquel de Parde Motta:** redação do relatório.

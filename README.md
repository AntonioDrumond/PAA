# PAA
Trabalho para a disciplina de PAA, da universidade PUC Minas com o professor Sílvio Jamil Ferzoli Guimarães

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

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
g++ src/main.cc -o src/main
.\src\main | python visualizer\visualizer.py

# Windows
g++ src/main.cc -o src/main.exe
.\src\main.exe | python visualizer\visualizer.py
```

---

Também é possível gerar um grafo aleatório ao adicionar a flag `random`, como:

```fish
./run.sh random
# Ou
.\src\main.exe random | python visualizer\visualizer.py
```

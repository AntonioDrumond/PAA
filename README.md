# PAA
Trabalho para a disciplina de PAA, da universidade PUC Minas com o professor Sílvio Jamil Ferzoli Guimarães

# Execução

Primeiro, baixe as dependências do visualizador de grafos com:
```fish
pip install -r visualizer/requirements.txt 
```

Após isso, execute o código com:
```fish
.\run.bat # Windows
./run.sh  # Linux
```

Ou alternativamente:
```fish
g++ src/main.cc -o src/main.exe
.\src\main.exe | python visualizer\visualizer.py
```

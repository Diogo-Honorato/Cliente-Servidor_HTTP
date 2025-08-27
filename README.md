# Back-end

Servidor HTTP básico

## Comandos CMAKE

Todos os comandos serão feitos a partir do diretório `back-end`:


Compilar: `cmake --build build`


Executar: `./build/bin/tp`


### Atualizar o CMake

Caso exista adicione um novo arquivo fonte(.cpp) ao projeto executar o comando:

```
cd build
cmake ..
```

### Observação Importante: 
Se você executar o programa diretamente a partir do caminho `./build/bin/tp` (ou similar), lembre-se de que **os caminhos de arquivos usados dentro do código serão relativos à pasta atual de execução**.  

Isso significa que, ao usar caminhos relativos (como `../data/arquivo.txt`), o programa irá procurar a partir da pasta `build/bin/`  **e não da raiz do projeto**.

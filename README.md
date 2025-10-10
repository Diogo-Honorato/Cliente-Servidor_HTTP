# Cliente-Servidor HTTP Básico em C

Este projeto implementa uma aplicação básica Cliente-Servidor utilizando o protocolo HTTP em C. O cliente solicita um arquivo de uma URL e o servidor fornece esse arquivo para o cliente fazer o download.

>[Servidor](Server/README.md): O servidor HTTP escuta requisições em uma porta específica e, ao receber uma requisição com o caminho de um arquivo, ele retorna o conteúdo do arquivo ou um erro caso o arquivo não seja encontrado.

>[Cliente](Client/README.md): O cliente faz uma requisição HTTP para o servidor, solicitando um arquivo através de uma URL. O arquivo é então baixado para o diretório local especificado.

## Requisitos

- Sistema Operacional: Linux/Unix com suporte a compilação C.

- Compilador: GCC ou qualquer compilador C compatível.

- Ferramenta de Build: make (opcional, mas recomendado para facilitar a compilação).

- Bibliotecas: Nenhuma biblioteca externa necessária além das bibliotecas padrão.
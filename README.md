# 🧮 tonto-compiler

O **tonto-compiler** é um projeto desenvolvido para a matéria de Compiladores do curso de Ciência da Computação. Com objetivo de construção do front-end de um compilador para a linguagem TONTO (Textual Ontology Language) passando pela análise lexica, sintática e semântica.

## 🧑‍💻 Desenvolvedores

- <a href="https://github.com/eduardoaquinosilva">Eduardo Aquino</a>
- <a href="https://github.com/ratelp">Rafael Martinho</a>

## 🔍 Analisador Léxico

O analisador léxico, gerado com a ferramenta **Flex**, é a primeira fase do compilador. Ele é responsável por ler os arquivos de código-fonte em TONTO, identificar sequências de caracteres (lexemas) e convertê-las em uma série de *tokens*. Cada token representa uma unidade fundamental da linguagem.

Ao final da análise, o programa gera um arquivo `output.txt` que lista todos os tokens encontrados, informando o tipo do token, seu valor (lexema), quantidade de tokens de cada tipo, número da linha e coluna.

Os tokens são classificados nas seguintes categorias:

- **Palavras Reservadas**: Identificadores com significado fixo na linguagem, como `package`, `import`, `genset`, etc.
- **Esteriótipos de Classe**: Anotações que definem a natureza de uma classe, como `«kind»`, `«phase»`, `«role»`, etc.
- **Esteriótipos de Relação**: Anotações que definem a natureza de uma relação, como `«material»`, `«mediation»`, etc.
- **Nomes de Classes, Relações e Instâncias**: Identificadores definidos pelo usuário para nomear elementos do modelo.
- **Tipos de Dados**: Tipos primitivos da linguagem, como `string`, `number`, etc.
- **Novos tipos de dados**: Tipos que podem ser definidos por aquele cria o modelo em tonto.
- **Meta-atributos**: Atributos que descrevem propriedades de outros elementos, como `ordered`, `derived`.
- **Símbolos Especiais**: Caracteres com função estrutural, como chaves `{ }`, parênteses `( )`, etc.
- **Constantes Numéricas**: Números inteiros e de ponto flutuante, como `9`, `2.1`, `71`.

Essa categorização é definida no arquivo `tokens.h` e utilizada pelo analisador para processar o código-fonte.

## 🏗️ Analisador Sintático

-- Em construção -- 2° unidade

## 🧠 Analisador Semântico

-- Em construção -- 3° unidade

## ⚡ Tecnologias Utilizadas

- **Linguagem:** C++ - 13.3.0
- **Analisador léxico:** Flex - 2.6.4
- **Automatizador de compilação:** CMAKE - 3.28.3

## 🛠️ Instruções de Setup

```bash
Linux

git clone https://github.com/eduardoaquinosilva/tonto-compiler.git

cd tonto-compiler

# dependências
sudo apt install g++ gdb make cmake flex libfl-dev

mkdir -p Build

cd Build

cmake ..

make

# caso deseje alterar os arquivos de testes somente substituir os arquivos de exemplo
# ou colocar na linha de comando colocando arquivo pasta tonto-compiler
./tonto-compiler ../teste.tonto ../testeT.txt
# para funcionar da maneira informada é necessário estar na pasta Build anteriormente criada
```

## 📤 Exemplo de Saída

```
Arquivo ../teste.txt:
<PACKAGE> | Linha 1 | Coluna 1
<CLASS_NAME, "CarOwnership"> | Linha 1 | Coluna 9
<CLASS_STEREOTYPE, "kind"> | Linha 3 | Coluna 1
<CLASS_NAME, "Organization"> | Linha 3 | Coluna 6
<CLASS_STEREOTYPE, "subkind"> | Linha 4 | Coluna 1
<CLASS_NAME, "CarAgency"> | Linha 4 | Coluna 9
<RELATION_NAME, "specializes"> | Linha 4 | Coluna 19
<CLASS_NAME, "Organization"> | Linha 4 | Coluna 31
<CLASS_STEREOTYPE, "kind"> | Linha 5 | Coluna 1
<CLASS_NAME, "Car"> | Linha 5 | Coluna 6

...

Quantidade de cada token identificados:
Classes: 8 | Relações: 4 | Palavras Reservadas: 1 | Instâncias: 0 | Esteriótipos de Classes: 3 | Esteriótipos de Relações: 2 | Meta atributos: 0 | Tipos: 0 | Novos tipos: 0 | Simbolos especiais: 8

Arquivo ../testeT.txt:
<PACKAGE> | Linha 13 | Coluna 1
<CLASS_NAME, "Car"> | Linha 13 | Coluna 9

Quantidade de cada token identificados:
Classes: 1 | Relações: 0 | Palavras Reservadas: 1 | Instâncias: 0 | Esteriótipos de Classes: 0 | Esteriótipos de Relações: 0 | Meta atributos: 0 | Tipos: 0 | Novos tipos: 0 | Simbolos especiais: 0

-------------------------------
Total de cada token identificado:
Classes: 9 | Relações: 4 | Palavras Reservadas: 2 | Instâncias: 0 | Esteriótipos de Classes: 3 | Esteriótipos de Relações: 2 | Meta atributos: 0 | Tipos: 0 | Novos tipos: 0 | Simbolos especiais: 8
```
# 🧮 tonto-compiler

O **tonto-compiler** é um projeto desenvolvido para a matéria de Compiladores do curso de Ciência da Computação. Com objetivo de construção do front-end de um compilador para a linguagem TONTO (Textual Ontology Language) passando pela análise lexica, sintática e semântica.

## 🔍 Analisador Léxico

O analisador léxico, gerado com a ferramenta **Flex**, é a primeira fase do compilador. Ele é responsável por ler os arquivos de código-fonte em TONTO, identificar sequências de caracteres (lexemas) e convertê-las em uma série de *tokens*. Cada token representa uma unidade fundamental da linguagem.

Ao final da análise, o programa gera um arquivo `output.txt` que lista todos os tokens encontrados, informando o tipo do token, seu valor (lexema), número da linha e coluna.

Os tokens são classificados nas seguintes categorias:

- **Palavras Reservadas**: Identificadores com significado fixo na linguagem, como `package`, `import`, `genset`, etc.
- **Esteriótipos de Classe**: Anotações que definem a natureza de uma classe, como `«kind»`, `«phase»`, `«role»`, etc.
- **Esteriótipos de Relação**: Anotações que definem a natureza de uma relação, como `«material»`, `«mediation»`, etc.
- **Nomes de Classes, Relações e Instâncias**: Identificadores definidos pelo usuário para nomear elementos do modelo.
- **Tipos de Dados**: Tipos primitivos da linguagem, como `string`, `number`, etc.
- **Novos tipos de dados**: Tipos que podem ser definidos por aquele cria o modelo em tonto.
- **Meta-atributos**: Atributos que descrevem propriedades de outros elementos, como `ordered`, `derived`.
- **Símbolos Especiais**: Caracteres com função estrutural, como chaves `{ }`, parênteses `( )`, vírgulas `,` , etc.

Essa categorização é definida no arquivo `tokens.h` e utilizada pelo analisador para processar o código-fonte.

## 🏗️ Analisador Sintático

-- Em construção -- 2° unidade

## 🧠 Analisador Semântico

-- Em construção -- 3° unidade

## ⚡ Tecnologias Utilizadas

- **Linguagem:** C++ - 13.3.0
- **Analisador léxico** Flex - 2.6.4

## 🛠️ Instruções de Setup

```bash

git clone https://github.com/eduardoaquinosilva/tonto-compiler.git

cd tonto-compiler

mkdir -p Build

cd Build

cmake ..

make

# caso deseje alterar os arquivos de testes somente substituir os arquivos de exemplo
# ou colocar na linha de comando colocando arquivo pasta tonto-compiler
./tonto-compiler ../teste.txt ../testeT.txt

```

## 🧑‍💻 Desenvolvedores

- <a href="https://github.com/eduardoaquinosilva">Eduardo Aquino</a>
- <a href="https://github.com/ratelp">Rafael Martinho</a>
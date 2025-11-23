# 🧮 tonto-compiler

O **tonto-compiler** é um projeto desenvolvido para a matéria de Compiladores do curso de Ciência da Computação. Com objetivo de construção do front-end de um compilador para a linguagem TONTO (Textual Ontology Language) passando pela análise lexica, sintática e semântica.

## 🧑‍💻 Desenvolvedores

- <a href="https://github.com/eduardoaquinosilva">Eduardo Aquino</a>
- <a href="https://github.com/ratelp">Rafael Martinho</a>

## 🔍 Analisador Léxico

O analisador léxico, gerado com a ferramenta **Flex**, é a primeira fase do compilador. Ele é responsável por ler os arquivos de código-fonte em TONTO, identificar sequências de caracteres (lexemas) e convertê-las em uma série de *tokens*. Cada token representa uma unidade fundamental da linguagem.

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

## 🏗️ Analisador Sintático

O analisador sintático, gerado com a ferramenta **Bison**, é a segunda fase do compilador. Ele recebe a sequência de *tokens* do analisador léxico e verifica se essa sequência forma uma estrutura gramaticalmente válida de acordo com as regras da linguagem TONTO. Sua principal função é construir uma representação da estrutura do código-fonte e validar a sintaxe.

Ao final de uma análise bem-sucedida, o programa atualiza o arquivo `output.txt` com um **Relatório de Análise Sintática** (`Parsing Report`). Este relatório resume as principais estruturas de alto nível identificadas no código, como:

- **Packages**: O contêiner principal do modelo.
- **Classes**: As entidades fundamentais do modelo, incluindo seus estereótipos.
- **Tipos de Dados**: Definições de novos tipos de dados estruturados.
- **Enums**: Listas de valores nomeados.
- **Conjuntos de Generalização (Generalization Sets)**: Estruturas que definem hierarquias de especialização/generalização entre classes.
- **Relações**: Conexões entre classes, sejam elas internas ou externas, com seus respectivos estereótipos e cardinalidades.

## 🧠 Analisador Semântico

-- Em construção -- 3° unidade

## ⚡ Tecnologias Utilizadas

- **Linguagem:** C++ - 13.3.0*
- **Analisador léxico:** Flex - 2.6.4
- **Analisador Sintático:** Bison - 3.8.2
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
./tonto-compiler ../teste.tonto ../Car.tonto
# para funcionar da maneira informada é necessário estar na pasta Build anteriormente criada
```

## 📤 Exemplo de Saída

```
--- Parsing Report ---
Packages (2):
  - Person
  - Car
Classes (2):
  - Persona
  - PersonaErros
New Data Types (1):
  - AddressDataType
Enums (1):
  - EyeColor
Generalization Sets (2):
  - Set Name: PersonAgeGroup
    Parent: Person
    Children: Child Adult 
  - Set Name: PersonAgeGroup
    Parent: Person
    Children: Child Teenager Adult 
Relations (3):
  - [Internal] | Persona (<<componentOf>> has) -> Department
  - [External] | EmploymentContract (<<mediation>>) -> Employee
  - [Internal] | PersonaErros (<<componentOf>> has) -> Department

```
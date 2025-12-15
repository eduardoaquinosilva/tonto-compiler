# 🧮 tonto-compiler

O **tonto-compiler** é um projeto desenvolvido para a matéria de Compiladores do curso de Ciência da Computação. Com o objetivo de construção do front-end de um compilador para a linguagem TONTO (Textual Ontology Language) passando pela análise léxica, sintática e semântica.

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
- **Novos tipos de dados**: Tipos que podem ser definidos por aquele que cria o modelo em tonto.
- **Meta-atributos**: Atributos que descrevem propriedades de outros elementos, como `ordered`, `derived`.
- **Símbolos Especiais**: Caracteres com função estrutural, como chaves `{ }`, parênteses `( )`, etc.
- **Constantes Numéricas**: Números inteiros e de ponto flutuante, como `9`, `2.1`, `71`.

## 🏗️ Analisador Sintático

O analisador sintático, gerado com a ferramenta **Bison**, é a segunda fase do compilador. Ele recebe a sequência de *tokens* do analisador léxico e verifica se essa sequência forma uma estrutura gramaticalmente válida de acordo com as regras da linguagem TONTO. Sua principal função é construir uma representação da estrutura do código-fonte e validar a sintaxe.

As principais estruturas identificadas são classificadas nas seguintes categorias:

- **Packages**: O contêiner principal do modelo.
- **Classes**: As entidades fundamentais do modelo, incluindo seus estereótipos.
- **Tipos de Dados**: Definições de novos tipos de dados estruturados.
- **Enums**: Listas de valores nomeados.
- **Conjuntos de Generalização (Generalization Sets)**: Estruturas que definem hierarquias de especialização/generalização entre classes.
- **Relações**: Conexões entre classes, sejam elas internas ou externas, com seus respectivos estereótipos e cardinalidades.

## 🧠 Analisador Semântico

O analisador semântico é a terceira fase do compilador. Ele utiliza a tabela de símbolos construída durante a leitura dos arquivos nas fases anteriores e verifica se as estruturas construídas seguem as regras da linguagem TONTO que não foram possíveis de descrever em termos da gramática utilizada pelo analisador sintático. Sua principal função é validar as relações de significado entre as estruturas e identificar os principais padrões de projeto de ontologias.

Ao final da análise, o programa atualiza o arquivo `output.txt` com um **Relatório de Análise Semântica** (`Semantic Report`). Este relatório resume os principais padrões de projeto de ontologias de alto nível encontrados no código, informando quais foram encontrados de maneira completa e quais foi necessário aplicar coerção em determinados elementos da estrutura, como:

- **Subkind Pattern**: Define as relações semânticas do estereótipo `subkind`, validando os estereótipos das superclasses e fornecendo coerções cabíveis.
- **Role Pattern**: Define as relações semânticas do estereótipo `role`, validando os estereótipos das superclasses e fornecendo coerções cabíveis.
- **Phase Pattern**: Define as relações semânticas do estereótipo `phase`, validando os estereótipos das superclasses e a presença de classes irmãs, fornecendo coerções cabíveis.
- **Relator Pattern**:
- **Mode Pattern**:
- **RoleMixin Pattern**:

É na fase da análise semântica que os vários arquivos começam a ser tratados como partes de um único projeto, permitindo a correta importação de classes declaradas em arquivos externos ao que as está utilizando.

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

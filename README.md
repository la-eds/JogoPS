# JogoPS
# 🧩 Jogo da Palavra Secreta

Um jogo educativo desenvolvido em **C (back-end)** e **C# (front-end)**, onde o jogador precisa adivinhar uma palavra oculta com base em uma dica e dentro de um tempo limite.

---

## 🎯 Objetivo do Projeto

Este projeto foi desenvolvido como parte da disciplina de **Algoritmos e Programação**, com o objetivo de aplicar:

- Lógica de programação estruturada em **C**
- Criação de interface gráfica em **C# (WinForms)**
- Integração entre linguagens via **DLL**
- Manipulação de arquivos texto e strings
- Gerenciamento de tempo, tentativas e erros

---

## 🏗️ Estrutura do Projeto

```
📁 JogoDaPalavraSecreta/
│
├── 🧠 JogoBackEnd/            # Projeto em C
│   ├── funcoes.h              # Cabeçalhos das funções exportadas
│   ├── jogo_backend.c         # Implementação da lógica do jogo
│   ├── palavras.txt           # Banco de palavras e dicas
│   └── JogoBackEnd.dll        # Biblioteca compilada para uso no front-end
│
├── 💻 JogoFrontEnd/           # Projeto em C# (Windows Forms)
│   ├── Form1.cs               # Interface principal do jogo
│   ├── Backend.cs             # Comunicação com a DLL em C
│   └── Program.cs             # Inicialização do aplicativo
│
└── 📄 README.md               # Documentação do projeto
```

---

## ⚙️ Funcionamento Geral

### 🔹 **Back-End (C)**
Responsável pela lógica do jogo:
- Seleciona aleatoriamente uma palavra e uma dica do arquivo `palavras.txt`
- Armazena o estado atual (acertos, erros, tentativas)
- Processa cada tentativa de letra
- Retorna resultados (vitória, derrota, andamento) para o front-end

O código C é compilado como uma **biblioteca dinâmica (.dll)** para ser usada pelo front-end.

---

### 🔹 **Front-End (C#)**
Responsável pela interface e interatividade:
- Exibe dica, palavra mascarada e status
- Recebe entrada de letra
- Controla o cronômetro regressivo (30s)
- Mostra telas de vitória, derrota e fim de tempo
- Comunica-se com a DLL via `DllImport`

---

## 🧩 Estrutura do Arquivo de Palavras (`palavras.txt`)

Cada linha contém **palavra** e **dica**, separadas por `;`:

```
cachorro;Animal doméstico
vento;Ar em movimento
floresta;Conjunto de árvores
oceano;Grande massa de água salgada
sol;Estrela do sistema solar
```

---

## 🕹️ Controles do Jogo

| Ação | Descrição |
|------|------------|
| 📝 Digitar letra | Insira uma letra no campo de texto e clique **Tentar** |
| ⏱️ Tempo | 30 segundos por rodada |
| ✅ Vitória | Ao completar todas as letras da palavra |
| ❌ Derrota | Ao esgotar tentativas ou tempo |

---

## 🧑‍💻 Equipe / Autor
**Desenvolvido por:** Lucas Altieri  
**Disciplina:** Algoritmos e Programação II  
**Ano:** 2025

---

## 📜 Licença
Este projeto foi desenvolvido para fins educacionais.  
Uso livre mediante citação do autor.

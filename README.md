JOGO DE PERGUNTAS E RESPOSTAS DA MARVEL (C)
============================================

1. SOBRE O JOGO
-----------------

Este e um jogo de perguntas e respostas (quiz) no terminal, baseado no universo Marvel. O objetivo e responder corretamente a 10 perguntas principais para ajudar os Vingadores a impedir a ameaca de Thanos.

O jogo apresenta:
* 10 Perguntas Principais que valem pontos.
* Vidas: Voce comeca com 2 vidas. Perder uma pergunta custa 1 vida.
* Acoes Especiais (Uso Unico):
    * Dica: Receba uma pista sobre a pergunta.
    * Pulo: Pule a pergunta atual sem perder vida ou ponto.
    * Troca: Substitua a pergunta atual por uma pergunta secundaria (desde que haja perguntas secundarias disponiveis no arquivo perguntas.json).

2. COMO JOGAR
-----------------

1. Inicio: O jogo inicia mostrando a introducao e o Menu principal.
2. Acoes: Durante o jogo, voce tera as seguintes opcoes:
    * 1: Responder a pergunta.
    * 2: Usar sua unica Dica.
    * 3: Usar sua unica Troca de pergunta.
    * 4: Usar seu unico Pulo de questao.
    * 5: Desistir da partida.
3. Respostas: Ao escolher 1=Responder, digite o numero da alternativa correta (1, 2, 3 ou 4) e pressione ENTER.
4. Fim de Jogo: O jogo termina se voce responder as 10 perguntas ou se suas vidas chegarem a zero.

3. COMO COMPILAR E EXECUTAR
---------------------------------

Para compilar e executar este jogo, voce precisara ter o compilador GCC instalado em seu sistema.

PRE-REQUISITO
-------------
Alem do arquivo jogomarvel.c, voce deve ter um arquivo de dados chamado perguntas.json no mesmo diretorio, contendo a estrutura de 20 ou mais perguntas.

3.1. Compilacao no Windows (Usando MinGW/GCC)
---------------------------------------------
O codigo utiliza o cabecalho <windows.h> para a funcao Sleep(), que e especifica do Windows.

1. Abra o Prompt de Comando (CMD) ou PowerShell no diretorio onde estao jogomarvel.c e perguntas.json.
2. Execute o seguinte comando de compilacao:

   gcc jogomarvel.c -o jogomarvel.exe

3. Executar: Para rodar o jogo:

   .\jogomarvel.exe

3.2. Compilacao no Linux/macOS
--------------------------------
No Linux/macOS, e necessario remover a inclusao de <windows.h> e a chamada Sleep(1000) para evitar erros.

1. Abra o Terminal no diretorio do projeto.
2. Execute o comando de compilacao padrao:

   gcc jogomarvel.c -o jogomarvel

3. Executar: Para rodar o jogo:

   ./jogomarvel

4. CREDITOS
--------------------------------
Autores Oficiais do Codigo: Anita Silva, Emanuel Veloso, Felipe Andrade, Felipe Muniz e Joana Karoliny.
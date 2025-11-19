#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#define MAX_PERGUNTAS 100

typedef struct {
    char questao[300];
    char alternativas[4][150];
    int resposta;
    char dica[200];
    int nivel;   // 1 = principal, 2 = secundária (só usada na troca)
} Pergunta;

#ifdef _WIN32
#define LIMPAR "cls"
#else
#define LIMPAR "clear"
#endif

void limparTela() {
    system(LIMPAR);
}

void pausa() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

/* ---------- LEITURA DE ARQUIVO SIMPLES ---------- */
char *lerArquivoTexto(const char *nome) {
    FILE *f = fopen(nome, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buf = malloc(tam + 1);
    fread(buf, 1, tam, f);
    buf[tam] = '\0';
    fclose(f);
    return buf;
}

/* ---------- EXTRATOR DE STRING ---------- */
int extrairString(const char *txt, const char *chave, int pos, char *dest, int max) {
    const char *p = strstr(txt + pos, chave);
    if (!p) return -1;

    p = strchr(p, ':');
    while (*p && *p != '"') p++;
    p++;

    const char *q = p;
    while (*q && *q != '"') q++;

    int len = q - p;
    if (len >= max) len = max - 1;

    strncpy(dest, p, len);
    dest[len] = '\0';

    return q - txt;
}

/* ---------- EXTRATOR DE ALTERNATIVAS ---------- */
int extrairAlternativas(const char *txt, int pos, char dest[4][150]) {
    const char *p = strstr(txt + pos, "\"alternativas\"");
    p = strchr(p, '[') + 1;

    int i = 0;
    while (*p && *p != ']' && i < 4) {
        while (*p && *p != '"') p++;
        p++;

        const char *q = p;
        while (*q && *q != '"') q++;

        int len = q - p;
        if (len > 149) len = 149;

        strncpy(dest[i], p, len);
        dest[i][len] = '\0';

        p = q + 1;
        i++;
    }
    return i;
}

/* ---------- EXTRATOR DE INTEIRO ---------- */
int extrairInt(const char *txt, const char *chave, int pos, int *out) {
    const char *p = strstr(txt + pos, chave);
    p = strchr(p, ':') + 1;

    sscanf(p, "%d", out);
    while (*p >= '0' && *p <= '9') p++;

    return p - txt;
}

/* ---------- CARREGAR JSON ---------- */
int carregarPerguntasJSON(const char *arquivo, Pergunta perguntas[]) {

    char *txt = lerArquivoTexto(arquivo);
    if (!txt) return 0;

    int pos = 0, count = 0;

    while (txt[pos]) {
        const char *obj = strstr(txt + pos, "{");
        if (!obj) break;

        int base = obj - txt;

        extrairString(txt, "\"questao\"", base, perguntas[count].questao, 300);
        extrairAlternativas(txt, base, perguntas[count].alternativas);
        extrairInt(txt, "\"resposta\"", base, &perguntas[count].resposta);
        extrairString(txt, "\"dica\"", base, perguntas[count].dica, 200);

        /* DEFINE NÍVEL AUTOMÁTICO */
        perguntas[count].nivel = (count < 10 ? 1 : 2);

        count++;
        pos = base + 1;
    }

    free(txt);
    return count;
}

/* -------- INTRODUÇÃO --------*/
void mostrarIntroducao() {
    limparTela();
    printf("=== MISSÃO MARVEL ===\n\n");

    printf("O universo está em perigo. Thanos reuniu as Joias do Infinito e ameaça destruir tudo o que conhecemos.\n");
    printf("Os Vingadores contam com você, herói, para impedir que a calamidade aconteça.\n\n");

    printf("Cada pergunta respondida corretamente fortalece a equipe.\n");
    printf("Mas cuidado: o tempo está se esgotando, e apenas uma dica, um pulo e uma troca estão disponíveis — use-os com sabedoria.\n\n");

    printf("Prepare-se. Sua missão começa agora.\n");
    printf("Que a coragem dos Vingadores guie suas escolhas!\n\n");

    printf("Pressione ENTER para acessar o menu e iniciar sua missão...");
    getchar();  // espera ENTER corretamente
}





/* ---------- REGRAS ---------- */
void mostrarRegras() {
    limparTela();
    printf("===== REGRAS DO JOGO =====\n");
    printf("- O mundo está em perigo, para ajudar os vingadores você deve responder 10 perguntas segindo as seguintes regras:\n");
    printf("1- O jogo se inicia com 2 vidas\n");
    printf("2- Você possui 3 açoes especiais:\n");
    printf("- 1 pulo de questão\n");
    printf("- 1 troca de pergunta\n");
    printf("- 1 dica\n\n");
    printf("1) Voltar para menu princípal\nEscolha: ");
    int x; scanf("%d", &x); getchar();
}

/* ==========================================================
                     JOGO PRINCIPAL
   ========================================================== */

int main() {

    limparTela();
    mostrarIntroducao();

    printf("=== MISSÃO MARVEL ===\n\n");


    Pergunta perguntas[MAX_PERGUNTAS];
    int total = carregarPerguntasJSON("perguntas.json", perguntas);

    if (total < 20) {
        printf("ERRO: São necessárias 10 principais + 10 secundárias.\n");
        return 1;
    }

    while (1) {

        printf("\nMENU:\n1 - Jogar\n2 - Regras\n3 - Sair\nEscolha: ");
        int menu; scanf("%d", &menu); getchar();

        if (menu == 3) break;
        if (menu == 2) { mostrarRegras(); continue; }
        if (menu != 1) continue;

        /* --- VARIÁVEIS DA PARTIDA --- */
        int vidas = 2, pontos = 0;
        bool dicaUsada = false;
        bool puloUsado = false;
        bool trocaUsada = false;

        int idxSecundaria = 10; // começa depois das 10 principais

        /* ------------ LOOP DAS 10 PERGUNTAS PRINCIPAIS ------------ */
        for (int i = 0; i < 10 && vidas > 0; i++) {

REPETE:

            limparTela();

            printf("Vidas: %d | Dica: %d | Pulo: %d | Troca: %d\n",
                   vidas,
                   dicaUsada ? 0 : 1,
                   puloUsado ? 0 : 1,
                   trocaUsada ? 0 : 1);

            printf("\nPergunta %d/10\n", i+1);
            printf("Nível: %d\n\n", perguntas[i].nivel);
            printf("%s\n\n", perguntas[i].questao);

            for (int a = 0; a < 4; a++)
                printf("%d) %s\n", a+1, perguntas[i].alternativas[a]);

            printf("\n1=Responder  2=Dica  3=Trocar  4=Pular  5=Desistir\nEscolha: ");
            int esc; scanf("%d", &esc); getchar();

            /* === DICA === */
            if (esc == 2) {
                if (dicaUsada) {
                    printf("\nVocê já usou sua dica!\n");
                    pausa();
                    goto REPETE;
                }

                dicaUsada = true;
                printf("\n Dica: %s\n", perguntas[i].dica);
                pausa();
                goto REPETE;
            }

            /* === PULAR === */  
            if (esc == 4) {
                if (puloUsado) {
                    printf("\nVocê já usou o pulo!\n");
                    pausa();
                    goto REPETE;
                }
                puloUsado = true;
                continue;  // vai para a próxima pergunta
            }

            /* === TROCAR === */
            if (esc == 3) {
                if (trocaUsada) {
                    printf("\nVocê já usou a troca!\n");
                    pausa();
                    goto REPETE;
                }

                if (idxSecundaria >= total) {
                    printf("\nSem perguntas secundárias restantes!\n");
                    pausa();
                    goto REPETE;
                }

                trocaUsada = true;

                Pergunta temp = perguntas[i];
                perguntas[i] = perguntas[idxSecundaria];
                perguntas[idxSecundaria] = temp;

                idxSecundaria++;

                printf("\nPergunta substituída!\n");
                pausa();
                goto REPETE;
            }

            /* === DESISTIR === */
            if (esc == 5) break;

            /* === RESPONDER === */
            if (esc == 1) {
                printf("\nDigite sua resposta (A - D): ");
                int r; scanf("%d", &r); getchar();

                bool acertou = (r == perguntas[i].resposta);

                if (acertou) {
                    if (perguntas[i].nivel == 1) pontos++;
                    printf("\n Correto!\n");
                    Sleep(1000);
                } else {
                    vidas--;
                    printf("\n Errado! Resposta correta: %d) %s\n",
                           perguntas[i].resposta,
                           perguntas[i].alternativas[perguntas[i].resposta - 1]);
                }

                pausa();
            }
        }


/* ----------- RESULTADO FINAL ----------- */
limparTela();
printf("===== RESULTADO =====\n");
printf("Pontuação final: %d\n\n", pontos);

/* ----  VITÓRIA / DERROTA ---- */

if (pontos >= 7) {
    printf("Impressionante! Você alcançou %d pontos!\n", pontos);
    printf("A força dos Vingadores corre em suas veias. Sua coragem virou o jogo.\n");
    printf("A ameaça foi contida e o universo respira aliviado graças às suas decisões.\n");
}
else if (pontos <= -7) {
    printf("Você fez %d pontos.\n", pontos);
    printf("Thanos avançou como uma tempestade inevitável. Mesmo com bravura, você não conseguiu impedi-lo.\n");
    printf("Mas os Vingadores nunca desistem. A galáxia ainda precisa de você.\n");
}
else {
    printf("Você fez %d pontos.\n", pontos);
    printf("Você lutou bem, mas o poder das Joias do Infinito exige ainda mais determinação.\n");
    printf("Continue ao lado dos Vingadores e volte mais forte. A próxima batalha pode mudar tudo.\n");
}

pausa();
}


return 0;
}

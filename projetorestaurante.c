#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- ENUM PARA TIPO DE ITEM ---
typedef enum {
    REFEICAO_KG,
    QUENTINHA,
    BEBIDA
} Cardapio;

// --- STRUCT DE ITEM INDIVIDUAL ---
typedef struct {
    Cardapio tipo;
    float peso;
    float preco_unitario;
    int quantidade;
    float valor_total_item;
} ItemVenda;

#define MAX_VENDAS_POR_DIA 150

// --- STRUCT DE REGISTRO DIÁRIO ---
typedef struct {
    int dia;
    int mes;
    int ano;
    ItemVenda vendas[MAX_VENDAS_POR_DIA];
    int num_vendas;
    float total_dia;
} RegistroDiario;

#define MAX_DIAS_MES 31
#define MAX_MESES_ANO 12

RegistroDiario registros_diarios[MAX_DIAS_MES * MAX_MESES_ANO];
int num_registros_diarios = 0;

float totais_mensais[MAX_MESES_ANO] = {0.0};

typedef struct {
    int mes;
    float total_vendido;
} VendaMensal;

VendaMensal vendas_anuais[MAX_MESES_ANO];

// --- PROTÓTIPOS ---
void exibir_menu_principal();
void registrar_venda();
void gerar_relatorio_diario();
void gerar_relatorio_mensal();
void gerar_relatorio_anual_ordenado();
ItemVenda criar_refeicao_kg();
ItemVenda criar_quentinha();
ItemVenda criar_bebida();

// --- MAIN ---
int main() {
    int opcao;
    do {
        exibir_menu_principal();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: registrar_venda(); break;
            case 2: gerar_relatorio_diario(); break;
            case 3: gerar_relatorio_mensal(); break;
            case 4: gerar_relatorio_anual_ordenado(); break;
            case 0: printf("Saindo do programa. Obrigado!\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
    return 0;
}

// --- FUNÇÕES ---

void exibir_menu_principal() {
    printf("\n--- SISTEMA DE VENDAS DO RESTAURANTE ---\n");
    printf("1. Registrar nova venda\n");
    printf("2. Gerar relatorio diario\n");
    printf("3. Gerar relatorio mensal\n");
    printf("4. Gerar relatorio anual ordenado\n");
    printf("0. Sair\n");
    printf("----------------------------------------\n");
}

ItemVenda criar_refeicao_kg() {
    ItemVenda item;
    item.tipo = REFEICAO_KG;
    const float PRECO_KG_FIXO = 50.00;
    float peso;
    printf("Digite o peso da refeicao (Kg): ");
    scanf("%f", &peso);
    item.peso = peso;
    item.preco_unitario = PRECO_KG_FIXO;
    item.quantidade = 0;
    item.valor_total_item = peso * PRECO_KG_FIXO;
    printf("Refeicao registrada. Valor: R$ %.2f\n", item.valor_total_item);
    return item;
}

ItemVenda criar_quentinha() {
    ItemVenda item;
    item.tipo = QUENTINHA;
    item.peso = 0;
    item.quantidade = 0;
    item.preco_unitario = 20.00;
    item.valor_total_item = 20.00 + 0.50;
    printf("Quentinha registrada. Valor: R$ %.2f\n", item.valor_total_item);
    return item;
}

ItemVenda criar_bebida() {
    ItemVenda item;
    item.tipo = BEBIDA;
    item.peso = 0;
    float preco;
    int qtd;
    printf("Digite o preco unitario da bebida: R$ ");
    scanf("%f", &preco);
    printf("Digite a quantidade: ");
    scanf("%d", &qtd);
    item.preco_unitario = preco;
    item.quantidade = qtd;
    item.valor_total_item = preco * qtd;
    printf("Bebida registrada. Valor: R$ %.2f\n", item.valor_total_item);
    return item;
}

void registrar_venda() {
    int dia, mes, ano, tipo;
    printf("\nDigite o dia da venda (1-31): "); scanf("%d", &dia);
    printf("Digite o mes da venda (1-12): "); scanf("%d", &mes);
    printf("Digite o ano da venda: "); scanf("%d", &ano);

    if (dia < 1 || dia > 31 || mes < 1 || mes > 12) {
        printf("Data invalida!\n"); return;
    }

    int indice = -1;
    for (int i = 0; i < num_registros_diarios; i++) {
        if (registros_diarios[i].dia == dia &&
            registros_diarios[i].mes == mes &&
            registros_diarios[i].ano == ano) {
            indice = i; break;
        }
    }

    if (indice == -1) {
        indice = num_registros_diarios++;
        registros_diarios[indice] = (RegistroDiario){dia, mes, ano, {}, 0, 0.0};
    }

    while (1) {
        printf("\n1. Refeicao por Kg\n2. Quentinha\n3. Bebida\n0. Finalizar Venda\nOpcao: ");
        scanf("%d", &tipo);
        if (tipo == 0) break;
        if (registros_diarios[indice].num_vendas >= MAX_VENDAS_POR_DIA) {
            printf("Limite de vendas atingido.\n"); break;
        }
        ItemVenda v;
        switch (tipo) {
            case 1: v = criar_refeicao_kg(); break;
            case 2: v = criar_quentinha(); break;
            case 3: v = criar_bebida(); break;
            default: printf("Tipo invalido!\n"); continue;
        }
        registros_diarios[indice].vendas[registros_diarios[indice].num_vendas++] = v;
        registros_diarios[indice].total_dia += v.valor_total_item;
        totais_mensais[mes - 1] += v.valor_total_item;
    }
}

void gerar_relatorio_diario() {
    int dia, mes, ano, indice = -1;
    printf("\nRelatorio Diario - Digite dia, mes e ano:\n");
    scanf("%d%d%d", &dia, &mes, &ano);

    for (int i = 0; i < num_registros_diarios; i++) {
        if (registros_diarios[i].dia == dia && registros_diarios[i].mes == mes && registros_diarios[i].ano == ano) {
            indice = i; break;
        }
    }

    if (indice == -1) {
        printf("Nenhum registro neste dia.\n"); return;
    }

    printf("\n--- VENDAS EM %02d/%02d/%04d ---\n", dia, mes, ano);
    printf("%-15s %-10s %-10s %s\n", "Tipo", "Peso/Qtd", "Preco", "Total");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < registros_diarios[indice].num_vendas; i++) {
        ItemVenda v = registros_diarios[indice].vendas[i];
        switch (v.tipo) {
            case REFEICAO_KG:
                printf("%-15s %-10.2f %-10.2f R$ %.2f\n", "Refeicao (Kg)", v.peso, v.preco_unitario, v.valor_total_item);
                break;
            case QUENTINHA:
                printf("%-15s %-10s %-10s R$ %.2f\n", "Quentinha", "--", "--", v.valor_total_item);
                break;
            case BEBIDA:
                printf("%-15s %-10d %-10.2f R$ %.2f\n", "Bebida", v.quantidade, v.preco_unitario, v.valor_total_item);
                break;
        }
    }
    printf("-----------------------------------------------\n");
    printf("TOTAL DO DIA: R$ %.2f\n", registros_diarios[indice].total_dia);
}

void gerar_relatorio_mensal() {
    const char *meses[] = {"Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"};
    float total = 0.0;
    printf("\n--- RELATORIO MENSAL ---\n");
    for (int i = 0; i < MAX_MESES_ANO; i++) {
        printf("%-10s R$ %.2f\n", meses[i], totais_mensais[i]);
        total += totais_mensais[i];
    }
    printf("---------------------------\nTOTAL ANUAL: R$ %.2f\n", total);
}

void gerar_relatorio_anual_ordenado() {
    const char *meses[] = {"Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"};
    for (int i = 0; i < MAX_MESES_ANO; i++) {
        vendas_anuais[i].mes = i + 1;
        vendas_anuais[i].total_vendido = totais_mensais[i];
    }
    for (int i = 0; i < MAX_MESES_ANO - 1; i++) {
        for (int j = 0; j < MAX_MESES_ANO - 1 - i; j++) {
            if (vendas_anuais[j].total_vendido < vendas_anuais[j+1].total_vendido) {
                VendaMensal temp = vendas_anuais[j];
                vendas_anuais[j] = vendas_anuais[j+1];
                vendas_anuais[j+1] = temp;
            }
        }
    }
    printf("\n--- RELATORIO ANUAL ORDENADO ---\n");
    for (int i = 0; i < MAX_MESES_ANO; i++) {
        int m = vendas_anuais[i].mes - 1;
        printf("%-10s R$ %.2f\n", meses[m], vendas_anuais[i].total_vendido);
    }
}

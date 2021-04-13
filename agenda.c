#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//definicao das variaveis principais
typedef char texto[80];

texto *nome;
texto *endereco;
double *telefone_res;
double *telefone_cel;
int *data_nascimento;

char linhas_arq[500] = "";

char continuar[1];

int quantos_c;

//funcao para aplicar o malloc nas variaveis principais
void malloc_contato(int tam)
{
    nome = (texto *)malloc(tam * sizeof(texto));
    endereco = (texto *)malloc(tam * sizeof(texto));
    telefone_res = (double *)malloc(tam * sizeof(double));
    telefone_cel = (double *)malloc(tam * sizeof(double));
    data_nascimento = (int *)malloc(tam * sizeof(int));
}

//contar o primeiro numero do arquivo
void quantos_cont()
{
    FILE *arq = fopen("Contatos.txt", "r");
    if (arq == NULL)
    {
        quantos_c = 0;
        malloc_contato(10);
        return;
    }
    else
        fscanf(arq, "%d\n", &quantos_c);
    malloc_contato(10 + quantos_c);

    fclose(arq);
}

//aumentar e diminuir o primeiro numero do arquivo
void atualiza_quantos()
{
    FILE *input = fopen("Contatos.txt", "r");
    FILE *output = fopen("quantos-transf.txt", "w");

    fprintf(output, "%d\n", quantos_c);

    unsigned int linha_selecionada = 1;
    unsigned int linha_atual = 1;
    while (fgets(linhas_arq, 500, input) != NULL)
    {
        if (linha_atual != linha_selecionada)
        {
            fputs(linhas_arq, output);
        }
        memset(linhas_arq, 0, sizeof(char) * 500);
        linha_atual += 1;
    }
    fclose(input);
    fclose(output);
    remove("Contatos.txt");
    rename("quantos-transf.txt", "Contatos.txt");
}

//adicionar novos contatos
void adicionar(int alterar)
{
    system("cls");
    quantos_c++;
    atualiza_quantos();

    FILE *arq = fopen("Contatos.txt", "a");

    if (alterar == 0)
        printf("_-_-_-_-_-_-_-_-Adicionar Contato-_-_-_-_-_-_-_-_\n\n");
    else
        printf("\nInsira as informacoes corretas do contato.\n");
    
    fflush(stdin);

    printf("Nome: ");
    fgets(nome[quantos_c], 79, stdin);
    nome[quantos_c][strlen(nome[quantos_c]) - 1] = '\0';

    printf("Endereco (Rua e n): ");
    fgets(endereco[quantos_c], 79, stdin);
    endereco[quantos_c][strlen(endereco[quantos_c]) - 1] = '\0';

    printf("Telefone Residencial: ");
    scanf("%lf", &telefone_res[quantos_c]);

    printf("Telefone Celular: ");
    scanf("%lf", &telefone_cel[quantos_c]);

    printf("Data de nascimento: ");
    scanf("%d", &data_nascimento[quantos_c]);
    fprintf(arq, "%s;%s;%.0lf;%.0lf;%d\n", nome[quantos_c], endereco[quantos_c], telefone_res[quantos_c], telefone_cel[quantos_c], data_nascimento[quantos_c]);
    
    if (alterar == 1)
        return;

    printf("\nAdicionado com sucesso.\n");
    printf("\nPressione alguma tecla e enter para continuar.\n");
    scanf("%s", continuar);
    system("cls");
}

//listar contatos
void listar(int remover)
{
    int print_lista[100000];

    FILE *arq = fopen("Contatos.txt", "r");
    
    int x=0, z=0;
    if (arq != NULL)
    {
        if (remover == 0){
            system("cls");
            printf("_-_-_-_-_-_-_-_-Lista dos Contatos-_-_-_-_-_-_-_-_\n\n");
            printf("Voce possui %d contatos:\n", quantos_c);
        }

        fscanf(arq, "%*[^\n]");
        print_lista[0] = fgetc(arq);
        while (print_lista[z] != EOF)
        {
            printf("%c", print_lista[z]);
            z++;
            print_lista[z] = fgetc(arq);
            if (print_lista[z - 1] == '\n' && print_lista[z] != EOF)
            {
                x++;
                printf("%d- ", x);
            }
        }
    }
    else
        printf("\nAinda nao foram adicionados contatos.\n");

    fclose(arq);

    if (remover == 1)
        return;
    
    printf("\nPressione alguma tecla e enter para continuar.\n");
    scanf("%s", continuar);
    system("cls");
}

//remover contatos
void remover(int alterar)
{
    system("cls");
    FILE *input = fopen("Contatos.txt", "r");

    if (input == NULL)
    {
        printf("\nAinda nao foram adicionados contatos.\n");
        printf("\nPressione alguma tecla e enter para continuar.\n");
        scanf("%s", continuar);
        system("cls");
        return;
    }

    quantos_c--;

    FILE *output = fopen("transferir.txt", "w");
    int num_linha;

    if (alterar == 0)
    {
        printf("_-_-_-_-_-_-_-_-Remover Contato-_-_-_-_-_-_-_-_\n");
        printf("\nQue contato quer remover?\n");
    }
    else
        printf("\nQue contato quer alterar?\n");

    listar(1);

    printf("\nEscreva o numero da linha do contato: ");
    scanf("%d", &num_linha);

    fprintf(output, "%d\n", quantos_c);
    unsigned int linha_selecionada = num_linha + 1;
    unsigned int linha_atual = 1;
    while (fgets(linhas_arq, 500, input) != NULL)
    {
        if (linha_atual != linha_selecionada && linha_atual != 1)
        {
            fputs(linhas_arq, output);
        }
        memset(linhas_arq, 0, sizeof(char) * 500);
        linha_atual += 1;
    }
    fclose(input);
    fclose(output);
    remove("Contatos.txt");
    rename("transferir.txt", "Contatos.txt");

    if (alterar == 1)
        return;
    
    printf("\nRemovido com sucesso.\n");
    printf("\nPressione alguma tecla e enter para continuar.\n");
    scanf("%s", continuar);
    system("cls");
}

//pesquisar contatos pelo nome
void pesquisar()
{
    system("cls");
    FILE *arq = fopen("Contatos.txt", "r");
    int achar = 0, i=0;
    char linha_atual[500];
    texto nome_procurado;
    
    if (arq == NULL)
        {
            system("cls");
            printf("\nAinda nao foram adicionados contatos.");
            printf("\nPressione alguma tecla e enter para continuar.\n");
            scanf("%s", continuar);
            system("cls");
            return;
        }
    
    printf("_-_-_-_-_-_-_-_-Pesquisar Contatos-_-_-_-_-_-_-_-_\n");

    fflush(stdin);
    printf("\nEscreva o nome da pessoa: ");
    fgets(nome_procurado, 79, stdin);
    nome_procurado[strlen(nome_procurado) - 1] = '\0';
    printf("\n");
    
    while (fgets(linha_atual, 500, arq) != NULL){
        if ((strstr(linha_atual, nome_procurado)) != NULL)
        {
            printf("%d- ", i);
            printf("%s", linha_atual);
            achar++;
        }
        i++;
    }
    if (achar == 0)
    {
        printf("\nEste nome nao esta registrado.\n");
    }

    printf("\nPressione alguma tecla e enter para continuar.\n");
    scanf("%s", continuar);
    system("cls");

    fclose(arq);
}


//alterar um contato
void alterar()
{
    system("cls");
    FILE *arq = fopen("Contatos.txt", "r");

    if (arq == NULL)
    {
        printf("\nAinda nao foram adicionados contatos.\n");
        printf("\nPressione alguma tecla e enter para continuar.\n");
        scanf("%s", continuar);
        system("cls");
        return;
    }

    printf("_-_-_-_-_-_-_-_-Alterar Contato-_-_-_-_-_-_-_-_\n\n");

    remover(1);
    adicionar(1);
    printf("Alterado com sucesso.\n");
    printf("\nPressione alguma tecla e enter para continuar.\n");
    scanf("%s", continuar);
    system("cls");
}



//funcao main
int main()
{
    quantos_cont();
    int escolha;
    do
    {
        printf("_-_-_-_-_-_-_-_-Agenda de Contatos-_-_-_-_-_-_-_-_");

        printf("\n\n1. Adicionar Contato\n");
        printf("2. Remover Contato\n");
        printf("3. Pesquisar Contato\n");
        printf("4. Alterar Contato\n");
        printf("5. Listar todos os contatos\n");
        printf("6. Sair\n");

        printf("\nEscolha uma opcao: ");
        scanf("%d", &escolha);
        switch (escolha)
        {
        case 1: adicionar(0); break;
        case 2: remover(0); break;
        case 3: pesquisar(); break;
        case 4: alterar(); break;
        case 5: listar(0); break;
        case 6: return 0;
        default: printf("Opcao invalida.\n");
        }
    } while (escolha != 6);
}
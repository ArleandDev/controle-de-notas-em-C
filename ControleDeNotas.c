#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<locale.h>

struct aluno{ 
	int matricula;
	char nome[50];
	char email[50];
	char turma[5];
	float nota1;
	float nota2;
	float media;
	char estado[20];
};
typedef struct aluno ALUNO;

void cabecalho();
void cadastrar();
void dadosCabecalho();
void dadosAluno();
void remover();
ALUNO pesquisar();
void adicionarNotas();
void listar();
ALUNO retornaAluno();

int main(){
  setlocale(LC_ALL, "Portuguese");
  struct aluno *controle;
  int cap;
  int n;  
  int opcao;
  int i;
  FILE *arquivo;

  arquivo = fopen("controle.dat","r");
  if(arquivo==NULL){
    n = 0;    
    cap = 50; 
    controle = (struct aluno *)malloc(sizeof(struct aluno)*cap);
  }else{ 
    fread(&n, sizeof(int), 1, arquivo);
    cap = n*2; 
    controle = (struct aluno *)malloc(sizeof(struct aluno)*cap);
    fread(controle, sizeof(struct aluno), n, arquivo);
    fclose(arquivo);
  }
  
do{
	cabecalho();
	printf("\n1 - Cadastrar Aluno\n2 - Excluir Aluno\n3 - Pesquisar Aluno\n4 - Listar Alunos Cadastrados\n");
	printf("5 - Lançar Notas de um Aluno\n6 - Sair\n\n");
	printf("Escolha uma opção: ");
	scanf("%d", &opcao);

	switch(opcao){
		case 1: cadastrar(); break;	
		case 2: remover(); break;
		case 3: pesquisar(); break;
		case 4: listar(); break;
		case 5: adicionarNotas();break;	
		case 6: printf("Obrigado por utilizar nossos serviços.\n"); break;
		default: printf("Opção Inválida.\n"); break;  	
  	}
}while(opcao!=6);

  free(controle);
  return 0;	
}

void cabecalho(){
	system("cls");
	printf("----------------------------------------------------\n");
	printf("CONTROLE DE NOTAS\n");
	printf("----------------------------------------------------\n");	
}

void cadastrar(){
	FILE* arquivo;
	struct aluno aln;
    char continuar = 'n';
    //int i;
    
    aln.nota1 = -1;
    aln.nota2 = -1;

	arquivo = fopen("controle.dat", "ab");
	if(arquivo == NULL){
		printf("Erro na abertura do arquivo.\n");
	}else{
		do{
			cabecalho();
			fflush(stdin);
			printf("Informe o numero de matricula:");
			scanf("%d", &aln.matricula);
			
			printf("Informe o nome:");
            fflush(stdin);
			scanf("%s", &aln.nome);
			
			printf("Informe o e-mail:");
            fflush(stdin);
			scanf("%s", &aln.email);
			
			printf("Informe a turma do aluno (manhã/tarde):");
            fflush(stdin); 
			scanf("%s", &aln.turma);


            fwrite(&aln, sizeof(ALUNO), 1, arquivo);
			printf("Deseja continuar cadastrando mais alunos? (s/n)");
            fflush(stdin);
            scanf(" %c",&continuar);
		}while(continuar == 's');
		fclose(arquivo);
	}
}

void remover(){
 	FILE* arquivo;
 	FILE* temp;
 	ALUNO aln;
    int matricula;
    char continuar = 'n';
 
 	arquivo = fopen("controle.dat","r");
    remove("tmp.dat");
 	temp = fopen("tmp.dat","ab");
 
 	if(arquivo==NULL||temp==NULL){
  		printf("Erro na abertura do arquivo.\n");
 	}else{
 		aln = pesquisar();
        matricula = aln.matricula;

        while(fread(&aln, sizeof(ALUNO), 1, arquivo)){
                if (aln.matricula != matricula){
                    fwrite(&aln,sizeof(ALUNO), 1, temp);
                }
            }
  		
  		fclose(arquivo);
  		fclose(temp);
  		fflush(stdin);
 		printf("Deseja excluir esse aluno? (s/n)");
 		scanf(" %c",&continuar);
		if(continuar == 's'){
 		
   			if(remove("controle.dat")==0&&rename("tmp.dat","controle.dat")==0){
    			printf("\nAluno removido com sucesso.");
   			}else{
    			printf("\nErro.");

    			remove("tmp.dat");
   			}
   		}else{}
  	}
  		
  	
        getch();
 	}

void listar(){
	FILE* arquivo;
	ALUNO aln;
	
	arquivo = fopen("controle.dat", "r");
	cabecalho();
	if(arquivo == NULL){
		printf("Erro na abertura do arquivo.\n");
	}else{
        dadosCabecalho();
        while(fread(&aln, sizeof(ALUNO), 1, arquivo)){
            dadosAluno(aln);
        }
	}
	fclose(arquivo);
    getch();
}

ALUNO pesquisar(){
	int matricula;
	ALUNO aln;
	cabecalho();
    fflush(stdin);
    printf("Digite a matrícula do aluno:");
    scanf("%d", &matricula);
    aln = retornaAluno(matricula);
    getch();
    return aln;
}

ALUNO retornaAluno(int matricula){
	FILE* arquivo;
	ALUNO aln;

    arquivo = fopen("controle.dat", "r");
    if(arquivo == NULL){
            printf("Erro na abertura do arquivo.\n");
        }
        else{
            dadosCabecalho();
            while(fread(&aln, sizeof(ALUNO), 1, arquivo)){
                if (aln.matricula == matricula){
                    dadosAluno(aln);
                    break;
                }
            }
        }
    fclose(arquivo);
    return aln;
}

void dadosCabecalho(){
        printf("%10s | %10s | %20s | %5s | %s| %4s | %s | %s\n", "MATRICULA", "NOME", "EMAIL", "TURMA", "NOTA1", "NOTA2", "MEDIA", "STATUS");
}

void dadosAluno(ALUNO aln){
    char nota1[32];
    char nota2[32];
    char media[32];
    char estado[32];

    sprintf(nota1, "%.2f", aln.nota1);
    sprintf(nota2, "%.2f", aln.nota2);
    sprintf(media, "%.2f", aln.media);
	if (aln.nota1 < 0)
	    strcpy(nota1,"Sem nota.");
	if (aln.nota2 < 0)
	    strcpy(nota2,"Sem nota.");


    printf("%10d | %10s | %20s | %5s | %s | %s | %s | %s\n", aln.matricula, aln.nome, aln.email, aln.turma, nota1, nota2, media, aln.estado);
}

void adicionarNotas(){
 	FILE* arquivo;
 	FILE* temp;
 	ALUNO aln;
    int matricula;
    int num;
    
 
 	arquivo = fopen("controle.dat","r");
    remove("tmp.dat");
 	temp = fopen("tmp.dat","ab");
 
 	if(arquivo==NULL||temp==NULL){
  		printf("Erro na abertura do arquivo.\n");
 	}else{
 		aln = pesquisar();
        matricula = aln.matricula;

        while(fread(&aln, sizeof(ALUNO), 1, arquivo)){
                if (aln.matricula == matricula){
                    printf("Digite o número da nota que deseja alterar\n1 - Primeira Nota\n2 - Segunda Nota\n");
				    scanf("%d", &num);
						switch(num){
							case 1:
								printf("Informe a nova nota: ");
								scanf("%f", &aln.nota1);
								printf("Nova nota: %0.2f", aln.nota1);
							break;
								
							case 2:
								printf("Informe a nova nota: ");
								scanf("%f", &aln.nota2);
								printf("Nova nota: %0.2f", aln.nota2);
							break;
						}
						if (aln.nota1>0 && aln.nota2>0){
	    				aln.media = (aln.nota1+aln.nota2)/2;
						}
						if ((aln.nota1+aln.nota2)/2 >= 7.0){
							sprintf(aln.estado, "Aprovado");
						}
						if ((aln.nota1+aln.nota2)/2 < 7.0){
							sprintf(aln.estado, "Reprovado");
						}

                }
                fwrite(&aln,sizeof(ALUNO), 1, temp);
            }
  		
  		fclose(arquivo);
  		fclose(temp);
  		fflush(stdin);
 
   		if(remove("controle.dat")==0&&rename("tmp.dat","controle.dat")==0){
    		printf("\nNotas adicionadas com sucesso.");
   		}else{
    		printf("\nErro.");

    		remove("tmp.dat");
   		}
  		}
        getch();
 	}
